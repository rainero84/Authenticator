#include "logindialog.h"
#include "ui_logindialog.h"

#include <QDebug>

LoginDialog::LoginDialog(Authentication_manager *authentication_manager, QWidget *parent) :
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
}

LoginDialog::~LoginDialog()
{
    delete _authenticaton_manager;
    delete ui;
}

void LoginDialog::on_btn_login_clicked()
{
    qDebug() << ui->combo_username->currentData() << ui->edit_password->text();
    Login_result result = _authenticaton_manager->authenticate(ui->combo_username->currentData().toString(), ui->edit_password->text());
    switch (result) {
    case LOGIN_RESULT_SUCCESS         :
        ui->label_result->setText("Success");
        hide();
        break;
    case LOGIN_RESULT_INVALID_USER    : ui->label_result->setText("Invalid User"); break;
    case LOGIN_RESULT_INVALID_PASSWORD: ui->label_result->setText("Incorrect Password"); break;
    }

    qDebug() << result;
}
