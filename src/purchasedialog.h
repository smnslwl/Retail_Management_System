#ifndef PURCHASEDIALOG_H
#define PURCHASEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
QT_END_NAMESPACE

class PurchaseDialog : public QDialog
{
    Q_OBJECT

public:
    PurchaseDialog(QWidget *parent = 0);
    QLineEdit *nameLineEdit;
    QSpinBox *quantitySpinBox;
    QSpinBox *amountSpinBox;
    QDoubleSpinBox *costSpinBox;
    QCheckBox *modifyPriceCheckBox;
    QSpinBox *priceSpinBox;

private:
    int currentPrice;
    void purchaseChanged();
    void modifyPriceToggled();
};

#endif // PURCHASEDIALOG_H
