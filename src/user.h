#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

class User
{
public:
    User(const QString &userName = "", const QString &userUsername = "", const QString &userPassword = "", bool userIsAdmin = 0);
    ~User();
    int id() const;
    const QString &name() const;
    const QString &username() const;
    const QString &password() const;
    bool isAdmin() const;
    void setId(int userId);
    void setName(const QString &userName);
    void setUsername(const QString &userUsername);
    void setPassword(const QString &userPassword);
    void setIsAdmin(bool userIsAdmin);
    void save();
    void remove();

private:
    int m_id;
    QString m_name;
    QString m_username;
    QString m_password;
    int m_isadmin;

public:
    static const int columnCount = 5;
    static User getById(int userId);
    static User getByUsername(const QString &userUsername);
    static QList<User> getAll();
};

#endif // USER_H
