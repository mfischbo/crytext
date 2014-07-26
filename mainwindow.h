#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
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

private:
    Ui::MainWindow *ui;
    crytext::CryTextService *service;
    crytext::SMTPService    *mailService;
    QString currentFile;
};

#endif // MAINWINDOW_H
