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
