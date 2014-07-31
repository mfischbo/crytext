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

#include <QFileDialog>
#include <QFont>
#include <QTextDocument>
#include <QDebug>
#include "settingsdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sendstickerdialog.h"
#include "emaildialog.h"
#include "stickerlabel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("CryText // New File"));
    this->setWindowIcon(QIcon(":/images/images/crypted.png"));

    this->service = new crytext::CryTextService();

    // adjust tab stop width.. who in the world uses 8 spaces for tabs??
    ui->plainTextEdit->setTabStopWidth(4 * QFontMetrics(this->getDefaultFont()).width(' '));
    ui->plainTextEdit->setFont(this->getDefaultFont());

    // build the list of stickers
    QListIterator<crytext::Sticker*> it(*this->service->getAvailableStickers());
    ui->stickerList->setIconSize(QSize(32, 32));
    while (it.hasNext()) {
        crytext::Sticker *s = it.next();

        QString txt;
        txt.append(*s->getFirstName());
        txt.append(" ");
        txt.append(*s->getLastName());
        txt.append("\n").append(*s->getEMail());

        QListWidgetItem *item = new QListWidgetItem(QIcon(":/images/images/crypted-32.png"), txt, ui->stickerList);
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
        doc->setDefaultFont(this->getDefaultFont());
        QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(doc);
        doc->setDocumentLayout(layout);
        ui->plainTextEdit->setDocument(doc);
        ui->plainTextEdit->setTabStopWidth(4 * QFontMetrics(this->getDefaultFont()).width(' '));
        ui->plainTextEdit->setFont(this->getDefaultFont());
        this->setWindowTitle(filename);
        this->currentFile = filename;
    }
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog* sd = new SettingsDialog(this->service, this);
    sd->show();
}

void MainWindow::on_actionSticker_Mail_triggered()
{
    SendStickerDialog* d = new SendStickerDialog(this->service, this);
    d->show();
}

void MainWindow::on_actionNew_triggered()
{
    QTextDocument *doc = new QTextDocument(this);
    QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(doc);
    doc->setDocumentLayout(layout);
    doc->setDefaultFont(this->getDefaultFont());
    ui->plainTextEdit->setDocument(doc);
    ui->plainTextEdit->setTabStopWidth(4 * QFontMetrics(this->getDefaultFont()).width(' '));
    ui->plainTextEdit->setFont(this->getDefaultFont());
    this->setWindowTitle("CryText // New File");

}

void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
                this, tr("Save as"),
                service->getSettings()->getRecentDirectory(),
                tr("Crytext Files (*.cry)"));

    if (filename != 0) {
        if (!filename.endsWith(".cry"))
            filename.append(".cry");

        qDebug() << "Saving editor contents to " << filename;
        service->saveAs(filename, ui->plainTextEdit->document());
        this->currentFile = filename;
        this->setWindowTitle(filename.left(1));
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
            txt.append("\n").append(*s->getEMail());

            item->setText(txt);
            //item->setData(0, QVariant(*s->getEMail()));

            item->setIcon(QIcon(":/images/images/crypted-32.png"));
            ui->stickerList->addItem(item);
        }
    }
}

void MainWindow::on_btn_RemoveSticker_clicked()
{
    QListWidgetItem *item = ui->stickerList->currentItem();
    QString txt = item->text();
    QStringList sl = txt.split(QRegExp("\n"));
    if (sl.length() == 2) {
        service->removeStickerByEmail(sl[1]);
        delete item;
    }
}

void MainWindow::on_stickerList_itemDoubleClicked(QListWidgetItem *item)
{
    // grab the stickers email
    QString txt = item->text();
    QStringList sl = txt.split(QRegExp("\n"));
    if (sl.length() == 2) {

        QString email = sl[1];

        crytext::Sticker *s = service->getStickerByEmail(email);
        service->addRecipient(s);

        StickerLabel* l = new StickerLabel(s, this);
        connect(l, SIGNAL(deleteIconClicked(QWidget*)), this, SLOT(onStickerRemoveClicked(QWidget*)));
        ui->Recipients->layout()->addWidget(l);
    }
}

void MainWindow::onStickerRemoveClicked(QWidget* label) {
    ui->Recipients->layout()->removeWidget(label);
    StickerLabel* l = (StickerLabel*) label;
    Sticker* s = l->getSticker();
    service->removeRecipient(s);
    delete label;
}

void MainWindow::on_actionSave_triggered()
{
    if (this->currentFile.length() == 0)
        return this->on_actionSave_as_triggered();
    else {
        service->saveAs(this->currentFile, ui->plainTextEdit->document());
        this->setWindowTitle(this->currentFile);
    }
}

void MainWindow::on_stickerList_itemClicked(QListWidgetItem *item)
{
    ui->btn_RemoveSticker->setEnabled(true);
}

void MainWindow::on_actionAbout_triggered()
{
    ui->plainTextEdit->document()->setPlainText("CryText v0.1 - Icons from http://www.icon8.com/");
}



void MainWindow::on_actionSend_Mail_triggered()
{
    EmailDialog* ed = new EmailDialog(this->service, ui->plainTextEdit->document(), this);
    ed->show();
    /*
    service->sendAsEMail("Test", "This is radio active", ui->plainTextEdit->document());
    */
}

const QFont
MainWindow::getDefaultFont() {
    return QFont("Bitstream Vera Sans Mono", 10);
}

void MainWindow::on_plainTextEdit_textChanged()
{
    if (!this->windowTitle().endsWith("*")) {
        QString t = this->windowTitle().append("*");
        this->setWindowTitle(t);
    }
}
