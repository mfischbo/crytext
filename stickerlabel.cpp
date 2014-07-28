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
}

QString
StickerLabel::text() {
    return ui->label->text();
}

StickerLabel::~StickerLabel()
{
    delete ui;
}

void StickerLabel::on_toolButton_clicked()
{
    emit deleteIconClicked(this);
}
