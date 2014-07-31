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

#ifndef CRYTEXTSERVICE_H
#define CRYTEXTSERVICE_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QTextDocument>
#include <QTextStream>

#include "util/cryptutils.h"
#include "model/settings.h"
#include "model/sticker.h"

namespace crytext {
class CryTextService
{
private:

    Settings*            settings;
    CryptUtils*          utils;
    QList<Sticker*>      stickers;
    QList<Sticker*>      recipients;

    CryptoPP::RSA::PrivateKey   privateKey;
    CryptoPP::RSA::PublicKey    publicKey;

    /**
     * @brief setRecentDir Internally stores the directory currently used by the user
     * @param filename The filename the user opened
     */
    void setRecentDir(QString filename);

public:
    CryTextService();
    ~CryTextService();

    /**
     * @brief open Opens an unencrypted file
     * @param filename The filename of the file to be opened
     * @return A QTextDocument used for the editor
     */
    QTextDocument* open(const QString &filename);

    /**
     * @brief saveAs Saves the documents contents in the given file
     * @param filename The name of the file where to save contents
     * @param document The document to be saved
     */
    void saveAs(const QString &filename, const QTextDocument* document);

    /**
     * @brief sendAsEMail Sends the current crytext file as attachment to all recipients
     */
    void sendAsEMail(QString subject, QString message, const QTextDocument *doc);

    /**
     * @brief exportPrivateSicker exports the users sticker to a file with the given name
     * @param filename The name where to save the sticker
     */
    void exportPrivateSticker(QString &filename);

    /**
     * @brief exportPrivateSticker writes the sticker to the given stream
     * @param stream The stream to write the sticker to
     */
    void exportPrivateSticker(QTextStream &stream);

    /**
     * @brief importSticker Imports a crytext sticker from the given location
     * @param filename The name of the file to be imported as sticker
     */
    Sticker *importSticker(QString &filename);

    /**
     * @brief getAvailableStickers Returns a list of stickers that had been imported by the user
     * @return The list of stickers
     */
    QList<Sticker*>* getAvailableStickers();

    /**
     * @brief removeStickerByEmail removes a sticker from the internal list
     * @param email The email address contained in the sticker
     */
    bool removeStickerByEmail(QString email);

    /**
     * @brief getStickerByEmail Returns the sticker with the given email address
     * @param email The email address of the sticker
     * @return The sticker or null if no such sticker exists
     */
    Sticker* getStickerByEmail(QString email);

    /**
     * @brief addRecipient Adds a recipient to the list of encrypted blocks
     * @param s The recipients sticker
     */
    void addRecipient(Sticker *s);

    /**
     * @brief removeRecipient Removes the recipient for the given sticker
     * @param s The sticker to be removed from the recipients list
     */
    void removeRecipient(Sticker *s);

    QListIterator<Sticker*>* getRecipients();

    /**
     * @brief getSettings Returns a pointer on the QSettings for direct access
     * @return The QSettings
     */
    Settings *getSettings();
};
}
#endif // CRYTEXTSERVICE_H
