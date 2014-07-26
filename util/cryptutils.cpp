#include "cryptutils.h"

#include <QDebug>
#include <QString>
#include <QFile>
#include <QStringList>
#include <QRegExp>

#include <crypto++/asn.h>
#include <crypto++/rsa.h>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>

#include <crypto++/rsa.h>
#include <crypto++/osrng.h>
#include <crypto++/base64.h>
#include <crypto++/hex.h>
#include <crypto++/files.h>

using namespace crytext;

CryptUtils::CryptUtils()
{

}


bool CryptUtils::generateRSAKeyPair(const QString &dirname) {

    CryptoPP::AutoSeededRandomPool rnd;
    CryptoPP::RSA::PrivateKey key;
    key.GenerateRandomWithKeySize(rnd, 3072);

    // save the private key file
    QString filename = QString(dirname).append("/rsa-private.key");
    CryptoPP::FileSink fs1(filename.toStdString().c_str());

    CryptoPP::ByteQueue queue;
    key.Save(queue);
    queue.CopyTo(fs1);
    fs1.MessageEnd();
    qDebug() << "Saved private key in " << filename;

    // save the public key file
    QString pubFile = QString(dirname).append("/rsa-public.key");
    CryptoPP::FileSink fs2(pubFile.toStdString().c_str());
    CryptoPP::ByteQueue pubQueue;
    CryptoPP::RSA::PublicKey pubKey(key);
    pubKey.Save(pubQueue);
    pubQueue.CopyTo(fs2);
    fs2.MessageEnd();
    qDebug() << "Saved public key in " << pubFile;

    return true;
}

bool CryptUtils::readKeyPairFrom(const QString &dirname) {

    qDebug() << "Reading keypair from " << dirname;

    QString prvFilename = QString(dirname).append("/rsa-private.key");
    QFile f(prvFilename);
    if (!f.exists()) {
        qDebug() << "No key file found at " << prvFilename;
        return false;
    }

    CryptoPP::FileSource fs1(prvFilename.toStdString().c_str(), true);
    CryptoPP::ByteQueue queue;
    fs1.TransferTo(queue);
    queue.MessageEnd();
    this->privateKey.Load(queue);

    QString pubFilename = QString(dirname).append("/rsa-public.key");
    QFile f2(pubFilename);
    if (!f2.exists()) {
        qDebug() << "No key file found at " << pubFilename;
        return false;
    }
    CryptoPP::FileSource fs2(pubFilename.toStdString().c_str(), true);
    CryptoPP::ByteQueue q2;
    fs2.TransferTo(q2);
    q2.MessageEnd();
    this->publicKey.Load(q2);

    CryptoPP::AutoSeededRandomPool rnd;
    if (!this->privateKey.Validate(rnd, 3)) {
        qDebug() << "Failed to validate private key";
        return false;
    }
    if (!this->publicKey.Validate(rnd, 3)) {
        qDebug() << "Failed to validate public key";
        return false;
    }
    return true;
}

CryptoPP::RSA::PublicKey*
CryptUtils::getRSAPublicKey() {
    return &publicKey;
}

CryptoPP::RSA::PrivateKey*
CryptUtils::getRSAPrivateKey() {
    return &privateKey;
}

QString
CryptUtils::aesEncrypt(const byte* key, const byte* iv, QString plaintext) const {

    std::string stdPlain = plaintext.toStdString();
    std::string stdCipher;

    CryptoPP::AES::Encryption aesEnc(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEnc(aesEnc, iv);
    CryptoPP::StreamTransformationFilter stfEnc(cbcEnc, new CryptoPP::StringSink(stdCipher));
    stfEnc.Put(reinterpret_cast<const unsigned char*> (stdPlain.c_str()), stdPlain.length() + 1);
    stfEnc.MessageEnd();
    return QString::fromStdString(stdCipher);
}

QString
CryptUtils::aesDecrypt(const byte* key, const byte* iv, QString ciphertext) const {

    CryptoPP::AES::Decryption aesDecr(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecr(aesDecr, iv);

    std::string stdPlain;
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecr, new CryptoPP::StringSink(stdPlain));
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>(ciphertext.toStdString().c_str()), ciphertext.length());
    stfDecryptor.MessageEnd();
    return QString::fromStdString(stdPlain);
}

void
CryptUtils::publicKeyAsHex(QString &target) const {
    CryptoPP::HexEncoder enc;
    CryptoPP::ByteQueue q;

    std::string sink;
    enc.Attach(new CryptoPP::StringSink(sink));

    this->publicKey.Save(q);
    q.CopyTo(enc);
    enc.MessageEnd();

    target.append(QString::fromStdString(sink));
}

bool
CryptUtils::readPublicKeyFromHex(QString &base64, CryptoPP::RSA::PublicKey &pk) const {
    const byte *p = (unsigned char*) base64.toStdString().c_str();

    CryptoPP::HexDecoder dec;
    dec.Put(p, base64.toStdString().length());
    dec.MessageEnd();

    pk.Load(dec);

    // check the key for validity
    CryptoPP::AutoSeededRandomPool rng;
    return pk.Validate(rng, 3);
}

void
CryptUtils::toBase64(const std::string &str, std::string &output) const {

    const byte *p = (unsigned char*) str.c_str();

    CryptoPP::Base64Encoder enc;
    enc.Put(p, str.length());
    enc.MessageEnd();
    CryptoPP::word64 size = enc.MaxRetrievable();
    if (size) {
        output.resize(size);
        enc.Get((byte*) output.data(), output.size());
    }
}

void
CryptUtils::fromBase64(const std::string &str, std::string &output) const {
    const byte *p = (unsigned char*) str.c_str();

    CryptoPP::Base64Decoder dec;
    dec.Put(p, str.length());
    dec.MessageEnd();
    CryptoPP::word64 size = dec.MaxRetrievable();
    if (size) {
        output.resize(size);
        dec.Get((byte*) output.data(), output.size());
    }
}

