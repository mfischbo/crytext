#include "crytextfile.h"

#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QTextStream>
#include <QDebug>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/base64.h>
#include <crypto++/osrng.h>
#include <crypto++/files.h>
#include <crypto++/hex.h>

using namespace crytext;

CryTextFile::CryTextFile(QTextStream &inStream, const CryptUtils *utils, QString emailPart)
{

    this->utils = utils;
    QString contents = inStream.readAll();

    // pick the part to read
    QString myPart;
    QStringList parts = contents.split(QRegExp("====BOUNDARY:", Qt::CaseSensitive), QString::SkipEmptyParts);
    QStringListIterator it = QStringListIterator(parts);
    int i=1;
    while (it.hasNext()) {
        QString p = it.next();
        qDebug() << p;

        QString mail = p.left(p.indexOf("\n"));
        if (mail == emailPart) {
            p.remove(0, p.indexOf("\n") +1);
            myPart = p;
            qDebug() << "Using part #" << i;
        }
        i++;
    }

    QStringList tokens = myPart.split(QRegExp("\\n{2}"));
    qDebug() << "found " << tokens.length() << " tokens";
    if (tokens.length() != 3) {
        qDebug() << "Something went wrong";
    }

    this->utils->fromBase64(tokens[0].toStdString(), this->aesKeyCipher);
    this->utils->fromBase64(tokens[1].toStdString(), this->aesIVCipher);

    std::string ctxt;
    this->utils->fromBase64(tokens[2].toStdString(), ctxt);
    this->ciphertext = QString::fromStdString(ctxt);
}

CryTextFile::CryTextFile(const QTextDocument *document, const CryptUtils *utils) {

    this->utils = utils;

    CryptoPP::AutoSeededRandomPool rng;
    rng.GenerateBlock(this->aesKey, CryptoPP::AES::DEFAULT_KEYLENGTH);
    rng.GenerateBlock(this->iv, CryptoPP::AES::BLOCKSIZE);
    this->plaintext = document->toPlainText();
}


void
CryTextFile::encrypt(CryptoPP::RSA::PublicKey *publicKey) {

    // encrypt the plaintext with aes
    this->ciphertext = utils->aesEncrypt(this->aesKey, this->iv, this->plaintext);

    CryptoPP::AutoSeededRandomPool rng;
    qDebug() << "Checking public key validity";
    if (!publicKey->Validate(rng, 3))
        qDebug() << "Key is not valid";
    else
        qDebug() << "valid!";


    CryptoPP::RSAES_OAEP_SHA_Encryptor enc(*publicKey);
    CryptoPP::StringSource(this->aesKey, CryptoPP::AES::DEFAULT_KEYLENGTH ,true,
                           new CryptoPP::PK_EncryptorFilter(rng, enc,
                             new CryptoPP::StringSink(aesKeyCipher)));

    CryptoPP::StringSource(this->iv, CryptoPP::AES::BLOCKSIZE, true,
                           new CryptoPP::PK_EncryptorFilter(rng, enc,
                              new CryptoPP::StringSink(aesIVCipher)));

    if (this->aesKeyCipher.length() == 0)
        qDebug() << "Failed to encypt AES key";
    if (this->aesIVCipher.length() == 0)
        qDebug() << "Failed to encrypt AES IV";
}

void
CryTextFile::decrypt(CryptoPP::RSA::PrivateKey *privateKey) {

    // decrypt AES key and IV
    CryptoPP::AutoSeededRandomPool rng;
    qDebug() << "Checking validity of the private key";
    if (!privateKey->Validate(rng, 3)) {
        qDebug() << "Key is invalid";
    }

    std::string keyString;
    CryptoPP::RSAES_OAEP_SHA_Decryptor dec(*privateKey);
    CryptoPP::StringSource(this->aesKeyCipher, true,
          new CryptoPP::PK_DecryptorFilter(rng, dec,
            new CryptoPP::StringSink(keyString)));

    std::string ivString;
    CryptoPP::StringSource(this->aesIVCipher, true,
          new CryptoPP::PK_DecryptorFilter(rng, dec,
             new CryptoPP::StringSink(ivString)));

    memcpy(this->aesKey, keyString.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
    memcpy(this->iv, ivString.c_str(), CryptoPP::AES::BLOCKSIZE);

    // decrypt the cipher text with the yielded key
    this->plaintext = this->utils->aesDecrypt(this->aesKey, this->iv, this->ciphertext);
}


bool
CryTextFile::saveTo(QTextStream &stream) {

    // write the encrypted aes key
    std::string b64KeyCipher;
    this->utils->toBase64(aesKeyCipher, b64KeyCipher);
    stream << QString::fromStdString(b64KeyCipher);
    stream << "\n";

    // write the encrypted AES IV
    std::string b64IVCipher;
    this->utils->toBase64(aesIVCipher, b64IVCipher);
    stream << QString::fromStdString(b64IVCipher);
    stream << "\n";

    // write the actual payload
    std::string b64Payload;
    std::string cypher = this->ciphertext.toStdString();
    this->utils->toBase64(cypher, b64Payload);
    stream << QString::fromStdString(b64Payload);
    return true;
}




QString
CryTextFile::getPlainText() {
    return this->plaintext;
}


CryTextFile::~CryTextFile() {

}
