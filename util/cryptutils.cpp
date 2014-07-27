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

#include "util/cryptutils.h"

#include <QDebug>
#include <QString>
#include <QFile>
#include <QStringList>
#include <QRegExp>

#include <crypto++/rsa.h>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>

#include <crypto++/osrng.h>
#include <crypto++/base64.h>
#include <crypto++/hex.h>
#include <crypto++/files.h>

using namespace crytext;
using CryptoPP::AES;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::FileSink;
using CryptoPP::FileSource;
using CryptoPP::ByteQueue;
using CryptoPP::HexDecoder;
using CryptoPP::HexEncoder;
using CryptoPP::Base64Decoder;
using CryptoPP::Base64Encoder;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;


CryptUtils::CryptUtils()
{

}


bool CryptUtils::generateRSAKeyPair(const QString &dirname) {

    AutoSeededRandomPool rng;
    CryptoPP::RSA::PrivateKey key;
    key.GenerateRandomWithKeySize(rng, 3072);

    // save the private key file
    QString filename = QString(dirname).append("/rsa-private.key");
    FileSink fs1(filename.toStdString().c_str());

    ByteQueue queue;
    key.Save(queue);
    queue.CopyTo(fs1);
    fs1.MessageEnd();
    qDebug() << "Saved private key in " << filename;

    // save the public key file
    QString pubFile = QString(dirname).append("/rsa-public.key");
    FileSink fs2(pubFile.toStdString().c_str());
    ByteQueue pubQueue;
    CryptoPP::RSA::PublicKey pubKey(key);
    pubKey.Save(pubQueue);
    pubQueue.CopyTo(fs2);
    fs2.MessageEnd();
    qDebug() << "Saved public key in " << pubFile;

    return true;
}

bool
CryptUtils::readKeyPairFrom(const QString &dirname, CryptoPP::RSA::PrivateKey &privateKey,
                            CryptoPP::RSA::PublicKey &publicKey) {

    qDebug() << "Reading keypair from " << dirname;

    QString prvFilename = QString(dirname).append("/rsa-private.key");
    QFile f(prvFilename);
    if (!f.exists()) {
        qDebug() << "No key file found at " << prvFilename;
        return false;
    }

    FileSource fs1(prvFilename.toStdString().c_str(), true);
    ByteQueue queue;
    fs1.TransferTo(queue);
    queue.MessageEnd();
    privateKey.Load(queue);

    QString pubFilename = QString(dirname).append("/rsa-public.key");
    QFile f2(pubFilename);
    if (!f2.exists()) {
        qDebug() << "No key file found at " << pubFilename;
        return false;
    }
    FileSource fs2(pubFilename.toStdString().c_str(), true);
    ByteQueue q2;
    fs2.TransferTo(q2);
    q2.MessageEnd();
    publicKey.Load(q2);

    AutoSeededRandomPool rng;
    if (!privateKey.Validate(rng, 3)) {
        qDebug() << "Failed to validate private key";
        return false;
    }
    if (!publicKey.Validate(rng, 3)) {
        qDebug() << "Failed to validate public key";
        return false;
    }
    return true;
}


