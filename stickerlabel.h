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
    ~StickerLabel();

private:
    Ui::StickerLabel *ui;
};

#endif // STICKERLABEL_H
