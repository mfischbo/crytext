#ifndef SMTPSERVICE_H
#define SMTPSERVICE_H

#include <Poco/Net/SMTPClientSession.h>

#include "service/crytextservice.h"
#include "model/crytextfile.h"
#include "model/sticker.h"

namespace crytext {

class SMTPService
{
private :
    CryTextService* service;
    std::string hostname;
    std::string username;
    std::string password;
    Poco::Net::SMTPClientSession::LoginMethod method;

    Poco::Net::SMTPClientSession* session;

public:
    SMTPService(CryTextService* service);
    ~SMTPService();

    bool connect();
    bool disconnect();

    bool sendCrytextFile(const Sticker *sticker, const CryTextFile *file);
    bool sendSticker(const QString &recipient, const QString *message);
};

}
#endif // SMTPSERVICE_H
