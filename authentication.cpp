#include "authentication.h"

#include <QSettings>
#include <QDebug>
#include <QCryptographicHash>

const QString ORGANISATION = "Sample Organisaton";
const QString APPLICATION_NAME = "Sample Application";
const QString DEFAULT_ADMIN_USERNAME = "Administrator";
const QString DEFAULT_ADMIN_PASSWORD = "admin";

//------------------------------------------------------------------------------
/// Constructor - load existing users
Authentication_manager::Authentication_manager(QObject *parent) : QObject(parent)
{
    _authentication_state = AUTHENTICATION_STATE_NOT_AUTHENTICATED;
    _current_user = NULL;

    _load_users();
}

//------------------------------------------------------------------------------
Authentication_manager::~Authentication_manager() {
    _current_user = NULL;
    while (!_all_users.isEmpty()) {
        delete _all_users.begin().value();
        _all_users.erase(_all_users.begin());
    }
}

//------------------------------------------------------------------------------
/// Returns a list of available users
User_map Authentication_manager::get_available_users() {
    User_map user_map;
    for (QMap<QString, User_details*>::iterator iter = _all_users.begin(); iter != _all_users.end(); iter++) {
        user_map[iter.value()->username] = iter.value()->user_role;
    }
    return user_map;
}

//------------------------------------------------------------------------------
/// Authenticates the specified user with the specified password
Login_result Authentication_manager::authenticate(QString username, QString password) {
    Login_result login_result = LOGIN_RESULT_INVALID_USER;
    if (_authentication_state == AUTHENTICATION_STATE_NOT_AUTHENTICATED) {
        QCryptographicHash hasher(QCryptographicHash::Sha256);
        hasher.addData(password.toLocal8Bit());

        if (_all_users.find(username) != _all_users.end()) {
            User_details* user_details = _all_users[username];

            QString hash = hasher.result().toBase64();
            QString expected_hash = user_details->password_hash;

            if (hash == expected_hash) {
                qDebug() << "User" << username << "authenticated";
                login_result = LOGIN_RESULT_SUCCESS;
                _current_user = user_details;
                _authentication_state = AUTHENTICATION_STATE_AUTHENTICATED;

                emit signal_user_authenticated(_current_user->username, _current_user->user_role);

            } else {

                qDebug() << "User" << username << "password incorrect";
                login_result = LOGIN_RESULT_INVALID_PASSWORD;

                emit signal_user_authentication_failed(username, login_result);
            }
        } else {
            qDebug() << "User" << username << "is not valid";
            login_result = LOGIN_RESULT_INVALID_USER;

            emit signal_user_authentication_failed(username, login_result);
        }

    } else {
        login_result = LOGIN_RESULT_ALREADY_LOGGED_IN;
        qDebug() << "Already logged in";
    }
    return login_result;
}

//------------------------------------------------------------------------------
/// Logs out the current user
void Authentication_manager::logout() {
    if (_authentication_state == AUTHENTICATION_STATE_AUTHENTICATED) {
        QString username = _current_user->username;
        _authentication_state = AUTHENTICATION_STATE_NOT_AUTHENTICATED;
        _current_user = NULL;
        emit signal_user_logged_out(username);
    }
}

//------------------------------------------------------------------------------
/// Adds a new user with specified username and default password
Add_user_result Authentication_manager::add_user(QString username, QString default_password, User_role role) {
    Add_user_result result = ADD_USER_RESULT_PERMISSION_DENIED;

    if (_authentication_state == AUTHENTICATION_STATE_AUTHENTICATED && _current_user->user_role == USER_ROLE_ADMINISTRATOR) {

        if (_all_users.contains(username)) {
            qDebug() << "User exsist, not adding";
            result = ADD_USER_RESULT_USER_EXISTS;
        } else {
            qDebug() << "Adding user" << username;

            User_details* details = new User_details;
            details->username = username;
            details->user_role = role;

            // Hash the password
            QCryptographicHash hasher(QCryptographicHash::Sha256);
            hasher.addData(default_password.toLocal8Bit());
            details->password_hash = hasher.result().toBase64();

            _all_users[username] = details;

            _save_users();

            // Notify interested parties that the users have been updated
            emit signal_users_updated();
        }
    } else {
        qDebug() << "User is not an administrator, can't add user";
        result = ADD_USER_RESULT_PERMISSION_DENIED;
    }
    return result;
}

