#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"

#include <QDebug>

//------------------------------------------------------------------------------
/// Constructor - Loads users and adds to combo box
Login_dialog::Login_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    _authentication_manager(new Authentication_manager),
    _main_window(NULL)
{
    ui->setupUi(this);
    User_map users = _authentication_manager->get_available_users();
    for (User_map::iterator iter = users.begin(); iter != users.end(); iter++) {
        QString role = "<Unknown>";
        switch (iter.value()) {
        case USER_ROLE_ADMINISTRATOR: role = "Administrator"; break;
        case USER_ROLE_ENGINEER     : role = "Engineer"; break;
        case USER_ROLE_USER         : role = "User"; break;
        case USER_ROLE_UNSPECIFIED  : role = "<Unspecified>"; break;
        }

        ui->combo_username->addItem(QString("%1 [%2]").arg(iter.key()).arg(role), iter.key());
    }
    ui->edit_password->setFocus();

    // Connect signals
    connect(_authentication_manager, SIGNAL(signal_user_authenticated(QString,User_role)), this, SLOT(slot_user_authenticated(QString,User_role)));
    connect(_authentication_manager, SIGNAL(signal_user_authentication_failed(QString,Login_result)), this, SLOT(slot_user_authentication_failed(QString,Login_result)));
    connect(_authentication_manager, SIGNAL(signal_user_logged_out(QString)), this, SLOT(slot_user_logged_out(QString)));
}

//------------------------------------------------------------------------------
Login_dialog::~Login_dialog()
{
    if (_main_window != NULL) {
        delete _main_window;
    }
    delete _authentication_manager;
    delete ui;
}

//------------------------------------------------------------------------------
/// Handles user pressing Login button. Validates login and hides dialog
void Login_dialog::on_btn_login_clicked()
{
    qDebug() << ui->combo_username->currentData() << ui->edit_password->text();
    Login_result result = _authentication_manager->authenticate(ui->combo_username->currentData().toString(), ui->edit_password->text());
    switch (result) {
    case LOGIN_RESULT_SUCCESS         : ui->label_result->setText("Success"); break;
    case LOGIN_RESULT_INVALID_USER     : ui->label_result->setText("Invalid User"); break;
    case LOGIN_RESULT_INVALID_PASSWORD : ui->label_result->setText("Incorrect Password"); break;
    case LOGIN_RESULT_ALREADY_LOGGED_IN: ui->label_result->setText("Already logged in"); break;
    }

    qDebug() << result;
}

//------------------------------------------------------------------------------
/// Handles user pressing the Cancel button
void Login_dialog::on_btn_cancel_clicked()
{
    reject();
}

//------------------------------------------------------------------------------
/// Signal notifying that a user has been authenticated
void Login_dialog::slot_user_authenticated(QString username, User_role role) {
    qDebug() << username << "now logged in";
    ui->edit_password->setText("");

    hide();

    _main_window = new MainWindow(_authentication_manager, this);
    _main_window->show();
}

//------------------------------------------------------------------------------
/// Signal notifying that a user has been authenticated
void Login_dialog::slot_user_authentication_failed(QString username, Login_result result) {
    qDebug() << username << "login unsuccessful" << result;
}

//------------------------------------------------------------------------------
/// Signal notifying that a user has been logged out
void Login_dialog::slot_user_logged_out(QString username) {
    qDebug() << username << "logged out";
    ui->label_result->setText("");
    show();

    _main_window->hide();
    _main_window->deleteLater();
    _main_window = NULL;
}
