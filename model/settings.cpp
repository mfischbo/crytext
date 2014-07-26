#include "settings.h"

#include <QSettings>

crytext::Settings::Settings()
{
    this->m_s = new QSettings("de.mfischbo", "crytext");
    this->dataDirectory = m_s->value(DATA_DIR).toString();
    this->recentDir = m_s->value(RECENT_DIR, "/").toString();

    this->stickerEMail = m_s->value(STICKER_EMAIL).toString();
    this->stickerFirstName = m_s->value(STICKER_FIRST_N).toString();
    this->stickerLastName = m_s->value(STICKER_LAST_N).toString();

    this->smtpHost = m_s->value(SMTP_HOST).toString();
    this->smtpUsername = m_s->value(SMTP_USER).toString();
    this->smtpPassword = m_s->value(SMTP_PASSWORD).toString();
    this->smtpPort = m_s->value(CRY_SMTP_PORT).toInt();

    QString authMeth = m_s->value(SMTP_AUTH_METH, QVariant("-1")).toString();
    if (authMeth == "NO_AUTH") this->authMethod = NO_AUTH;
    if (authMeth == "PASSWORD") this->authMethod = PASSWORD;
    if (authMeth == "PLAINTEXT") this->authMethod = PLAINTEXT;
    if (authMeth == "CRAM_MD5") this->authMethod = CRAM_MD5;
    if (authMeth == "CRAM_SHA1") this->authMethod = CRAM_SHA1;
}

crytext::Settings::~Settings() {
    delete m_s;
}

const QString
crytext::Settings::getDataDirectory() {
    return this->dataDirectory;
}

void
crytext::Settings::setDataDirectory(const QString directory) {
    this->dataDirectory = directory;
    m_s->setValue(DATA_DIR, QVariant(directory));
}


const QString
crytext::Settings::getStickerEMail() {
    return this->stickerEMail;
}

void
crytext::Settings::setStickerEMail(const QString email) {
    this->stickerEMail = email;
    m_s->setValue(STICKER_EMAIL, QVariant(email));
}

const QString
crytext::Settings::getStickerFirstName() {
    return this->stickerFirstName;
}

void
crytext::Settings::setStickerFirstName(const QString firstName) {
    this->stickerFirstName = firstName;
    m_s->setValue(STICKER_FIRST_N, QVariant(firstName));
}

const QString
crytext::Settings::getStickerLastName() {
    return this->stickerLastName;
}

void
crytext::Settings::setStickerLastName(const QString lastName) {
    this->stickerLastName = lastName;
    m_s->setValue(STICKER_LAST_N, QVariant(lastName));
}

const QString
crytext::Settings::getSMTPHost() {
    return this->smtpHost;
}

void
crytext::Settings::setSMTPHost(const QString host) {
    this->smtpHost = host;
    m_s->setValue(SMTP_HOST, QVariant(host));
}

const QString
crytext::Settings::getSMTPPUsername() {
    return this->smtpUsername;
}

void
crytext::Settings::setSMTPUsername(const QString username) {
    this->smtpUsername = username;
    m_s->setValue(SMTP_USER, QVariant(username));
}

const QString
crytext::Settings::getSMTPPassword() {
    return this->smtpPassword;
}

void
crytext::Settings::setSMTPPassword(const QString password) {
    this->smtpPassword = password;
    m_s->setValue(SMTP_PASSWORD, QVariant(password));
}

int crytext::Settings::getSMTPPort() {
    return this->smtpPort;
}

void
crytext::Settings::setSMTPPort(const int port) {
    this->smtpPort = port;
    m_s->setValue(CRY_SMTP_PORT, QVariant(port));
}

crytext::SMTPAuthMethod
crytext::Settings::getSMTPAuthMethod() {
    return this->authMethod;
}

void
crytext::Settings::setSMTPAuthMethod(const SMTPAuthMethod method) {
    switch (method) {
    case crytext::NO_AUTH :
        m_s->setValue(SMTP_AUTH_METH, QVariant("NO_AUTH"));
        break;
    case crytext::PASSWORD :
        m_s->setValue(SMTP_AUTH_METH, QVariant("PASSWORD"));
        break;
    case crytext::CRAM_MD5 :
        m_s->setValue(SMTP_AUTH_METH, QVariant("CRAM_MD5"));
        break;
    case crytext::CRAM_SHA1 :
        m_s->setValue(SMTP_AUTH_METH, QVariant("CRAM_SHA1"));
        break;
    case crytext::PLAINTEXT :
        m_s->setValue(SMTP_AUTH_METH, QVariant("PLAINTEXT"));
    }
    this->authMethod = method;
}

const QString
crytext::Settings::getRecentDirectory() {
    return this->recentDir;
}

void
crytext::Settings::setRecentDirectory(const QString directory) {
    this->recentDir = directory;
    m_s->setValue(RECENT_DIR, QVariant(directory));
}
