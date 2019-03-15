#include "user.h"
#include <QtSql>

/**
 * Constructs an user object.
**/
User::User(const QString &userName, const QString &userUsername, const QString &userPassword, bool userIsAdmin)
{
    m_id = 0;
    m_name = userName;
    m_username = userUsername;
    m_password = userPassword;
    m_isadmin = userIsAdmin ? 1 : 0;
}

/**
 * Destructs the user object.
**/
User::~User()
{
}

/**
 * Returns the id of the user.
**/
int User::id() const
{
    return m_id;
}

/**
 * Returns the name of the user.
**/
const QString &User::name() const
{
    return m_name;
}

/**
 * Returns the username of the user.
**/
const QString &User::username() const
{
    return m_username;
}

/**
 * Returns the hashed password of the user.
**/
const QString &User::password() const
{
    return m_password;
}

/**
 * Returns 1 if user is an admin, 0 if not.
**/
bool User::isAdmin() const
{
    return (m_isadmin == 0 ? false : true);
}

/**
 * Sets the id of the user to userId.
**/
void User::setId(int userId)
{
    m_id = userId;
}

/**
 * Sets the name of the user to userName.
**/
void User::setName(const QString &userName)
{
    m_name = userName;
}

/**
 * Sets the username of the user to userUsername.
**/
void User::setUsername(const QString &userUsername)
{
    m_username = userUsername;
}

/**
 * Sets the hashed password of the user to userPassword.
**/
void User::setPassword(const QString &userPassword)
{
    m_password = userPassword;
}

/**
 * Sets the isAdmin attribut of the user to 1 if admin, 0 if not.
**/
void User::setIsAdmin(bool userIsAdmin)
{
    m_isadmin = userIsAdmin ? 1 : 0;
}

/**
 * Saves the user to database.
 *
 * If this is a newly created user, an INSERT INTO is used.
 * An user with its id = 0 is assumed to be a new user.
 * Once created, the id of the user is set to the id of the newly inserted user.
 *
 * If this user already existed in the database, an UPDATE statement is used.
 * A user with a nonzero value of ID is assumed to already have existed in the database.
**/
void User::save()
{
    QSqlQuery q;

    if (m_id == 0) {
        q.prepare(QLatin1String("INSERT INTO users (name, username, password, isadmin) VALUES(:name, :username, :password, :isadmin)"));
        q.bindValue(":name", m_name);
        q.bindValue(":username", m_username);
        q.bindValue(":password", m_password);
        q.bindValue(":isadmin", m_isadmin);
        q.exec();
        m_id = q.lastInsertId().toInt();
    } else {
        q.prepare(QLatin1String("UPDATE users SET name = :name, username = :username, password = :password, isadmin = :isadmin WHERE ID = :id"));
        q.bindValue(":name", m_name);
        q.bindValue(":username", m_username);
        q.bindValue(":password", m_password);
        q.bindValue(":isadmin", m_isadmin);
        q.bindValue(":id", m_id);
        q.exec();
    }
}

/**
 * Removes the user from the database.
 *
 * The user can only be deleted if it already existed in the database.
 * A user with a nonzero value of ID is assumed to already have existed in the database.
 * Once the user has been removed from the database, its ID is set to 0.
 * Calling remove() on a user with its ID = 0 will not do anything.
 * However, it is possible to setName() and save() to add a new user into the database.
**/
void User::remove()
{
    QSqlQuery q;

    if (m_id != 0) {
        q.prepare(QLatin1String("DELETE FROM users WHERE id = :id"));
        q.bindValue(":id", m_id);
        q.exec();
        m_id = 0;
    }
}

/**
 * Selects the user with the given id from the database.
 * Returns a User object.
**/
User User::getById(int userId)
{
    User user;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM users WHERE id = :id"));
    q.bindValue(":id", userId);
    q.exec();

    if (q.next()) {
        user.setId(q.value("id").toInt());
        user.setName(q.value("name").toString());
        user.setUsername(q.value("username").toString());
        user.setPassword(q.value("password").toString());
        user.setIsAdmin(q.value("isadmin").toBool());
    };

    return user;
}

/**
 * Selects the user with the given username from the database.
 * Returns a User object.
**/
User User::getByUsername(const QString &userUsername)
{
    User user;

    QSqlQuery q;
    q.prepare(QLatin1String("SELECT * FROM users WHERE username = :username"));
    q.bindValue(":username", userUsername);
    q.exec();

    if (q.next()) {
        user.setId(q.value("id").toInt());
        user.setName(q.value("name").toString());
        user.setUsername(q.value("username").toString());
        user.setPassword(q.value("password").toString());
        user.setIsAdmin(q.value("isadmin").toBool());
    };

    return user;
}

/**
 * Selects all users from the database.
 * Returns a QList of User objects.
**/
QList<User> User::getAll()
{
    QList<User> users;

    QSqlQuery q(QLatin1String("SELECT * FROM users"));
    q.exec();

    while (q.next()) {
        User user;
        user.setId(q.value("id").toInt());
        user.setName(q.value("name").toString());
        user.setUsername(q.value("username").toString());
        user.setPassword(q.value("password").toString());
        user.setIsAdmin(q.value("isadmin").toBool());
        users.append(user);
    }

    return users;
}
