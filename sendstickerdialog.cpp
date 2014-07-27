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

#include "sendstickerdialog.h"
#include "ui_sendstickerdialog.h"

SendStickerDialog::SendStickerDialog(crytext::CryTextService *service, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendStickerDialog)
{
    ui->setupUi(this);
    this->service = service;
}

SendStickerDialog::~SendStickerDialog()
{
    delete ui;
}

void SendStickerDialog::on_buttonBox_accepted()
{
    QString recipient = ui->le_Recipient->text();
    QString subject   = ui->le_Subject->text();
    QString message = ui->te_Message->document()->toPlainText();

    crytext::SMTPService mailer(service);
    mailer.sendSticker(recipient, subject, message);
}
