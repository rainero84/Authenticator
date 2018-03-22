#include "user_add_update.h"
#include "ui_user_add_update.h"

#include <QString>
const QString UNMODIFIED_PASSWORD = "*******************";
const QString USER_ADMINISTRATOR_STR = "Administrator";
const QString USER_ENGINEER_STR = "Engineer";
const QString USER_USER_STR = "User";
const QString USER_UNSPECIFIED_STR = "<Unspecified>";

//------------------------------------------------------------------------------
/// Constructor for adding a new user
User_add_update::User_add_update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_add_update)
{
    ui->setupUi(this);
    setWindowTitle("Add User...");

    ui->combo_role->addItem(USER_ENGINEER_STR, USER_ROLE_ENGINEER);
    ui->combo_role->addItem("User", USER_ROLE_USER);

    _is_edit_dialog = false;
}

//------------------------------------------------------------------------------
/// Constructor for dialog for changing current user password
User_add_update::User_add_update(QString username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_add_update)
{
    ui->setupUi(this);
    setWindowTitle("Edit User...");
    ui->edit_username->setText(username);
    ui->edit_username->setEnabled(false);
    ui->combo_role->setVisible(false);
    ui->label_role->setVisible(false);

    ui->edit_password->setText(UNMODIFIED_PASSWORD);

    _is_edit_dialog = true;
}

//------------------------------------------------------------------------------
/// Constructor for dialog for changing username and password
User_add_update::User_add_update(QString username, User_role role, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_add_update)
{
    ui->setupUi(this);
    setWindowTitle("Edit User...");
    ui->edit_username->setText(username);
    ui->edit_username->setEnabled(false);

    ui->combo_role->addItem(USER_ADMINISTRATOR_STR, USER_ROLE_ADMINISTRATOR);
    ui->combo_role->addItem(USER_ENGINEER_STR, USER_ROLE_ENGINEER);
    ui->combo_role->addItem(USER_USER_STR, USER_ROLE_USER);

    switch(role) {
    case USER_ROLE_ADMINISTRATOR: ui->combo_role->setCurrentText(USER_ADMINISTRATOR_STR); break;
    case USER_ROLE_ENGINEER:      ui->combo_role->setCurrentText(USER_ENGINEER_STR); break;
    case USER_ROLE_USER:          ui->combo_role->setCurrentText(USER_USER_STR); break;
    case USER_ROLE_UNSPECIFIED:   ui->combo_role->setCurrentText(USER_UNSPECIFIED_STR); break;
    }

    ui->edit_password->setText(UNMODIFIED_PASSWORD);

    _is_edit_dialog = true;
}

//------------------------------------------------------------------------------
/// Destructor
User_add_update::~User_add_update()
{
    delete ui;
}

//------------------------------------------------------------------------------
/// Handles user pressing OK, thereby adding or editing the user
void User_add_update::on_btn_ok_clicked()
{
    if (_is_edit_dialog) {

        // Check if the password changed
        if (!ui->combo_role->isVisible()) {

            // Check if the password was changed. If not, nothing needs to be done
            if (ui->edit_password->text() != UNMODIFIED_PASSWORD) {
                emit signal_user_edit(ui->edit_username->text(), ui->edit_password->text());
            }

        } else if (ui->edit_password->text() == UNMODIFIED_PASSWORD) {

            // If the password has been unmodified, only indicate role change
            emit signal_user_edit(ui->edit_username->text(), static_cast<User_role>(ui->combo_role->currentData().toInt()));
        } else {

            // If the password was changed, indicate password and role change
            emit signal_user_edit(ui->edit_username->text(), ui->edit_password->text(), static_cast<User_role>(ui->combo_role->currentData().toInt()));
        }
    } else {

        // Signal that a new user has been added
        emit signal_user_add(ui->edit_username->text(), ui->edit_password->text(), static_cast<User_role>(ui->combo_role->currentData().toInt()));
    }
    accept();
}

//------------------------------------------------------------------------------
/// Handles the request for cancelling the user add / edit
void User_add_update::on_btn_cancel_clicked()
{
    reject();
}
