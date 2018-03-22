#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "authentication.h"
#include "logindialog.h"

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
    /// Signal indicating that the user is attempting to log in
    void on_actionLogin_triggered();

    /// Signal indicating that the user management is to be started
    void on_actionManage_Users_triggered();

    /// Signal notifying that a user has been authenticated
    void slot_user_authenticated(QString username, User_role role);

    /// Signal notifying that a user has been authenticated
    void slot_user_authentication_failed(QString username, Login_result result);

    /// Signal notifying that a user has been logged out
    void slot_user_logged_out(QString username);

    /// Slot for handling the current user password change
    void slot_user_password_change(QString username, QString password);

    /// Action for handling user logout
    void on_actionLogout_triggered();

    /// Action for handling user password change
    void on_actionChange_Password_triggered();

private:
    Ui::MainWindow *ui;

    /// Pointer to the Authentication_manager object
    Authentication_manager* _authentication_manager;

    /// Indicates whether the user is logged in or not
    QLabel* _login_status;
};

#endif // MAINWINDOW_H
