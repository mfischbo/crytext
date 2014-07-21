#include "settingsdialog.h"
#include "crytextservice.h"
#include <QFileDialog>
#include <QDebug>

SettingsDialog::SettingsDialog(CryTextService *service, QWidget *parent)
{
    setupUi(this);
    this->service = service;
    if (service->getSettings()->contains("Settings/DataDir")) {
        this->dataDirectory = service->getSettings()->value("Settings/DataDir").toString();
        this->le_SettingsPath->setText(this->dataDirectory);
    } else
        this->dataDirectory = "";

    if (service->getSettings()->contains("Sticker/EMail")) {
        this->le_EMail->setText(service->getSettings()->value("Sticker/EMail").toString());
    }

    if (service->getSettings()->contains("Sticker/FirstName")) {
        this->le_FirstName->setText(service->getSettings()->value("Sticker/FirstName").toString());
    }

    if (service->getSettings()->contains("Sticker/LastName")) {
        this->le_LastName->setText(service->getSettings()->value("Sticker/LastName").toString());
    }
}

void SettingsDialog::on_pb_Browse_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Choose your directory"), service->getRecentDirectory());

    if (dirname.length() > 0)
        this->dataDirectory = dirname;
    qDebug() << "Settings dir is " << this->dataDirectory;
    this->le_SettingsPath->setText(this->dataDirectory);
}

void SettingsDialog::on_buttonBox_accepted()
{
    if (this->dataDirectory.length() > 0)
        this->service->getSettings()->setValue("Settings/DataDir", dataDirectory);
    this->service->getSettings()->setValue("Sticker/EMail", this->le_EMail->text());
    this->service->getSettings()->setValue("Sticker/LastName", this->le_LastName->text());
    this->service->getSettings()->setValue("Sticker/FirstName", this->le_FirstName->text());
}

void SettingsDialog::on_btn_ExSticker_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save sticker as"), service->getRecentDirectory(), "Crytext Sticker (*.cts)");
    if (!filename.endsWith(".cts"))
        filename.append(".cts");
    service->exportPrivateSicker(filename);
}
