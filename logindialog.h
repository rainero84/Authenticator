#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMainWindow>
#include "authentication.h"
#include "logindialog.h"

namespace Ui {
class LoginDialog;
}

class Login_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Login_dialog(QWidget *parent = 0);
    ~Login_dialog();

    /// Handles user key press
    void keyPressEvent(QKeyEvent* event);

private slots:
    /// Handles user pressing Login button
    void on_btn_login_clicked();

    /// Handles user pressing the Cancel button
    void on_btn_cancel_clicked();

    /// Slot handling that a user has been authenticated
    void slot_user_authenticated(QString username, User_role role);

    /// Slot handling that a user has been authenticated
    void slot_user_authentication_failed(QString username, Login_result result);

    /// Slot handling user being logged out
    void slot_user_logged_out(QString username);

    /// Slot handling updated users
    void slot_users_updated();
private:
    /// Updates the displayed users
    void _refesh_users();

    Ui::LoginDialog *ui;

    /// Pointer to the Authentication_manager
    Authentication_manager* _authentication_manager;

    /// Pointer to the main window
    QMainWindow* _main_window;
};

#endif // LOGINDIALOG_H
