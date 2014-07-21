#ifndef CRYTEXTSERVICE_H
#define CRYTEXTSERVICE_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QTextDocument>

#include "cryptutils.h"
#include "sticker.h"

class CryTextService
{
private:
    QSettings*                    settings;
    CryptUtils*                   utils;
    QList<Sticker*>               stickers;
    QList<Sticker*>               recipients;

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
     * @brief exportPrivateSicker exports the users sticker to a file with the given name
     * @param filename The name where to save the sticker
     */
    void exportPrivateSicker(QString &filename);

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
     * @brief getSettings Returns a pointer on the QSettings for direct access
     * @return The QSettings
     */
    QSettings *getSettings();

    /**
     * @brief getRecentDirectory Returns a QString with the recent directory the user used
     * @return The users recent directory
     */
    QString getRecentDirectory();
};

#endif // CRYTEXTSERVICE_H
