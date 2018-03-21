#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QObject>
#include <QString>
#include <QMap>

/// @enum User_type
typedef enum {
    USER_ROLE_ADMINISTRATOR,
    USER_ROLE_ENGINEER,
    USER_ROLE_USER
} User_role;

typedef QMap<QString,User_role> User_map;


/// @enum Login_result enumerates the status of a login
typedef enum {
    LOGIN_RESULT_SUCCESS = 0,
    LOGIN_RESULT_INVALID_USER = 1,
    LOGIN_RESULT_INVALID_PASSWORD = 2
} Login_result;

/// @enum Add_user_result enumerate the possible user-add results
typedef enum {
    ADD_USER_RESULT_SUCCESS = 0,
    ADD_USER_RESULT_ADMIN_NOT_LOGGED_IN = 1,
    ADD_USER_RESULT_USER_EXISTS = 2,
    ADD_USER_RESULT_PERMISSION_DENIED = 3
} Add_user_result;

/// @enum Modify_password_result enumerates the possible results of changing a user property
typedef enum {
    MODIFY_RESULT_SUCCESS = 0,
    MODIFY_RESULT_NOT_LOGGED_IN = 1,
    MODIFY_RESULT_PERMISSION_DENIED = 2,
    MODIFY_RESULT_USER_NOT_FOUND = 3
} Modify_result;

/// @enum Authentication_state enumerates the authentication state;
typedef enum {
    AUTHENTICATION_STATE_NOT_AUTHENTICATED = -1,
    AUTHENTICATION_STATE_AUTHENTICATED = 0
} Authentication_state;

/// @struct User_details holds the details of a user
typedef struct {
    QString username;
    User_role user_role;
    QString password_hash;
} User_details;

class Authentication_manager : public QObject
{
    Q_OBJECT
public:
    /// Constructor - load existing users
    explicit Authentication_manager(QObject *parent = 0);

    /// Destructor - delete all user details
    ~Authentication_manager();

    /// Returns a list of available users
    User_map get_available_users();

    /// Authenticates the specified user with the specified password
    Login_result authenticate(QString username, QString password);

    /// Logs out the current user
    void logout();

    /// Adds a new user with specified username and default password
    Add_user_result add_user(QString username, QString default_password, User_role role);

    /// Changes the password of the currently logged in user
    Modify_result change_current_user_password(QString new_password);

    /// Changes the password of a user
    Modify_result change_password(QString username, QString new_password);

    /// Changes the role of a user
    Modify_result change_role(QString username, User_role new_role);


signals:
    /// Signal notifying that a user has been authenticated
    void signal_user_authenticated(QString username, User_role role);

    /// Signal notifying that a user has been authenticated
    void signal_user_authentication_failed(QString username, Login_result result);

    /// Signal notifying that a user has been logged out
    void signal_user_logged_out(QString username);

public slots:

private:
    /// Loads the list of all available users
    bool _load_users();

    /// Saves the list of all available users
    bool _save_users();

    /// Current state of authentication
    Authentication_state _authentication_state;

    /// Pointer to the details of the currently logged in user
    User_details* _current_user;

    /// Holds the details of all available users
    QMap<QString, User_details*> _all_users;
};

#endif // AUTHENTICATION_H
