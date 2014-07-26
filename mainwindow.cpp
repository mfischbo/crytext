#include <QFileDialog>
#include <QTextDocument>
#include <QDebug>
#include "settingsdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    this->service = new crytext::CryTextService();
    ui->setupUi(this);
    this->setWindowTitle(tr("CryText // New File"));

    // adjust tab stop width.. who in the world uses 8 spaces for tabs??
    ui->plainTextEdit->setTabStopWidth(4);

    // build the list of stickers
    QListIterator<crytext::Sticker*> it(*this->service->getAvailableStickers());
    while (it.hasNext()) {
        crytext::Sticker *s = it.next();
        QListWidgetItem *item = new QListWidgetItem(ui->stickerList);
        QString txt;
        txt.append(*s->getFirstName());
        txt.append(" ");
        txt.append(*s->getLastName());

        item->setText(txt);

        item->setData(0, QVariant(*s->getEMail()));
        ui->stickerList->addItem(item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete service;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
           tr("Open File"), service->getSettings()->getRecentDirectory(),
           tr("Crytext Files (*.cry);; Text Files (*.*)"));

    if (filename.length() > 0) {
        QTextDocument* doc = service->open(filename);
        QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(doc);
        doc->setDocumentLayout(layout);
        ui->plainTextEdit->setDocument(doc);
        this->setWindowTitle(filename);
        this->currentFile = filename;
    }
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog* sd = new SettingsDialog(this->service, this);
    sd->show();
}

void MainWindow::on_actionNew_triggered()
{
    QTextDocument *doc = new QTextDocument(this);
    QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(doc);
    doc->setDocumentLayout(layout);
    ui->plainTextEdit->setDocument(doc);
    this->setWindowTitle("CryText // New File");

}

void MainWindow::on_actionSave_as_triggered()
{
    const QString filename = QFileDialog::getSaveFileName(
                this, tr("Save as"),
                service->getSettings()->getRecentDirectory(),
                tr("Crytext Files (*.cry)"));

    if (filename != 0) {
        qDebug() << "Saving editor contents to " << filename;
        service->saveAs(filename, ui->plainTextEdit->document());
        this->currentFile = filename;
        this->setWindowTitle(filename);
    }
}

void MainWindow::on_actionClose_triggered()
{
    delete service;
    QCoreApplication::quit();
}

void MainWindow::on_actionStickers_triggered()
{
    ui->plainTextEdit->hide();
}

// import sticker
void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Choose a sticker to import"),
        service->getSettings()->getRecentDirectory(),
        "Crytext Sticker (*.cts)");


    if (filename.length() > 0) {
        crytext::Sticker *s = this->service->importSticker(filename);
        if (s != 0) {
            QListWidgetItem *item = new QListWidgetItem(ui->stickerList);
            QString txt;
            txt.append(*s->getFirstName());
            txt.append(" ");
            txt.append(*s->getLastName());

            item->setText(txt);
            item->setData(0, QVariant(*s->getEMail()));
            ui->stickerList->addItem(item);
        }
    }
}

void MainWindow::on_btn_RemoveSticker_clicked()
{
    QListWidgetItem *item = ui->stickerList->currentItem();
    service->removeStickerByEmail(item->data(0).toString());
    delete item;
}

void MainWindow::on_stickerList_itemDoubleClicked(QListWidgetItem *item)
{
    crytext::Sticker *s = service->getStickerByEmail(item->data(0).toString());
    service->addRecipient(s);

    QLabel *l = new QLabel(ui->Recipients);
    l->setText(*s->getEMail());
    ui->Recipients->layout()->addWidget(l);
}

void MainWindow::on_actionSave_triggered()
{
    if (this->currentFile.length() == 0)
        return this->on_actionSave_as_triggered();
    else
        service->saveAs(this->currentFile, ui->plainTextEdit->document());
}

void MainWindow::on_stickerList_itemClicked(QListWidgetItem *item)
{
    ui->btn_RemoveSticker->setEnabled(true);
}

void MainWindow::on_actionAbout_triggered()
{
    ui->plainTextEdit->document()->setPlainText("CryText v0.1 - Icons from http://www.icon8.com/");
}
