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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QPlainTextDocumentLayout>
#include <QFont>
#include "service/crytextservice.h"
#include "service/smtpservice.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSettings_triggered();

    void on_actionNew_triggered();

    void on_actionSave_as_triggered();

    void on_actionClose_triggered();

    void on_actionStickers_triggered();

    void on_pushButton_2_clicked();

    void on_btn_RemoveSticker_clicked();

    void on_stickerList_itemDoubleClicked(QListWidgetItem *item);

    void on_actionSave_triggered();

    void on_stickerList_itemClicked(QListWidgetItem *item);

    void on_actionAbout_triggered();

    void on_actionSticker_Mail_triggered();

    void on_actionSend_Mail_triggered();

private:
    Ui::MainWindow *ui;
    crytext::CryTextService *service;
    QString currentFile;

    const QFont getDefaultFont();
};



#endif // MAINWINDOW_H
