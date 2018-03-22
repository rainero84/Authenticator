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
    /// Constructor for adding a new user
    explicit User_add_update(QWidget *parent = 0);

    /// Constructor for dialog for changing current user password
    explicit User_add_update(QString username, QWidget *parent = 0);

    /// Constructor for dialog for changing username and password
    explicit User_add_update(QString username, User_role role, QWidget *parent = 0);

    /// Destructor
    ~User_add_update();

private slots:
    /// Handles user pressing OK, thereby adding or editing the user
    void on_btn_ok_clicked();

    /// Handles the request for cancelling the user add / edit
    void on_btn_cancel_clicked();

signals:
    void signal_user_edit(QString username, QString password);
    void signal_user_edit(QString username, User_role role);
    void signal_user_edit(QString username, QString password, User_role role);
    void signal_user_add(QString username, QString password, User_role role);

private:
    Ui::User_add_update *ui;

    /// Indicates whether this dialog is being used for adding or editing
    bool _is_edit_dialog;
};

#endif // USER_ADD_UPDATE_H
