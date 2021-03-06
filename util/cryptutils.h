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

#ifndef CRYTPUTILS_H
#define CRYTPUTILS_H

#include <QString>
#include <crypto++/rsa.h>
#include <crypto++/aes.h>
#include <crypto++/osrng.h>

using CryptoPP::AutoSeededRandomPool;

namespace crytext {

class CryptUtils
{
private:

public:
    CryptUtils();

    /**
     * @brief readKeyPairFrom Reads the users RSA keypair from the given directory
     * @param dirname The name of the directory to read from
     * @return True, if the keys could be read, false otherwise
     */
    bool readKeyPairFrom(const QString &dirname, CryptoPP::RSA::PrivateKey &privateKey, CryptoPP::RSA::PublicKey &publicKey);

    /**
     * @brief generateRSAKeyPair Generates a new RSA keypair for the user and stores it
     * in the directory with the given name
     * @param dirname The name of the directory to store the keypair
     * @return True on success, false otherwise
     */
    bool generateRSAKeyPair(const QString &dirname);

    /**
     * @brief getRSAPublicKey Returns the users RSA public key
     * @return The users public key
     */
    CryptoPP::RSA::PublicKey* getRSAPublicKey();

    /**
     * @brief getRSAPrivateKey Returns the users RSA private key
     * @return The private key file
     */
    CryptoPP::RSA::PrivateKey* getRSAPrivateKey();

    /**
     * @brief fromHex decodes a hex string into a string containing the bytes
     * @param str The hex string
     * @param output The output where to put the result to
     */
    void fromHex(const std::string &str, std::string &output) const;

    /**
     * @brief toHex encodes arbitrary byte data into a hex encoded string
     * @param str The byte source to encode
     * @param output The output where to put the result to
     */
    void toHex(const std::string &str, std::string &output) const;

    /**
     * @brief toBase64 Encodes a string to base64 encoding
     * @param str The string that should be encoded
     * @param output The string to which the base64 string is written to
     */
    void toBase64(const std::string &str, std::string &output) const;

    /**
     * @brief fromBase64 Decodes a base64 string to binary data
     * @param str The string to read from
     * @param output The string to which the data will be written to
     */
    void fromBase64(const std::string &str, std::string &output) const;

    /**
     * @brief aesEncrypt Encrypts data using the provided key and iv
     * @param key The AES key to be used for encryption
     * @param iv The AES IV for the CBC mode to be used
     * @param plaintext The plaintext to be encrypted
     * @return The ciphertext
     */
    QString aesEncrypt(const byte* key, const byte* iv, QString plaintext) const;

    /**
     * @brief aesDecrypt Decrypts the provided ciphertext using the key and iv
     * @param key The AES key used for decryption
     * @param iv The IV used for CBC mode
     * @param ciphertext The ciphertext to be decrypted
     * @return The plaintext
     */
    QString aesDecrypt(const byte* key, const byte* iv, QString ciphertext) const;

    /**
     * @brief rsaEncrypt Encrypts the provided plaintext using the public RSA key
     * @param plaintext The plaintext to be encrypted
     * @param length The length of the plaintext
     * @param key The public key used for encryption
     * @return The ciphertext
     */
    QString rsaEncrypt(const byte* plaintext, int length, CryptoPP::RSA::PublicKey* key) const;

    /**
     * @brief rsaDecrypt Decrypts the provided plaintext using the private RSA key
     * @param ciphertext Ptr to the ciphertext to be decrypted
     * @param length The length of the ciphertext
     * @param key The private key to be used for decryption
     * @return The plaintext
     */
    QString rsaDecrypt(const byte* ciphertext, int length, CryptoPP::RSA::PrivateKey* key) const;

    /**
     * @brief generateRandomAESKey Generates a random AES key and stores it in the provided byte[]
     * @param ptr Points to the byte array that holds the key
     */
    void generateRandomAESKey(byte *ptr) const;

    /**
     * @brief generateRandomAES_IV Generates a random AES IV for CBC mode
     * @param ptr Points to the byte array that holds the iv
     */
    void generateRandomAES_IV(byte *ptr) const;

    /**
     * @brief publicKeyAsHex Exports the public key this class holds as hex string
     * @param target Reference where to write the hex string
     */
    void publicKeyAsHex(QString &target, CryptoPP::RSA::PublicKey &key) const;

    /**
     * @brief readPublicKeyFromHex Reads a hex encoded string and tries to generate a RSA key from it
     * @param hex The hexadecimal encoded public key
     * @param pk Reference to the public key that will be loaded
     * @return True if the key is valid, false otherwise
     */
    bool loadPublicKey(std::string &hex, CryptoPP::RSA::PublicKey &pk) const;
};
}
#endif // CRYTPUTILS_H
