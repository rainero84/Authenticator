#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <QDialog>
#include "user_add_update.h"

namespace Ui {
class User_management;
}

class User_management : public QDialog
{
    Q_OBJECT

public:
    explicit User_management(QWidget *parent = 0);
    ~User_management();

private slots:
    void slot_user_edit(QString username, QString password, User_role role);
    void slot_user_edit(QString username, User_role role);
    void slot_user_add(QString username, QString password, User_role role);

    void on_btn_add_clicked();

private:
    Ui::User_management *ui;
};

#endif // USER_MANAGEMENT_H
