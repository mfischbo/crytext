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

#ifndef STICKERLABEL_H
#define STICKERLABEL_H

#include <QWidget>
#include "model/sticker.h"

using crytext::Sticker;

namespace Ui {
class StickerLabel;
}

class StickerLabel : public QWidget
{
    Q_OBJECT

public:
    explicit StickerLabel(Sticker *sticker, QWidget *parent = 0);
    Sticker* getSticker();
    QString text();
    ~StickerLabel();

signals:
    void deleteIconClicked(QWidget*);

private slots:
    void on_toolButton_clicked();

private:
    Ui::StickerLabel *ui;
    Sticker *sticker;
};

#endif // STICKERLABEL_H
