#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    ConfirmDialog(QWidget *parent = 0);
    QLabel *message;
};

#endif // CONFIRMDIALOG_H
