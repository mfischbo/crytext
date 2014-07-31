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

#include "stickerlabel.h"
#include "ui_stickerlabel.h"

using crytext::Sticker;

StickerLabel::StickerLabel(Sticker* sticker, QWidget *parent) :

    QWidget(parent),
    ui(new Ui::StickerLabel)
{
    ui->setupUi(this);
    ui->lb_Image->setPixmap(QPixmap::fromImage(QImage(":/images/images/crypted-20.png")));

    QString label = QString(*sticker->getFirstName()).append(" ").append(*sticker->getLastName());
    ui->label->setText(label);
    ui->label->setToolTip(QString(*sticker->getEMail()));

    this->sticker = sticker;
}

QString
StickerLabel::text() {
    return ui->label->text();
}

Sticker*
StickerLabel::getSticker() {
    return this->sticker;
}

StickerLabel::~StickerLabel()
{
    delete ui;
}

void StickerLabel::on_toolButton_clicked()
{
    emit deleteIconClicked(this);
}
