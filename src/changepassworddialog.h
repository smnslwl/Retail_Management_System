#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    ChangePasswordDialog(QWidget *parent = 0);
    QLineEdit *oldPassword;
    QLineEdit *newPassword1;
    QLineEdit *newPassword2;
};

#endif // CHANGEPASSWORDDIALOG_H