//------------------------------------------------------------------------------
/// Changes the password of the currently logged in user
Modify_result Authentication_manager::change_current_user_password(QString new_password) {
    Modify_result result = MODIFY_RESULT_NOT_LOGGED_IN;

    if (_authentication_state == AUTHENTICATION_STATE_AUTHENTICATED) {
        if (_current_user != NULL) {
            qDebug() << "Updating password of" << _current_user->username;

            // Hash the password
            QCryptographicHash hasher(QCryptographicHash::Sha256);
            hasher.addData(new_password.toLocal8Bit());
            _current_user->password_hash = hasher.result().toBase64();

            _save_users();

            // Notify interested parties that the users have been updated
            emit signal_users_updated();

        } else {
            result = MODIFY_RESULT_PERMISSION_DENIED;
        }
    } else {
        result = MODIFY_RESULT_NOT_LOGGED_IN;
    }
    return result;
}

//------------------------------------------------------------------------------
/// Changes the password of the currently logged in user
Modify_result Authentication_manager::change_password(QString username, QString new_password) {
    Modify_result result = MODIFY_RESULT_NOT_LOGGED_IN;

    if (_authentication_state == AUTHENTICATION_STATE_AUTHENTICATED) {
        if (_current_user != NULL) {
            qDebug() << "Updating password of" << username;

            if (_current_user->user_role == USER_ROLE_ADMINISTRATOR || _current_user->username == username) {
                if (_all_users.contains(username)) {
                    qDebug() << "Updating password of" << username;

                    // Hash the password
                    QCryptographicHash hasher(QCryptographicHash::Sha256);
                    hasher.addData(new_password.toLocal8Bit());
                    _all_users[username]->password_hash = hasher.result().toBase64();

                    _save_users();

                    // Notify interested parties that the users have been updated
                    emit signal_users_updated();
                } else {
                    qDebug() << "Could not find user" << username;
                    result = MODIFY_RESULT_USER_NOT_FOUND;
                }
            } else {
                result = MODIFY_RESULT_PERMISSION_DENIED;
            }
        } else {
            result = MODIFY_RESULT_PERMISSION_DENIED;
        }
    } else {
        result = MODIFY_RESULT_NOT_LOGGED_IN;
    }
    return result;
}

//------------------------------------------------------------------------------
/// Changes the password of the currently logged in user
Modify_result Authentication_manager::change_role(QString username, User_role new_role) {
    Modify_result result = MODIFY_RESULT_NOT_LOGGED_IN;

    if (_authentication_state == AUTHENTICATION_STATE_AUTHENTICATED) {
        if (_current_user != NULL) {
            qDebug() << "Updating password of" << _current_user->username;

            if (_current_user->user_role == USER_ROLE_ADMINISTRATOR) {
                if (_all_users.contains(username)) {
                    qDebug() << "Updating role of" << username;

                    _all_users[username]->user_role = new_role;

                    _save_users();

                    // Notify interested parties that the users have been updated
                    emit signal_users_updated();
                } else {
                    qDebug() << "Could not find user" << username;
                    result = MODIFY_RESULT_USER_NOT_FOUND;
                }
            } else {
                result = MODIFY_RESULT_PERMISSION_DENIED;
            }
        } else {
            result = MODIFY_RESULT_PERMISSION_DENIED;
        }
    } else {
        result = MODIFY_RESULT_NOT_LOGGED_IN;
    }
    return result;
}

//------------------------------------------------------------------------------
/// Removes the specifies user
void Authentication_manager::remove_user(QString username) {
    if (_all_users.contains(username)) {
        if (_all_users[username]->user_role != USER_ROLE_ADMINISTRATOR) {
            _all_users.remove(username);

            _save_users();

            // Notify interested parties that the users have been updated
            emit signal_users_updated();
        } else {
            qWarning() << "Cannot remove administrator";
        }

    } else {
        qDebug() << "User" << username << "not found";
    }
}

