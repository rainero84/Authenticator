#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "authentication.h"
#include "logindialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLogin_triggered();

    void on_actionManage_Users_triggered();

private:
    Ui::MainWindow *ui;
    Authentication_manager* _authentication_manager;
    LoginDialog* _login_dialog;
};

#endif // MAINWINDOW_H
