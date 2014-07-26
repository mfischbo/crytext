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
#define SMTP_PORT       "SMTP/Port"
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
    short getSMTPPort();
    SMTPAuthMethod getSMTPAuthMethod();

    void setDataDirectory(const QString directory);
    void setRecentDirectory(const QString directory);
    void setStickerEMail(const QString email);
    void setStickerFirstName(const QString firstName);
    void setStickerLastName(const QString lastName);
    void setSMTPHost(const QString host);
    void setSMTPUsername(const QString username);
    void setSMTPPassword(const QString password);
    void setSMTPPort(const short port);
    void setSMTPAuthMethod(const SMTPAuthMethod method);
};
}
#endif // SETTINGS_H
