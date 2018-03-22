#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user_management.h"
#include "user_add_update.h"

#include <QDebug>


//------------------------------------------------------------------------------
/// Constructor - Create authentication manager and set up signal connections
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _authentication_manager(new Authentication_manager),
    _login_status(new QLabel)
{

    ui->setupUi(this);
    ui->actionManage_Users->setEnabled(false);

    // Connect signals
    connect(_authentication_manager, SIGNAL(signal_user_authenticated(QString,User_role)), this, SLOT(slot_user_authenticated(QString,User_role)));
    connect(_authentication_manager, SIGNAL(signal_user_authentication_failed(QString,Login_result)), this, SLOT(slot_user_authentication_failed(QString,Login_result)));
    connect(_authentication_manager, SIGNAL(signal_user_logged_out(QString)), this, SLOT(slot_user_logged_out(QString)));

    // Show the Login dialog
    Login_dialog* login_dialog = new Login_dialog(_authentication_manager, this);
    login_dialog->setModal(true);
    connect(login_dialog, SIGNAL(accepted()), login_dialog, SLOT(deleteLater()));
    connect(login_dialog, SIGNAL(rejected()), login_dialog, SLOT(deleteLater()));
    login_dialog->show();

    // Add the label to the status bar indicating whether logged in or not
    this->statusBar()->addPermanentWidget(_login_status);
    _login_status->setText("Logged Out");
}

//------------------------------------------------------------------------------
/// Destructor. Free allocated memory
MainWindow::~MainWindow()
{
    delete _login_status;
    delete _authentication_manager;
    delete ui;
}

//------------------------------------------------------------------------------
/// Handles login request
void MainWindow::on_actionLogin_triggered()
{
    Login_dialog* login_dialog = new Login_dialog(_authentication_manager, this);
    login_dialog->setModal(true);
    connect(login_dialog, SIGNAL(accepted()), login_dialog, SLOT(deleteLater()));
    connect(login_dialog, SIGNAL(rejected()), login_dialog, SLOT(deleteLater()));
    login_dialog->show();
}

//------------------------------------------------------------------------------
/// Handles user request to manager users
void MainWindow::on_actionManage_Users_triggered()
{
    User_management* user_management = new User_management(_authentication_manager, this);
    user_management->setModal(true);
    user_management->show();
}

//------------------------------------------------------------------------------
/// Signal notifying that a user has been authenticated
void MainWindow::slot_user_authenticated(QString username, User_role role) {
    qDebug() << username << "now logged in";
    ui->actionManage_Users->setEnabled(role == USER_ROLE_ADMINISTRATOR);
    _login_status->setText("Logged In");
}

//------------------------------------------------------------------------------
/// Signal notifying that a user has been authenticated
void MainWindow::slot_user_authentication_failed(QString username, Login_result result) {
    qDebug() << username << "login unsuccessful" << result;
}

//------------------------------------------------------------------------------
/// Signal notifying that a user has been logged out
void MainWindow::slot_user_logged_out(QString username) {
    qDebug() << username << "logged out";
    ui->actionManage_Users->setEnabled(false);
    _login_status->setText("Logged Out");
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
