#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "authentication.h"
#include "logindialog.h"

namespace Ui {
class LoginDialog;
}

class Login_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Login_dialog(Authentication_manager* authentication_manager, QWidget *parent = 0);
    ~Login_dialog();

private slots:
    /// Handles user pressing Login button
    void on_btn_login_clicked();

    /// Handles user pressing the Cancel button
    void on_btn_cancel_clicked();

private:
    Ui::LoginDialog *ui;

    /// Pointer to the Authentication_manager
    Authentication_manager* _authenticaton_manager;
};

#endif // LOGINDIALOG_H
