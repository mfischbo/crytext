#include "cryptsetup.h"
#include <QFileDialog>
#include "ui_cryptsetup.h"

CryptSetup::CryptSetup(CryTextService const *service, QWidget *parent) :
    QWizard(parent),
    ui(new Ui::CryptSetup)
{
    ui->setupUi(this);
    this->service = service;
}

CryptSetup::~CryptSetup()
{
    delete ui;
}

void CryptSetup::on_pushButton_clicked()
{
    QFileDialog d;
    d.setFileMode(QFileDialog::Directory);
    d.setOption(QFileDialog::ShowDirsOnly);
    QString dir = d.getExistingDirectory(this, tr("Settings directory"), "/");
    this->settingsDir = &dir;
    ui->dirNameEdit->setText(dir);
}
