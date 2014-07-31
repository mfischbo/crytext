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

#ifndef EMAILDIALOG_H
#define EMAILDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QList>
#include <QGridLayout>
#include <QSignalMapper>
#include <QTextDocument>
#include <service/crytextservice.h>

using crytext::CryTextService;

namespace Ui {
class EmailDialog;
}

class EmailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmailDialog(CryTextService *service, const QTextDocument *doc, QWidget *parent = 0);
    ~EmailDialog();

private slots:
    void on_btn_AddRecipient_clicked();
    void on_btn_RemoveRecipient_clicked(int i);

    void on_buttonBox_accepted();

private:
    QWidget* parent;
    Ui::EmailDialog *ui;
    CryTextService* service;
    const QTextDocument *document;

    QHBoxLayout *createRecipientRow(QWidget* parent, crytext::Sticker *s, int i, bool hasNext);
    QWidget* createRecipientTypeComboBox(QWidget *parent);

    QPushButton *addButton;
    QSignalMapper *mapper;
};

#endif // EMAILDIALOG_H
