#include "crytextfile.h"

#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QTextStream>
#include <QDebug>


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

    this->utils->fromHex(tokens[0].toStdString(), this->aesKeyCipher);
    this->utils->fromHex(tokens[1].toStdString(), this->aesIVCipher);

    std::string ctxt;
    this->utils->fromHex(tokens[2].toStdString(), ctxt);
    this->ciphertext = QString::fromStdString(ctxt);
}

CryTextFile::CryTextFile(const QTextDocument *document, const CryptUtils *utils) {

    this->utils = utils;
    this->utils->generateRandomAESKey(this->aesKey);
    this->utils->generateRandomAES_IV(this->iv);
    this->plaintext = document->toPlainText();
}


void
CryTextFile::encrypt(CryptoPP::RSA::PublicKey *publicKey) {

    // encrypt the plaintext with aes
    this->ciphertext = utils->aesEncrypt(this->aesKey, this->iv, this->plaintext);

    // encrypt the AES key and iv with the public key
    QString cipher = utils->rsaEncrypt(this->aesKey, CryptoPP::AES::DEFAULT_KEYLENGTH, publicKey);
    this->aesKeyCipher = cipher.toStdString();

    QString ivCipher = utils->rsaEncrypt(this->iv, CryptoPP::AES::BLOCKSIZE, publicKey);
    this->aesIVCipher = ivCipher.toStdString();

    if (this->aesKeyCipher.length() == 0)
        qDebug() << "Failed to encypt AES key";
    if (this->aesIVCipher.length() == 0)
        qDebug() << "Failed to encrypt AES IV";
}

void
CryTextFile::decrypt(CryptoPP::RSA::PrivateKey *privateKey) {

    // decrypt AES key and IV
    QString keyString = utils->rsaDecrypt((byte*) this->aesKeyCipher.data(), this->aesKeyCipher.length(), privateKey);
    QString ivString = utils->rsaDecrypt((byte *) this->aesIVCipher.data(), this->aesIVCipher.length(), privateKey);

    memcpy(this->aesKey, keyString.toStdString().c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
    memcpy(this->iv, ivString.toStdString().c_str(), CryptoPP::AES::BLOCKSIZE);

    // decrypt the cipher text with the yielded key
    this->plaintext = this->utils->aesDecrypt(this->aesKey, this->iv, this->ciphertext);
}


bool
CryTextFile::saveTo(QTextStream &stream) {

    // write the encrypted aes key
    std::string b64KeyCipher;
    this->utils->toHex(aesKeyCipher, b64KeyCipher);
    stream << QString::fromStdString(b64KeyCipher);
    stream << "\n\n";

    // write the encrypted AES IV
    std::string b64IVCipher;
    this->utils->toHex(aesIVCipher, b64IVCipher);
    stream << QString::fromStdString(b64IVCipher);
    stream << "\n\n";

    // write the actual payload
    std::string b64Payload;
    std::string cypher = this->ciphertext.toStdString();
    this->utils->toHex(cypher, b64Payload);
    stream << QString::fromStdString(b64Payload);
    stream << "\n";
    return true;
}




QString
CryTextFile::getPlainText() {
    return this->plaintext;
}


CryTextFile::~CryTextFile() {

}
