#include <QDebug>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/Net/NetException.h>
#include "smtpservice.h"
#include "util/cryptutils.h"

using namespace crytext;

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
SMTPService::connect() {
    try {
        this->session->login(this->method, this->username, this->password);

    } catch (Poco::Net::NetException e) {
        qDebug() << "Caught exception during login. Cause: " << QString::fromStdString(e.message());
    }
}

bool
SMTPService::disconnect() {
    this->session->close();
    return true;
}

bool
SMTPService::sendCrytextFile(const Sticker* sticker, const CryTextFile* file) {

}

bool
SMTPService::sendSticker(const QString &recipient, const QString *message) {

    Poco::Net::MailRecipient mRecp =
            Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,
                 recipient.toStdString());

    Poco::Net::MailMessage m;
    m.addRecipient(mRecp);
    m.setSubject(std::string("My Crytext Sticker"));

    // add the attachment
    CryptUtils utils;
    QString streamBuffer;
    QTextStream stream(&streamBuffer);
    service->exportPrivateSticker(stream);

    std::string base64Str;
    utils.toBase64(stream.readAll().toStdString(), base64Str);
    Poco::Net::StringPartSource sps(base64Str);

    // assemble the filename for the attachment
    QString fname = QString(service->getSettings()->getStickerEMail());
    fname.append(".cts");
    m.addAttachment(fname.toStdString(), &sps);

    Poco::Net::StringPartSource cps(message->toStdString());
    m.addContent(&cps);

    // send the message
    try {
        this->session->sendMessage(m);
    } catch (Poco::Net::SMTPException e) {
        qDebug() << "Caught exception " << QString::fromStdString(e.message()) << " while sending the sticker";
        return false;
    } catch (Poco::Net::NetException ne) {
        qDebug() << "Caught exception " << QString::fromStdString(ne.message()) << " while sending the sticker";
        return false;
    }
    return true;
}
