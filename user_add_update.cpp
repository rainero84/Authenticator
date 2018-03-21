#include "user_add_update.h"
#include "ui_user_add_update.h"

#include <QString>
const QString UNMODIFIED_PASSWORD = "*******************";

User_add_update::User_add_update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_add_update)
{
    ui->setupUi(this);
    setWindowTitle("Add User...");

    ui->combo_role->addItem("Engineer", USER_ROLE_ENGINEER);
    ui->combo_role->addItem("User", USER_ROLE_USER);

    _is_edit = false;
}

User_add_update::User_add_update(QString username, User_role role, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_add_update)
{
    ui->setupUi(this);
    setWindowTitle("Edit User...");
    ui->edit_username->setText(username);
    ui->edit_username->setEnabled(false);

    ui->combo_role->addItem("Administrator", USER_ROLE_ADMINISTRATOR);
    ui->combo_role->addItem("Engineer", USER_ROLE_ENGINEER);
    ui->combo_role->addItem("User", USER_ROLE_USER);

    switch(role) {
    case USER_ROLE_ADMINISTRATOR: ui->combo_role->setCurrentText("Administrator"); break;
    case USER_ROLE_ENGINEER:      ui->combo_role->setCurrentText("Engineer"); break;
    case USER_ROLE_USER:          ui->combo_role->setCurrentText("User"); break;
    }

    ui->edit_password->setText(UNMODIFIED_PASSWORD);

    _is_edit = true;
}

User_add_update::~User_add_update()
{
    delete ui;
}

void User_add_update::on_btn_ok_clicked()
{
    if (_is_edit) {
        if (ui->edit_password->text() == UNMODIFIED_PASSWORD) {
            emit signal_user_edit(ui->edit_username->text(), ui->edit_password->text(), static_cast<User_role>(ui->combo_role->currentData().toInt()));
        } else {
            emit signal_user_edit(ui->edit_username->text(), static_cast<User_role>(ui->combo_role->currentData().toInt()));
        }
    } else {
        emit signal_user_add(ui->edit_username->text(), ui->edit_password->text(), static_cast<User_role>(ui->combo_role->currentData().toInt()));
    }
}
