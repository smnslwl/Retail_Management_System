#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QCheckBox;
QT_END_NAMESPACE

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    UserDialog(QWidget *parent = 0);
    QLineEdit *nameLineEdit;
    QCheckBox *isAdminCheckBox;
};

#endif // USERDIALOG_H