//------------------------------------------------------------------------------
/// Gets the role of the specified user
bool Authentication_manager::get_user_role(QString username, User_role & role) {
    if (_all_users.contains(username)) {
        role = _all_users[username]->user_role;
        return true;
    } else {
        return false;
    }
}


//------------------------------------------------------------------------------
/// Determines if currently logged in
bool Authentication_manager::logged_in() {
    return _authentication_state == AUTHENTICATION_STATE_AUTHENTICATED;
}

//------------------------------------------------------------------------------
/// Returns the username of the currently logged in user
QString Authentication_manager::get_logged_in_user_name() {
    if (_authentication_state == AUTHENTICATION_STATE_AUTHENTICATED && _current_user != NULL) {
        return _current_user->username;
    } else {
        return "";
    }
}

//------------------------------------------------------------------------------
/// Loads the list of all available users
bool Authentication_manager::_load_users() {
    qDebug() << "Loading all users";

    // Reset authentication state
    _authentication_state = AUTHENTICATION_STATE_NOT_AUTHENTICATED;
    _current_user = NULL;

    // Delete existing users
    while (!_all_users.isEmpty()) {
        delete _all_users.begin().value();
        _all_users.erase(_all_users.begin());
    }

    bool success = true;

    // Create settings reader
    QSettings settings(ORGANISATION, APPLICATION_NAME, this);
    settings.beginGroup("Passwords");

    // Get details of all users
    QList<QString> users = settings.allKeys();

    // Iterate though all settings and
    foreach(QString user_details_name, users) {

        // Ensure this is a key for a user
        if (user_details_name.startsWith("user_details_")) {
            QStringList user_detail_list = settings.value(user_details_name).toString().split('|');

            // Ensure three entries are available
            if (user_detail_list.size() == 3) {
                User_details* details = new User_details;
                details->username = user_detail_list.at(0);
                details->user_role = static_cast<User_role>(user_detail_list.at(1).toInt());
                details->password_hash = user_detail_list.at(2);

                qDebug() << "User" << details->username << "details loaded";

                _all_users[details->username] = details;
            } else {
                qWarning() << "Invalid user entry list";
            }
        }
    }

    settings.endGroup();

    // Create a default user if no users exist
    if (_all_users.empty()) {
        qDebug() << "No users defined. Creating default administrator account";
        User_details* details = new User_details;
        details->username = DEFAULT_ADMIN_USERNAME;
        details->user_role = USER_ROLE_ADMINISTRATOR;

        // Hash the password
        QCryptographicHash hasher(QCryptographicHash::Sha256);
        hasher.addData(DEFAULT_ADMIN_PASSWORD.toLocal8Bit());

        details->password_hash = hasher.result().toBase64();

        _all_users[details->username] = details;
   }

    return success;
}

//------------------------------------------------------------------------------
/// Saves the list of all available users
bool Authentication_manager::_save_users() {
    qDebug() << "Saving all users";

    bool success = true;

    // Create object for saving password
    QSettings settings(ORGANISATION, APPLICATION_NAME, this);

    // Store passwords in Passwords section
    settings.beginGroup("Passwords");

    // Delete existing passwords
    // Get details of all users
    QList<QString> users = settings.allKeys();

    // Iterate though all settings and
    foreach(QString user_details_name, users) {
        qDebug() << "Removing" << user_details_name;
        settings.remove(user_details_name);
    }

    // Save details of each user
    for (QMap<QString, User_details*>::iterator iter = _all_users.begin(); iter != _all_users.end(); iter++) {
        QString user_details_csv;
        user_details_csv = iter.value()->username;
        user_details_csv.append("|");
        user_details_csv.append(QString("%1").arg(iter.value()->user_role));
        user_details_csv.append("|");
        user_details_csv.append(iter.value()->password_hash);
        settings.setValue(QString("user_details_%1").arg(iter.value()->username), user_details_csv);

        qDebug() << "User detals:" << user_details_csv;
    }

    return success;
}

