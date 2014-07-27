#ifndef EMAILDIALOG_H
#define EMAILDIALOG_H

#include <QDialog>
#include <QList>
#include <service/crytextservice.h>

using crytext::CryTextService;

namespace Ui {
class EmailDialog;
}

class EmailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmailDialog(CryTextService *service, QWidget *parent = 0);
    ~EmailDialog();

private slots:


    void on_btn_AddRecipient_clicked();

    void on_buttonBox_accepted();

private:
    QWidget* parent;
    Ui::EmailDialog *ui;
    CryTextService* service;
};

#endif // EMAILDIALOG_H
