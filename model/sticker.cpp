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

#include "sticker.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <crypto++/rsa.h>
#include "util/cryptutils.h"

using namespace crytext;

Sticker::Sticker(QString &filename, const CryptUtils *utils)
{
    QFile f(filename);
    if (!f.open(QIODevice::Text | QIODevice::ReadOnly)) {
        qDebug() << "Could not read sticker from " << filename;
    }
    QTextStream inStream(&f);
    this->email = inStream.readLine();
    this->firstName = inStream.readLine();
    this->lastName = inStream.readLine();
    QString pubK = inStream.readLine();
    std::string hex = pubK.toStdString();
    f.close();

    qDebug() << "Public Key Hex as length : " << pubK.length();

    bool valid = utils->loadPublicKey(hex, publicKey);
    if (!valid) {
        qDebug() << "Public key seems not to be valid";
    } else
        qDebug() << "Successfully created sticker for email " << this->email;
}

Sticker::~Sticker() {
    //delete this->publicKey;
}

QString *Sticker::getEMail() {
    return &email;
}

QString*
Sticker::getFirstName() {
    return &firstName;
}

QString*
Sticker::getLastName() {
    return &lastName;
}

CryptoPP::RSA::PublicKey*
Sticker::getPublicKey() {
    return &publicKey;
}

