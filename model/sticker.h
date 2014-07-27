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

#ifndef STICKER_H
#define STICKER_H

#include <QString>
#include <crypto++/rsa.h>
#include "util/cryptutils.h"

namespace crytext {
class Sticker
{

private:

    QString email;
    QString firstName;
    QString lastName;
    CryptoPP::RSA::PublicKey publicKey;

public:

    Sticker(QString &filename, const CryptUtils *utils);
    ~Sticker();

    QString* getEMail();
    QString* getFirstName();
    QString* getLastName();
    CryptoPP::RSA::PublicKey* getPublicKey();

};
}
#endif // STICKER_H
