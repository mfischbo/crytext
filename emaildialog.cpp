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

#include "emaildialog.h"
#include "ui_emaildialog.h"

using crytext::CryTextService;

EmailDialog::EmailDialog(CryTextService* service, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailDialog)
{
    ui->setupUi(this);
    this->service = service;
    this->parent = parent;
}

EmailDialog::~EmailDialog()
{
    delete ui;
}


void EmailDialog::on_btn_AddRecipient_clicked()
{

}

void EmailDialog::on_buttonBox_accepted()
{
    QString subject = ui->le_Subject->text();
    QString message = ui->pte_Message->document()->toPlainText();

}
