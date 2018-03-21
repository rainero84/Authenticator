#include "user_management.h"
#include "ui_user_management.h"

#include <QDebug>

User_management::User_management(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_management)
{
    ui->setupUi(this);
}

User_management::~User_management()
{
    delete ui;
}

void User_management::on_btn_add_clicked()
{
    User_add_update* add_dialog = new User_add_update(this);
    connect(add_dialog, SIGNAL(signal_user_add(QString,QString,User_role)), this, SLOT(slot_user_add(QString,QString,User_role)));
    connect(add_dialog, SIGNAL(accepted()), add_dialog, SLOT(deleteLater()));
    add_dialog->setModal(true);
    add_dialog->show();
}


void User_management::slot_user_edit(QString username, QString password, User_role role) {
    qDebug() << "Editing" << username;
}

void User_management::slot_user_edit(QString username, User_role role) {
    qDebug() << "Editing" << username;
}

void User_management::slot_user_add(QString username, QString password, User_role role) {
    qDebug() << "Adding" << username;
}
