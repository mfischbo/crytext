#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextDocument>
#include <QDataStream>
#include <QStringList>
#include <QStringListIterator>
#include <QString>
#include <QDebug>

#include "crytextservice.h"
#include "crytextfile.h"
#include "cryptutils.h"
#include "sticker.h"


CryTextService::CryTextService()
{
    this->settings = new QSettings(QString("de.mfischbo"), QString("crytext"));
    qDebug() << "Settings directory is " << this->settings->fileName();

    utils = new CryptUtils();
    stickers = QList<Sticker*>();
    recipients = QList<Sticker*>();

    // check if a data directory is set and a rsa key pair exists
    if (this->settings->contains("Settings/DataDir")) {
        QString datadir = this->settings->value("Settings/DataDir").toString();

        bool containsKeys = this->utils->readKeyPairFrom(datadir);
        if (!containsKeys) {
            this->utils->generateRSAKeyPair(datadir);
            this->utils->readKeyPairFrom(datadir);
        }

        // read all stickers
        QStringList filter(QString("*.cts"));

        QStringList stFiles;

        QDir d(datadir);
        stFiles = d.entryList(filter);
        QStringListIterator it(stFiles);
        while (it.hasNext()) {
            QString fName = datadir;//
            if (!fName.endsWith("/"))
                fName.append("/");
            fName.append(it.next());
            Sticker *s = new Sticker(fName, this->utils);
            stickers << s;
        }
    }
}

CryTextService::~CryTextService() {
    delete settings;
    delete utils;
    for (int i=0; i < stickers.length(); i++) {
        Sticker *s = stickers.at(i);
        s->~Sticker();
    }
}

QTextDocument* CryTextService::open(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) return 0;

    // open regular txt file
    if (!filename.endsWith(".cry")) {
        QTextStream inStream(&file);
        QString plaintext = inStream.readAll();
        QTextDocument *retval = new QTextDocument();
        retval->setPlainText(plaintext);

        this->setRecentDir(filename);
        return retval;
    }

    // open encrypted file
    if (filename.endsWith(".cry")) {
        QTextStream inStream(&file);
        CryTextFile ctf(inStream, this->utils, settings->value("Sticker/EMail", "john.doe@example.com").toString());

        ctf.decrypt(this->utils->getRSAPrivateKey());
        QString plaintext = ctf.getPlainText();

        QTextDocument *retval = new QTextDocument();
        retval->setPlainText(plaintext);

        this->setRecentDir(filename);
        return retval;
    }
    return 0;
}

void
CryTextService::saveAs(const QString &filename, const QTextDocument* document) {

    // always create an entry for the author!
    CryTextFile ctf(document, this->utils);
    ctf.encrypt(this->utils->getRSAPublicKey());

    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file for writing";
    }
    QTextStream ts(&f);
    ts << "====BOUNDARY:";
    ts << this->getSettings()->value("Sticker/EMail", "john.doe@example.com").toString() << "\n";
    ctf.saveTo(ts);

    // create entries for each other recipient
    QListIterator<Sticker*> it = QListIterator<Sticker*>(this->recipients);
    while (it.hasNext()) {
        Sticker *s = it.next();
        CryTextFile ctf(document, this->utils);
        ctf.encrypt(s->getPublicKey());

        ts << "====BOUNDARY:";
        ts << *s->getEMail() << "\n";
        ctf.saveTo(ts);
    }

    f.flush();
    f.close();
    this->setRecentDir(filename);
}

void
CryTextService::exportPrivateSicker(QString &filename) {

    QFile f(filename);
    if (!f.open(QIODevice::Text | QIODevice::WriteOnly)) {
        qDebug() << "Unable to open device at " << filename;
    }

    QTextStream outStream(&f);
    outStream << this->settings->value("Sticker/EMail").toString() << "\n";
    outStream << this->settings->value("Sticker/FirstName").toString() << "\n";
    outStream << this->settings->value("Sticker/LastName").toString() << "\n";

    QString key;
    this->utils->publicKeyAsHex(key);
    outStream << key;
    outStream.flush();
    f.flush();
    f.close();
    this->setRecentDir(filename);
}

Sticker *
CryTextService::importSticker(QString &filename) {

    // create a new sticker and add it to the list of stickers
    Sticker *sticker = new Sticker(filename, this->utils);
    if (sticker != 0) {

        // copy the sticker file to the settings directory
        QString toFile = QString(this->settings->value("Settings/DataDir").toString());
        if (!toFile.endsWith("/"))
            toFile.append("/");
        toFile.append(sticker->getEMail());
        toFile.append(".cts");

        QFile::copy(filename, toFile);
        stickers << sticker;
        return sticker;
    }
    this->setRecentDir(filename);
    return 0;
}

bool
CryTextService::removeStickerByEmail(QString email) {

    // find the sticker
    QListIterator<Sticker*> it = QListIterator<Sticker*>(this->stickers);
    while (it.hasNext()) {
        Sticker *s = it.next();
        if (s->getEMail() == email) {
            this->stickers.removeOne(s);

            // delete the sticker file
            QString fName = QString("/").append(s->getEMail()->append(".cts"));
            QFile f(this->settings->value("Settings/DataDir", "").toString().append(fName));
            if (f.exists()) {
                f.remove();
                return true;
            }
        }
    }
    return false;
}

Sticker*
CryTextService::getStickerByEmail(QString email) {
    QListIterator<Sticker*> it = QListIterator<Sticker*>(this->stickers);
    while (it.hasNext()) {
        Sticker *s = it.next();
        if (s->getEMail() == email) {
            return s;
        }
    }
    return 0;
}

void
CryTextService::addRecipient(Sticker *s) {
    this->recipients.append(s);
}

QList<Sticker*>*
CryTextService::getAvailableStickers() {
    return &stickers;
}


QSettings* CryTextService::getSettings() {
    return settings;
}

QString
CryTextService::getRecentDirectory() {
    return this->settings->value("UI/RecentDir", QDir::homePath()).toString();
}

void
CryTextService::setRecentDir(QString filename) {
    filename.remove(filename.lastIndexOf("/"), filename.length()-1);
    settings->setValue("UI/RecentDir", filename);
}
