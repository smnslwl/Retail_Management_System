#ifndef PURCHASEEDITDIALOG_H
#define PURCHASEEDITDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE

class PurchaseEditDialog : public QDialog
{
    Q_OBJECT

public:
    PurchaseEditDialog(QWidget *parent = 0);
    QLineEdit *productLineEdit;
    QSpinBox *quantitySpinBox;
    QSpinBox *amountSpinBox;
    QCheckBox *updateDateCheckBox;
};

#endif // PURCHASEEDITDIALOG_H
