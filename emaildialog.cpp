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

#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include "emaildialog.h"
#include "ui_emaildialog.h"
#include "model/sticker.h"

using crytext::CryTextService;
using crytext::Sticker;

EmailDialog::EmailDialog(CryTextService* service, const QTextDocument *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailDialog)
{
    ui->setupUi(this);
    this->service = service;
    this->parent = parent;
    this->document = doc;
    this->mapper = new QSignalMapper();

    QListIterator<Sticker*>* it = service->getRecipients();
    for (int i=0; it->hasNext(); i++) {
        Sticker *s = it->next();
        ui->rcpt_Layout->addLayout(createRecipientRow(parent, s, i, it->hasNext()));
    }
    connect(this->mapper, SIGNAL(mapped(int)), this, SLOT(on_btn_RemoveRecipient_clicked(int)));
}

QHBoxLayout*
EmailDialog::createRecipientRow(QWidget *parent, Sticker *s, int i, bool hasNext) {
    QHBoxLayout *l = new QHBoxLayout();
    l->addWidget(this->createRecipientTypeComboBox(parent));

    QLineEdit* recp = new QLineEdit(parent);
    if (s != 0)
        recp->setText(*s->getEMail());
    l->addWidget(recp);

    QPushButton* btn = new QPushButton(parent);
    btn->setIcon(QIcon(":/images/images/delete_sign-20.png"));
    l->addWidget(btn);
    connect(btn, SIGNAL(clicked()), this->mapper, SLOT(map()));
    mapper->setMapping(btn, i);

    if (!hasNext) {
        this->addButton = new QPushButton(parent);
        this->addButton->setIcon(QIcon(":/images/images/plus-32.png"));
        l->addWidget(this->addButton);
        connect(this->addButton, SIGNAL(clicked()), this, SLOT(on_btn_AddRecipient_clicked()));
    }
    return l;
}


EmailDialog::~EmailDialog()
{
    delete ui;
}


void
EmailDialog::on_btn_AddRecipient_clicked()
{
    // delete the add button and add a new one (done in createRecipientRow)
    delete this->addButton;
    ui->rcpt_Layout->addLayout(createRecipientRow(parent, 0, ui->rcpt_Layout->children().length(), false));
}

void
EmailDialog::on_btn_RemoveRecipient_clicked(int i) {
    qDebug() << "Remove recipient clicked on row " << i;

    QHBoxLayout *layout = (QHBoxLayout*) ui->rcpt_Layout->itemAt(i);
    QLayoutItem *item;
    while ((item = layout->itemAt(0)) != 0) {

        QWidget *w = item->widget();

        // according to docs caller is responsible for setting
        // the new geometry on the widget
        layout->removeWidget(w);
        w->setVisible(false);
    }
    delete layout;

    // append the add button to the row at i-1
    if (i > 0) {
        QHBoxLayout* l = (QHBoxLayout*) ui->rcpt_Layout->itemAt(i-1);
        l->addWidget(this->addButton);
        this->addButton->setVisible(true);
    }
}


void
EmailDialog::on_buttonBox_accepted()
{
    QString subject = ui->le_Subject->text();
    QString message = ui->pte_Message->document()->toPlainText();
    service->sendAsEMail(subject, message, this->document);
}

QWidget*
EmailDialog::createRecipientTypeComboBox(QWidget *parent) {
    QComboBox* b = new QComboBox(parent);
    b->addItem(tr("To:"));
    b->addItem(tr("CC:"));
    b->addItem(tr("BCC:"));
    return b;
}
