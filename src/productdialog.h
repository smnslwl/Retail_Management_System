#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QSpinBox;
class QComboBox;
QT_END_NAMESPACE

class ProductDialog : public QDialog
{
    Q_OBJECT

public:
    ProductDialog(QWidget *parent = 0);
    QLineEdit *nameLineEdit;
    QComboBox *categoryComboBox;
    QSpinBox *priceSpinBox;
};

#endif // PRODUCTDIALOG_H
