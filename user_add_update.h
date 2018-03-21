#ifndef USER_ADD_UPDATE_H
#define USER_ADD_UPDATE_H

#include <QDialog>
#include <logindialog.h>

namespace Ui {
class User_add_update;
}

class User_add_update : public QDialog
{
    Q_OBJECT

public:
    explicit User_add_update(QWidget *parent = 0);
    explicit User_add_update(QString username, User_role role, QWidget *parent = 0);
    ~User_add_update();

private slots:
    void on_btn_ok_clicked();

signals:
    void signal_user_edit(QString username, QString password, User_role role);
    void signal_user_edit(QString username, User_role role);
    void signal_user_add(QString username, QString password, User_role role);

private:
    Ui::User_add_update *ui;

    bool _is_edit;
};

#endif // USER_ADD_UPDATE_H
