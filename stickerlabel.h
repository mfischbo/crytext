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
