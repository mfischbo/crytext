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
#include "service/smtpservice.h"
#include "model/crytextfile.h"
#include "model/sticker.h"
#include "util/cryptutils.h"

using namespace crytext;

CryTextService::CryTextService()
{
    this->settings = new Settings();
    qDebug() << "Settings directory is " << this->settings->getDataDirectory();

    utils = new CryptUtils();
    stickers = QList<Sticker*>();
    recipients = QList<Sticker*>();

    // check if a data directory is set and a rsa key pair exists
    if (this->settings->getDataDirectory().length() > 0) {

        bool containsKeys = this->utils->readKeyPairFrom(settings->getDataDirectory(), this->privateKey, this->publicKey);
        if (!containsKeys) {
            this->utils->generateRSAKeyPair(settings->getDataDirectory());
            this->utils->readKeyPairFrom(settings->getDataDirectory(), this->privateKey, this->publicKey);
        }

        // read all stickers
        QStringList filter(QString("*.cts"));

        QStringList stFiles;

        QDir d(settings->getDataDirectory());
        stFiles = d.entryList(filter);
        QStringListIterator it(stFiles);
        while (it.hasNext()) {
            QString fName = settings->getDataDirectory();//
            if (!fName.endsWith("/"))
                fName.append("/");
            fName.append(it.next());
            qDebug() << "Trying to import sticker " << fName;
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

QTextDocument*
CryTextService::open(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) return 0;

    // open regular txt file
    if (!filename.endsWith(".cry")) {
        QTextStream inStream(&file);
        QString plaintext = inStream.readAll();
        QTextDocument *retval = new QTextDocument();
        retval->setPlainText(plaintext);

        settings->setRecentDirectory(filename);
        return retval;
    }

    // open encrypted file
    if (filename.endsWith(".cry")) {
        QTextStream inStream(&file);
        CryTextFile ctf(inStream, this->utils,
                        settings->getStickerEMail());

        ctf.decrypt(&privateKey);
        QString plaintext = ctf.getPlainText();

        QTextDocument *retval = new QTextDocument();
        retval->setPlainText(plaintext);

        settings->setRecentDirectory(filename);
        return retval;
    }
    return 0;
}

void
CryTextService::saveAs(const QString &filename, const QTextDocument* document) {

    // always create an entry for the author!
    CryTextFile ctf(document, this->utils);
    ctf.encrypt(&publicKey);

    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file for writing";
    }
    QTextStream ts(&f);
    ts << "====BOUNDARY:";
    ts << settings->getStickerEMail() << "\n";
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
    settings->setRecentDirectory(filename);
}

void
CryTextService::sendAsEMail(QString subject, QString message, const QTextDocument* doc) {

    // encrypt the file
    QListIterator<Sticker*> it = QListIterator<Sticker*>(this->recipients);
    SMTPService mailer(this);

    while (it.hasNext()) {
        Sticker *s = it.next();
        qDebug() << "Sending mail message to " << *s->getEMail();

        CryTextFile ctf(doc, this->utils);
        ctf.encrypt(s->getPublicKey());
        mailer.sendCrytextFile(s, &ctf, subject, message);
    }
}

void
CryTextService::exportPrivateSticker(QString &filename) {

    QFile f(filename);
    if (!f.open(QIODevice::Text | QIODevice::WriteOnly)) {
        qDebug() << "Unable to open device at " << filename;
    }
    QTextStream outStream(&f);
    this->exportPrivateSticker(outStream);

    f.flush();
    f.close();
    settings->setRecentDirectory(filename);
}

void
CryTextService::exportPrivateSticker(QTextStream &stream) {

    stream << settings->getStickerEMail() << "\n";
    stream << settings->getStickerFirstName() << "\n";
    stream << settings->getStickerLastName() << "\n";

    QString key;
    this->utils->publicKeyAsHex(key, this->publicKey);
    stream << key;
    stream.flush();
}

Sticker*
CryTextService::importSticker(QString &filename) {

    // create a new sticker and add it to the list of stickers
    Sticker *sticker = new Sticker(filename, this->utils);
    if (sticker != 0) {

        // copy the sticker file to the settings directory
        QString toFile = settings->getDataDirectory();
        if (!toFile.endsWith("/"))
            toFile.append("/");
        toFile.append(sticker->getEMail());
        toFile.append(".cts");

        QFile::copy(filename, toFile);
        stickers << sticker;
        return sticker;
    }
    settings->setRecentDirectory(filename);
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
            QString dir = QString(settings->getDataDirectory());
            QString fName = QString("/").append(s->getEMail()->append(".cts"));

            QFile f(dir.append(fName));
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

QListIterator<Sticker*>*
CryTextService::getRecipients() {
    return new QListIterator<Sticker*>(this->recipients);
}

QList<Sticker*>*
CryTextService::getAvailableStickers() {
    return &stickers;
}


Settings* CryTextService::getSettings() {
    return settings;
}
