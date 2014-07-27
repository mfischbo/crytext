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

#ifndef CRYTEXTFILE_H
#define CRYTEXTFILE_H

#include <QString>
#include <QTextDocument>
#include <QTextStream>
#include <QByteArray>
#include <QFile>
#include <crypto++/aes.h>
#include <crypto++/rsa.h>

#include "util/cryptutils.h"

namespace crytext {

class CryTextFile
{
public:

    /**
     * @brief CryTextFile Creates a new crytext file from the input stream.
     * Call this constructor for encrypted files only!
     * @param inStream The input stream of the encrypted file
     */
    CryTextFile(QTextStream &inStream, const CryptUtils *utils, QString emailPart);

    /**
     * @brief CryTextFile Creates a new crytext file from a given QTextDocument for encryption
     * @param document The document that should be encrypted and stored
     */
    CryTextFile(const QTextDocument *document, const CryptUtils *utils);

    /**
     * @brief encrypt Encrypts the file using the specified public key
     * @param publicKey The public key used for encryption
     */
    void encrypt(CryptoPP::RSA::PublicKey *publicKey);

    /**
     * @brief decrypt Decryptes the file using the specified private key
     * @param privateKey The private key to be used for decryption
     */
    void decrypt(CryptoPP::RSA::PrivateKey *privateKey);

    /**
     * @brief getPlainText Returns the plaintext of the underlying file
     * @return The plaintext
     */
    QString getPlainText();

    /**
     * @brief saveTo Saves the file to the output stream in an encrypted manner.
     * @param stream The stream to write the file to
     * @return True on success, otherwise false
     */
    bool saveTo(QTextStream &stream);

    // destructor
    ~CryTextFile();



private:

    const CryptUtils *utils;

    // the aes key to be used to de-/encrypt the plaintext
    byte aesKey[ CryptoPP::AES::DEFAULT_KEYLENGTH];

    // the iv used to initialize the block stream
    byte iv[ CryptoPP::AES::BLOCKSIZE ];

    // the rsa private key
    CryptoPP::RSA::PrivateKey *prvKey;

    // the rsa public key
    CryptoPP::RSA::PublicKey  *pubKey;

    // the rsa encrypted aes key as stored in the file
    std::string aesKeyCipher;

    // the rsa encrypted iv as stored in the file
    std::string aesIVCipher;

    QString plaintext;      // the files content plain
    QString ciphertext;     // the files content aes encrypted
};

}
#endif // CRYTEXTFILE_H