QString
CryptUtils::aesEncrypt(const byte* key, const byte* iv, QString plaintext) const {

    qDebug() << "Running aesEncrypt() with keysize " << AES::DEFAULT_KEYLENGTH << "\tBlocksize : " << AES::BLOCKSIZE;

    std::string stdPlain = plaintext.toStdString();
    std::string stdCipher;

    CryptoPP::AES::Encryption aesEnc(key, AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEnc(aesEnc, iv);

    StreamTransformationFilter stfEnc(cbcEnc, new StringSink(stdCipher));
    stfEnc.Put(reinterpret_cast<const unsigned char*> (stdPlain.c_str()), stdPlain.length());
    stfEnc.MessageEnd();

    qDebug() << "Resulted in cipher with length " << stdCipher.length();

    return QString::fromStdString(stdCipher);
}

QString
CryptUtils::aesDecrypt(const byte* key, const byte* iv, QString ciphertext) const {
    qDebug() << "Running aesDecrypt() with keysize " << AES::DEFAULT_KEYLENGTH << "\tBlocksize : " << AES::BLOCKSIZE;

    CryptoPP::AES::Decryption aesDecr(key, AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecr(aesDecr, iv);

    qDebug() << "Blocksize is : " << AES::BLOCKSIZE;
    qDebug() << "cipher length: " << ciphertext.length();

    std::string stdPlain;
    StreamTransformationFilter stfDecryptor(cbcDecr, new StringSink(stdPlain));
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>(ciphertext.toStdString().c_str()), ciphertext.length());
    stfDecryptor.MessageEnd();
    return QString::fromStdString(stdPlain);
}

QString
CryptUtils::rsaEncrypt(const byte *plaintext, int length, CryptoPP::RSA::PublicKey* key) const {

    AutoSeededRandomPool rng;
    std::string retval;
    CryptoPP::RSAES_OAEP_SHA_Encryptor enc(*key);
    CryptoPP::StringSource(plaintext, length, true,
                           new CryptoPP::PK_EncryptorFilter(rng, enc,
                             new CryptoPP::StringSink(retval)));
    return QString::fromStdString(retval);
}


QString
CryptUtils::rsaDecrypt(const byte* ciphertext, int length, CryptoPP::RSA::PrivateKey* key) const {

    AutoSeededRandomPool rng;
    std::string retval;
    CryptoPP::RSAES_OAEP_SHA_Decryptor dec(*key);
    CryptoPP::StringSource(ciphertext, length, true,
          new CryptoPP::PK_DecryptorFilter(rng, dec,
            new CryptoPP::StringSink(retval)));
    return QString::fromStdString(retval);
}

void
CryptUtils::generateRandomAESKey(byte* ptr) const {
    AutoSeededRandomPool rng;
    rng.GenerateBlock(ptr, CryptoPP::AES::DEFAULT_KEYLENGTH);
}

void
CryptUtils::generateRandomAES_IV(byte *ptr) const {
    AutoSeededRandomPool rng;
    rng.GenerateBlock(ptr, CryptoPP::AES::BLOCKSIZE);
}

void
CryptUtils::publicKeyAsHex(QString &target, CryptoPP::RSA::PublicKey &key) const {
    HexEncoder enc;
    ByteQueue q;

    std::string sink;
    enc.Attach(new StringSink(sink));

    key.Save(q);
    q.CopyTo(enc);
    enc.MessageEnd();
    target.append(QString::fromStdString(sink));
}

bool
CryptUtils::loadPublicKey(std::string &hex, CryptoPP::RSA::PublicKey &pk) const {
    const byte *p = (unsigned char*) hex.data();

    HexDecoder dec;
    dec.Put(p, hex.length());
    dec.MessageEnd();

    pk.Load(dec);

    // check the key for validity
    AutoSeededRandomPool rng;
    return pk.Validate(rng, 3);
}

void
CryptUtils::fromHex(const std::string &str, std::string &output) const {
    StringSource sSource(str, true, new HexDecoder(new StringSink(output)));
}

void
CryptUtils::toHex(const std::string &str, std::string &output) const {
    StringSource sSource(str, true, new HexEncoder(new StringSink(output)));
}

void
CryptUtils::toBase64(const std::string &str, std::string &output) const {

    const byte *p = (unsigned char*) str.c_str();

    Base64Encoder enc;
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

    qDebug() << "fromBase64() source length is " << str.length() << " bytes";
    const byte *p = (unsigned char*) str.c_str();

    Base64Decoder dec;
    dec.Put(p, str.length());
    dec.MessageEnd();
    CryptoPP::word64 size = dec.MaxRetrievable();
    qDebug() << "fromBase64() decoded size is " << size;
    if (size) {
        output.resize(size);
        dec.Get((byte*) output.data(), output.size());
    }
}

