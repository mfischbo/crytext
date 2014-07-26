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
    f.close();

    publicKey = CryptoPP::RSA::PublicKey();
    bool valid = utils->readPublicKeyFromHex(pubK, publicKey);
    if (!valid) {
        qDebug() << "Public key seems not to be valid";
    } else
        qDebug() << "Successfully created sticker for email " << this->email;
}

Sticker::~Sticker() {
    //delete this->publicKey;
}

QString*
Sticker::getEMail() {
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

