#include "sendstickerdialog.h"
#include "ui_sendstickerdialog.h"

SendStickerDialog::SendStickerDialog(crytext::CryTextService *service, crytext::SMTPService *mailer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendStickerDialog)
{
    ui->setupUi(this);
    this->service = service;
    this->mailer = mailer;
}

SendStickerDialog::~SendStickerDialog()
{
    delete ui;
}

void SendStickerDialog::on_buttonBox_accepted()
{
    QString recipient = ui->le_Recipient->text();
    QString message = ui->te_Message->document()->toPlainText();

    mailer->connect();
    mailer->sendSticker(recipient, &message);
    mailer->disconnect();
}
