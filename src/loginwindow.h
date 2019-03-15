#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
class QRadioButton;
class QTabWidget;
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = 0);

private:
    void displayError(const QString &message);
    void displayInfo(const QString &message);
    void connectToDatabase();
    void login();
    void signup();
    bool hasNoUserAccounts;
    QLineEdit *loginUsernameEdit;
    QLineEdit *loginPasswordEdit;
    QPushButton *loginButton;
    QPushButton *loginExitButton;
    QLineEdit *signupNameEdit;
    QLineEdit *signupUsernameEdit;
    QLineEdit *signupPasswordEdit;
    QLineEdit *signupConfirmPasswordEdit;
    QPushButton *signupButton;
    QPushButton *signupExitButton;
    QTabWidget *tabWidget;
};

#endif // LOGINWINDOW_H
