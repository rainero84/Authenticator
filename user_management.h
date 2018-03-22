#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <QDialog>
#include "user_add_update.h"
#include "authentication.h"

namespace Ui {
class User_management;
}

typedef enum {
    USER_DETAIL_COLUMN_USERNAME = 0,
    USER_DETAIL_COLUMN_PASSWORD = 1
} User_detail_column;

class User_management : public QDialog
{
    Q_OBJECT

public:
    /// Constructor. Refreshes current user list
    explicit User_management(Authentication_manager* authentication_manager, QWidget *parent = 0);

    /// Destructor
    ~User_management();

private slots:
    /// Handles user password and role change
    void slot_user_edit(QString username, QString password, User_role role);

    /// Handles user role change
    void slot_user_edit(QString username, User_role role);

    /// Handles user password and role add
    void slot_user_add(QString username, QString password, User_role role);

    /// Handles user requesting addition of new user
    void on_btn_add_clicked();

    /// Handles user requesting removal of user
    void on_btn_remove_clicked();

    /// Handles user requesting editing of selected user
    void on_btn_edit_clicked();

    /// Handles an update to users, which refreshes the list of users
    void slot_users_updated();

private:
    /// Refreshes the list of users
    void _refresh();

    Ui::User_management *ui;

    Authentication_manager *_authentication_manager;
};

#endif // USER_MANAGEMENT_H
