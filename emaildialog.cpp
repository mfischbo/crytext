#include "emaildialog.h"
#include "ui_emaildialog.h"

using crytext::CryTextService;

EmailDialog::EmailDialog(CryTextService* service, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailDialog)
{
    ui->setupUi(this);
    this->service = service;
    this->parent = parent;
}

EmailDialog::~EmailDialog()
{
    delete ui;
}


void EmailDialog::on_btn_AddRecipient_clicked()
{

}

void EmailDialog::on_buttonBox_accepted()
{
    QString subject = ui->le_Subject->text();
    QString message = ui->pte_Message->document()->toPlainText();

}
