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
