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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QString>
#include "service/crytextservice.h"
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
