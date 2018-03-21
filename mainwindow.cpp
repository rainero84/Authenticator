#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user_management.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _authentication_manager(new Authentication_manager)
{
    _login_dialog = new LoginDialog(_authentication_manager);
    _login_dialog->setModal(true);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete _authentication_manager;
    delete ui;
}

void MainWindow::on_actionLogin_triggered()
{
    _login_dialog->show();
}

void MainWindow::on_actionManage_Users_triggered()
{
    User_management* user_management = new User_management(this);
    user_management->setModal(true);
    user_management->show();
}
