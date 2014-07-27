/*
This file is part of crytext.

 Copyright 2014 Markus Fischböck

 crytext is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 crytext is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with crytext.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QDebug>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/StringPartSource.h>
#include <Poco/Net/NetException.h>
#include "smtpservice.h"
#include "util/cryptutils.h"

using namespace crytext;
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
SMTPService::sendCrytextFile(Sticker* sticker, CryTextFile* file, const QString &subject, const QString &message) {

    MailMessage msg;
    std::string sender = service->getSettings()->getStickerEMail().toStdString();
    std::string recp = sticker->getEMail()->toStdString();
    msg.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, recp));
    msg.setSubject(MailMessage::encodeWord(subject.toStdString()));
    msg.setContentType("text/plain;charset=UTF-8");
    msg.setSender(sender);

    // users text
    StringPartSource *cps = new StringPartSource(message.toStdString());
    msg.addContent(cps, MailMessage::ENCODING_8BIT);

    // the crytext file
    QString streamBuffer;
    QTextStream stream(&streamBuffer);

    // TODO: Crytextfile should have something like .toString() for this
    stream << "====BOUNDARY:";
    stream << *sticker->getEMail() << "\n";

    if (file->saveTo(stream)) {
        StringPartSource *sps = new StringPartSource(stream.readAll().toStdString());
        msg.addAttachment("testfile.cry", sps);
        try {
            this->session->login(method, username, password);
            this->session->sendMessage(msg);
            this->session->close();
        } catch (SMTPException ex) {
            qDebug() << "Erorr sending the mail. Cause: " << QString::fromStdString(ex.message());
            this->session->close();
            return false;
        } catch (NetException ex2) {
            qDebug() << "Error sending the mail. Cause: " << QString::fromStdString(ex2.message());
            return false;
        }
        return true;
    }
    return false;
}

bool
SMTPService::sendSticker(const QString &recipient, const QString &subject, const QString &message) {

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
