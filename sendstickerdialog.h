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

#ifndef SENDSTICKERDIALOG_H
#define SENDSTICKERDIALOG_H

#include <QDialog>
#include "service/crytextservice.h"
#include "service/smtpservice.h"

namespace Ui {
class SendStickerDialog;
}

class SendStickerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendStickerDialog(crytext::CryTextService *service, QWidget *parent = 0);
    ~SendStickerDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SendStickerDialog *ui;
    crytext::CryTextService *service;
};

#endif // SENDSTICKERDIALOG_H
