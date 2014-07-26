#include <QDebug>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/Net/NetException.h>
#include "smtpservice.h"
#include "util/cryptutils.h"

using crytext::SMTPService;
using Poco::Net::MailMessage;
using Poco::Net::MailRecipient;
using Poco::Net::StringPartSource;
using Poco::Net::NetException;
using Poco::Net::SMTPException;

SMTPService::SMTPService(CryTextService *service)
{
    this->service = service;
    Settings *s = service->getSettings();
    QString host = s->getSMTPHost();
    Poco::UInt16 port = (Poco::UInt16) s->getSMTPPort();
    this->session = new Poco::Net::SMTPClientSession(host.toStdString(), port);

    this->hostname = s->getSMTPHost().toStdString();
    this->username = s->getSMTPPUsername().toStdString();
    this->password = s->getSMTPPassword().toStdString();

    switch (s->getSMTPAuthMethod()) {
    case NO_AUTH:
        this->method = Poco::Net::SMTPClientSession::AUTH_NONE;
        break;
    case PASSWORD:
        this->method = Poco::Net::SMTPClientSession::AUTH_LOGIN;
    }
}

SMTPService::~SMTPService() {
    delete session;
}


bool
SMTPService::sendCrytextFile(const Sticker* sticker, const CryTextFile* file) {
    return false;
}

bool
SMTPService::sendSticker(const QString &recipient, const QString &subject, const QString &message) {

    /*
    MailRecipient mRecp = MailRecipient(MailRecipient::PRIMARY_RECIPIENT,
                 recipient.toStdString());
    */
    MailMessage msg;
    std::string sender = this->service->getSettings()->getStickerEMail().toStdString();
    msg.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, recipient.toStdString()));
    msg.setSubject(MailMessage::encodeWord(subject.toStdString()));
    msg.setContentType("text/plain;charset=UTF-8");
    msg.setSender(sender);

    // attach the users message
    StringPartSource *cps = new StringPartSource(message.toStdString());
    msg.addContent(cps, MailMessage::ENCODING_8BIT);


    // add the attachment
    QString streamBuffer;
    QTextStream stream(&streamBuffer);
    service->exportPrivateSticker(stream);

    // assemble the filename for the attachment
    QString fname = QString(service->getSettings()->getStickerEMail());
    fname.append(".cts");

    StringPartSource *sps = new StringPartSource(stream.readAll().toStdString());
    msg.addAttachment(fname.toStdString(), sps);


    // send the message
    try {
        this->session->login(this->method, this->username, this->password);
        this->session->sendMessage(msg);
        this->session->close();
        qDebug() << "Successfully sent message containing users sticker to " << recipient;
    } catch (SMTPException e) {
        qDebug() << "Caught exception " << QString::fromStdString(e.message()) << " while sending the sticker";
        this->session->close();
        return false;
    } catch (NetException ne) {
        qDebug() << "Caught exception " << QString::fromStdString(ne.message()) << " while sending the sticker";
        return false;
    }
    return true;
}
