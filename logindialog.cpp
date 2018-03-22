#include "logindialog.h"
#include "ui_logindialog.h"

#include <QDebug>

//------------------------------------------------------------------------------
/// Constructor - Loads users and adds to combo box
Login_dialog::Login_dialog(Authentication_manager *authentication_manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    _authenticaton_manager(authentication_manager)
{
    ui->setupUi(this);
    User_map users = _authenticaton_manager->get_available_users();
    for (User_map::iterator iter = users.begin(); iter != users.end(); iter++) {
        QString role = "<Unknown>";
        switch (iter.value()) {
        case USER_ROLE_ADMINISTRATOR: role = "Administrator"; break;
        case USER_ROLE_ENGINEER     : role = "Engineer"; break;
        case USER_ROLE_USER         : role = "User"; break;
        }

        ui->combo_username->addItem(QString("%1 [%2]").arg(iter.key()).arg(role), iter.key());
    }
    ui->edit_password->setFocus();
}

//------------------------------------------------------------------------------
Login_dialog::~Login_dialog()
{
    delete ui;
}

//------------------------------------------------------------------------------
/// Handles user pressing Login button. Validates login and hides dialog
void Login_dialog::on_btn_login_clicked()
{
    qDebug() << ui->combo_username->currentData() << ui->edit_password->text();
    Login_result result = _authenticaton_manager->authenticate(ui->combo_username->currentData().toString(), ui->edit_password->text());
    switch (result) {
    case LOGIN_RESULT_SUCCESS         :
        ui->label_result->setText("Success");

        /// Onky hide dialog / accept if user authenticated
        accept();
        break;
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
