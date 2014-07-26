#ifndef SMTPSERVICE_H
#define SMTPSERVICE_H

#include <Poco/Net/SMTPClientSession.h>

#include "service/crytextservice.h"
#include "model/crytextfile.h"
#include "model/sticker.h"

using Poco::Net::SMTPClientSession;

namespace crytext {

class SMTPService
{
private :
    CryTextService* service;
    std::string hostname;
    std::string username;
    std::string password;
    SMTPClientSession::LoginMethod method;
    SMTPClientSession* session;

public:
    SMTPService(CryTextService* service);
    ~SMTPService();

    bool sendCrytextFile(Sticker *sticker, CryTextFile *file, const QString &subject, const QString &message);
    bool sendSticker(const QString &recipient, const QString &subject, const QString &message);
};

}
#endif // SMTPSERVICE_H
