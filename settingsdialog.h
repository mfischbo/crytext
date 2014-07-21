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
    CryTextService *service;
    QString        dataDirectory;

public:
    SettingsDialog(CryTextService *service, QWidget *parent = 0);
private slots:
    void on_pb_Browse_clicked();
    void on_buttonBox_accepted();
    void on_btn_ExSticker_clicked();
};

#endif // SETTINGSDIALOG_H
