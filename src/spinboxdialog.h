#ifndef SPINBOXDIALOG_H
#define SPINBOXDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QSpinBox;
QT_END_NAMESPACE

class SpinBoxDialog : public QDialog
{
    Q_OBJECT

public:
    SpinBoxDialog(QWidget *parent = 0);
    QSpinBox *spinBox;
    QLabel *spinBoxLabel;
};

#endif // SPINBOXDIALOG_H
