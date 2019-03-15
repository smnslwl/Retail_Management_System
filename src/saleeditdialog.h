#ifndef SALEEDITDIALOG_H
#define SALEEDITDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE

class SaleEditDialog : public QDialog
{
    Q_OBJECT

public:
    SaleEditDialog(QWidget *parent = 0);
    QLineEdit *productLineEdit;
    QSpinBox *quantitySpinBox;
    QSpinBox *amountSpinBox;
    QCheckBox *updateDateCheckBox;
};

#endif // SALEEDITDIALOG_H
