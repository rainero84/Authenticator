#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user_management.h"
#include "user_add_update.h"

#include <QDebug>
#include <QMessageBox>


//------------------------------------------------------------------------------
/// Constructor - Create authentication manager and set up signal connections
MainWindow::MainWindow(Authentication_manager* authentication_manager, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _authentication_manager(authentication_manager),
    _login_status(new QLabel)
{

    ui->setupUi(this);

    // Add the label to the status bar indicating whether logged in or not
    this->statusBar()->addPermanentWidget(_login_status);

    _login_status->setText(_authentication_manager->get_logged_in_user_name());
}

//------------------------------------------------------------------------------
/// Destructor. Free allocated memory
MainWindow::~MainWindow()
{
    delete _login_status;
    delete ui;
}

//------------------------------------------------------------------------------
/// Handles user request to manager users
void MainWindow::on_actionManage_Users_triggered()
{
    if (_authentication_manager->logged_in() && _authentication_manager->get_logged_in_user_role() == USER_ROLE_ADMINISTRATOR) {
        User_management* user_management = new User_management(_authentication_manager, this);
        user_management->setModal(true);
        user_management->show();
    } else {
        QMessageBox::warning(this, "Manage Users", "Permission Denied");
    }
}

//------------------------------------------------------------------------------
/// Slot for handling the current user password change
void MainWindow::slot_user_password_change(QString, QString password) {
    qDebug() << "Changing password of current user";

    _authentication_manager->change_current_user_password(password);
}

//------------------------------------------------------------------------------
/// Gets called when the current user is to be logged out
void MainWindow::on_actionLogout_triggered()
{
    _authentication_manager->logout();
}

//------------------------------------------------------------------------------
/// Handles the signal of changing the current password
void MainWindow::on_actionChange_Password_triggered()
{
    if (_authentication_manager->logged_in()) {
        User_add_update* edit_dialog = new User_add_update(_authentication_manager->get_logged_in_user_name(), this);
        connect(edit_dialog, SIGNAL(signal_user_edit(QString,QString)), this, SLOT(slot_user_password_change(QString,QString)));
        connect(edit_dialog, SIGNAL(accepted()), edit_dialog, SLOT(deleteLater()));
        edit_dialog->setModal(true);
        edit_dialog->show();
    }
}
