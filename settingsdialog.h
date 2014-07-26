#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QString>
#include "crytextservice.h"
#include "ui_settings.h"

class SettingsDialog : public QDialog, public Ui::Dialog
{
    Q_OBJECT

private:
    crytext::CryTextService *service;

public:
    SettingsDialog(crytext::CryTextService *service, QWidget *parent = 0);
private slots:
    void on_pb_Browse_clicked();
    void on_buttonBox_accepted();
    void on_btn_ExSticker_clicked();
    void on_cb_SmtpAuthMethod_currentIndexChanged(const QString &arg1);
};

#endif // SETTINGSDIALOG_H
