#include "user_management.h"
#include "ui_user_management.h"

#include <QDebug>
#include <QTableWidgetItem>

//------------------------------------------------------------------------------
/// Constructor. Refreshes current user list
User_management::User_management(Authentication_manager *authentication_manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_management),
    _authentication_manager(authentication_manager)
{
    ui->setupUi(this);

    connect(_authentication_manager, SIGNAL(signal_users_updated()), this, SLOT(slot_users_updated()));

    _refresh();
}

//------------------------------------------------------------------------------
/// Destructor
User_management::~User_management()
{
    delete ui;
}

//------------------------------------------------------------------------------
/// Handles user requesting addition of new user
void User_management::on_btn_add_clicked()
{
    User_add_update* add_dialog = new User_add_update(this);
    connect(add_dialog, SIGNAL(signal_user_add(QString,QString,User_role)), this, SLOT(slot_user_add(QString,QString,User_role)));
    connect(add_dialog, SIGNAL(accepted()), add_dialog, SLOT(deleteLater()));
    add_dialog->setModal(true);
    add_dialog->show();
}

//------------------------------------------------------------------------------
/// Handles user password and role change
void User_management::slot_user_edit(QString username, QString password, User_role role) {
    qDebug() << "Editing" << username;
    _authentication_manager->change_password(username, password);
    _authentication_manager->change_role(username, role);
}

//------------------------------------------------------------------------------
/// Handles user role change
void User_management::slot_user_edit(QString username, User_role role) {
    qDebug() << "Editing" << username;
    _authentication_manager->change_role(username, role);
}

//------------------------------------------------------------------------------
/// Handles user password and role add
void User_management::slot_user_add(QString username, QString password, User_role role) {
    qDebug() << "Adding" << username;
    _authentication_manager->add_user(username, password, role);
}

//------------------------------------------------------------------------------
/// Handles user requesting removal of user
void User_management::on_btn_remove_clicked()
{
    if (ui->table_users->currentRow() != -1) {
        QString username = ui->table_users->item(ui->table_users->currentRow(), USER_DETAIL_COLUMN_USERNAME)->text();

        _authentication_manager->remove_user(username);
    }
}

//------------------------------------------------------------------------------
/// Handles user requesting editing of selected user
void User_management::on_btn_edit_clicked()
{
    if (ui->table_users->currentRow() != -1) {
        QString username = ui->table_users->item(ui->table_users->currentRow(), USER_DETAIL_COLUMN_USERNAME)->text();

        User_role role;
        if (_authentication_manager->get_user_role(username, role)) {
            User_add_update* edit_dialog = new User_add_update(username, role, this);
            connect(edit_dialog, SIGNAL(signal_user_edit(QString,QString,User_role)), this, SLOT(slot_user_edit(QString,QString,User_role)));
            connect(edit_dialog, SIGNAL(signal_user_edit(QString,User_role)), this, SLOT(slot_user_edit(QString,User_role)));
            connect(edit_dialog, SIGNAL(accepted()), edit_dialog, SLOT(deleteLater()));
            edit_dialog->setModal(true);
            edit_dialog->show();
        }
    }

}

//------------------------------------------------------------------------------
/// Handles an update to users, which refreshes the list of users
void User_management::slot_users_updated() {
    _refresh();
}

//------------------------------------------------------------------------------
/// Refreshes the list of users
void User_management::_refresh() {

    User_map users = _authentication_manager->get_available_users();

    ui->table_users->setRowCount(0);

    for (User_map::iterator iter = users.begin(); iter != users.end(); iter++) {
        int row = ui->table_users->rowCount();
        ui->table_users->setRowCount(ui->table_users->rowCount() + 1);

        QTableWidgetItem* table_username = new QTableWidgetItem;
        table_username->setText(iter.key());
        ui->table_users->setItem(row, USER_DETAIL_COLUMN_USERNAME, table_username);

        QTableWidgetItem* table_row = new QTableWidgetItem;
        switch (iter.value()) {
        case USER_ROLE_ADMINISTRATOR: table_row->setText("Administrator"); break;
        case USER_ROLE_ENGINEER     : table_row->setText("Engineer"); break;
        case USER_ROLE_USER         : table_row->setText("User"); break;
        case USER_ROLE_UNSPECIFIED  : table_row->setText("<Unspecified>"); break;
        }


        ui->table_users->setItem(row, USER_DETAIL_COLUMN_PASSWORD, table_row);
    }

    ui->table_users->resizeRowsToContents();
}
