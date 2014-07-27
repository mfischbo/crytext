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
