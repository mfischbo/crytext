#ifndef STICKER_H
#define STICKER_H

#include <QString>
#include <crypto++/rsa.h>
#include "cryptutils.h"

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

#endif // STICKER_H
