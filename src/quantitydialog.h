#ifndef QUANTITYDIALOG_H
#define QUANTITYDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QSpinBox;
QT_END_NAMESPACE

class QuantityDialog : public QDialog
{
    Q_OBJECT

public:
    QuantityDialog(QWidget *parent = 0);
    QSpinBox *quantitySpinBox;
};

#endif // QUANTITYDIALOG_H
