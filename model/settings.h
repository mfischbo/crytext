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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>

#define RECENT_DIR      "UI/RecentDir"

#define DATA_DIR        "Settings/DataDir"
#define EMAIL           "Settings/EMail"
#define STICKER_EMAIL   "Sticker/EMail"
#define STICKER_FIRST_N "Sticker/FirstName"
#define STICKER_LAST_N  "Sticker/LastName"
#define SMTP_USER       "SMTP/Username"
#define SMTP_PASSWORD   "SMTP/Password"
#define SMTP_HOST       "SMTP/Host"
#define CRY_SMTP_PORT       "SMTP/Port"
#define SMTP_AUTH_METH  "SMTP/AuthMethod"


namespace crytext {
    enum SMTPAuthMethod {
        NO_AUTH = 0,
        PASSWORD = 1,
        CRAM_MD5 = 2,
        CRAM_SHA1 = 3,
        PLAINTEXT = 4
    };

class Settings
{
private:
    QSettings *m_s;
    QString dataDirectory;
    QString recentDir;
    QString stickerEMail;
    QString stickerFirstName;
    QString stickerLastName;
    QString smtpHost;
    QString smtpUsername;
    QString smtpPassword;
    int	    smtpPort;
    SMTPAuthMethod authMethod;

    public:
    Settings();
    ~Settings();


    const QString getDataDirectory();
    const QString getRecentDirectory();
    const QString getStickerEMail();
    const QString getStickerFirstName();
    const QString getStickerLastName();
    const QString getSMTPHost();
    const QString getSMTPPUsername();
    const QString getSMTPPassword();
    int getSMTPPort();
    SMTPAuthMethod getSMTPAuthMethod();

    void setDataDirectory(const QString directory);
    void setRecentDirectory(const QString directory);
    void setStickerEMail(const QString email);
    void setStickerFirstName(const QString firstName);
    void setStickerLastName(const QString lastName);
    void setSMTPHost(const QString host);
    void setSMTPUsername(const QString username);
    void setSMTPPassword(const QString password);
    void setSMTPPort(const int port);
    void setSMTPAuthMethod(const SMTPAuthMethod method);
};
}
#endif // SETTINGS_H
