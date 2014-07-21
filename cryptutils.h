#ifndef CRYTPUTILS_H
#define CRYTPUTILS_H

#include <QString>
#include <crypto++/asn.h>
#include <crypto++/rsa.h>


class CryptUtils
{
private :
    CryptoPP::RSA::PublicKey    publicKey;
    CryptoPP::RSA::PrivateKey   privateKey;

public:
    CryptUtils();

    /**
     * @brief readKeyPairFrom Reads the users RSA keypair from the given directory
     * @param dirname The name of the directory to read from
     * @return True, if the keys could be read, false otherwise
     */
    bool readKeyPairFrom(QString &dirname);

    /**
     * @brief generateRSAKeyPair Generates a new RSA keypair for the user and stores it
     * in the directory with the given name
     * @param dirname The name of the directory to store the keypair
     * @return True on success, false otherwise
     */
    bool generateRSAKeyPair(QString &dirname);

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

    QString aesEncrypt(const byte* key, const byte* iv, QString plaintext) const;
    QString aesDecrypt(const byte* key, const byte* iv, QString ciphertext) const;

    void publicKeyAsHex(QString &target) const;
    bool readPublicKeyFromHex(QString &base64, CryptoPP::RSA::PublicKey &pk) const;
};

#endif // CRYTPUTILS_H
