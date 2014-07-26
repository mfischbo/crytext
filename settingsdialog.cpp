#include "settingsdialog.h"
#include "crytextservice.h"
#include "model/settings.h"
#include <QFileDialog>
#include <QDebug>

SettingsDialog::SettingsDialog(crytext::CryTextService *service, QWidget *parent)
{
    setupUi(this);
    this->service = service;
    crytext::Settings* s = service->getSettings();

    this->le_SettingsPath->setText(s->getDataDirectory());
    this->le_EMail->setText(s->getStickerEMail());
    this->le_FirstName->setText(s->getStickerFirstName());
    this->le_LastName->setText(s->getStickerLastName());

    this->le_SmtpHost->setText(s->getSMTPHost());
    this->le_SmtpUser->setText(s->getSMTPPUsername());
    this->le_SmtpPass->setText(s->getSMTPPassword());
    this->sb_SmtpPort->setValue(s->getSMTPPort());

    crytext::SMTPAuthMethod m = s->getSMTPAuthMethod();
    if (m == crytext::NO_AUTH)
        this->cb_SmtpAuthMethod->setCurrentIndex(0);
    if (m == crytext::PASSWORD)
        this->cb_SmtpAuthMethod->setCurrentIndex(1);
    if (m == crytext::PLAINTEXT)
        this->cb_SmtpAuthMethod->setCurrentIndex(2);
    if (m == crytext::CRAM_MD5)
        this->cb_SmtpAuthMethod->setCurrentIndex(3);
    if (m == crytext::CRAM_SHA1)
        this->cb_SmtpAuthMethod->setCurrentIndex(4);


}

void SettingsDialog::on_pb_Browse_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(
        this, tr("Choose your directory"), service->getSettings()->getRecentDirectory());

    if (dirname.length() > 0) {
        this->service->getSettings()->setDataDirectory(dirname);
        this->le_SettingsPath->setText(dirname);
    }
}

void SettingsDialog::on_buttonBox_accepted()
{
    crytext::Settings* s = service->getSettings();

    s->setDataDirectory(this->le_SettingsPath->text());
    s->setStickerEMail(this->le_EMail->text());
    s->setStickerFirstName(this->le_FirstName->text());
    s->setStickerLastName(this->le_LastName->text());
    s->setSMTPHost(this->le_SmtpHost->text());
    s->setSMTPUsername(this->le_SmtpUser->text());
    s->setSMTPPassword(this->le_SmtpPass->text());
    s->setSMTPPort(this->sb_SmtpPort->value());


}

void SettingsDialog::on_btn_ExSticker_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save sticker as"),
          service->getSettings()->getRecentDirectory(), "Crytext Sticker (*.cts)");

    if (!filename.endsWith(".cts"))
        filename.append(".cts");
    service->exportPrivateSicker(filename);
}

void SettingsDialog::on_cb_SmtpAuthMethod_currentIndexChanged(const QString &m)
{
    crytext::Settings* s = service->getSettings();
    if (m == "None")
        s->setSMTPAuthMethod(crytext::NO_AUTH);
    if (m == "Password")
        s->setSMTPAuthMethod(crytext::PASSWORD);
    if (m == "Plaintext")
        s->setSMTPAuthMethod(crytext::PLAINTEXT);
    if (m == "CRAM MD5")
        s->setSMTPAuthMethod(crytext::CRAM_MD5);
    if (m == "CRAM SHA1")
        s->setSMTPAuthMethod(crytext::CRAM_SHA1);
}
