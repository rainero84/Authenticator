#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "authentication.h"
#include "logindialog.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(Authentication_manager* authentication_manager, QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_btn_login_clicked();

private:
    Ui::LoginDialog *ui;

    Authentication_manager* _authenticaton_manager;
};

#endif // LOGINDIALOG_H
