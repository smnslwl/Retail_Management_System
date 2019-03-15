#include "purchasedialog.h"
#include <QtWidgets>

/**
 * Constructs a purchase add dialog.
**/
PurchaseDialog::PurchaseDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));
    currentPrice = 0;

    QLabel *nameLabel = new QLabel(tr("Name"));
    QLabel *quantityLabel = new QLabel(tr("Quantity"));
    QLabel *amountLabel = new QLabel(tr("Amount"));
    QLabel *costLabel = new QLabel(tr("Unit cost"));
    QLabel *priceLabel = new QLabel(tr("Price"));
    QPushButton *okButton = new QPushButton(QIcon(":/icons/ok.png"), tr("OK"));
    QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cancel.png"), tr("Cancel"));

    nameLineEdit = new QLineEdit;
    nameLineEdit->setEnabled(false);
    quantitySpinBox = new QSpinBox;
    quantitySpinBox->setMinimum(1);
    quantitySpinBox->setMaximum(1000000);
    amountSpinBox = new QSpinBox;
    amountSpinBox->setMinimum(0);
    amountSpinBox->setMaximum(1000000);
    costSpinBox = new QDoubleSpinBox;
    costSpinBox->setMinimum(0);
    costSpinBox->setMaximum(1000000);
    costSpinBox->setDecimals(2);
    costSpinBox->setEnabled(false);
    modifyPriceCheckBox = new QCheckBox(tr("Modify price"));
    priceSpinBox = new QSpinBox;
    priceSpinBox->setMinimum(0);
    priceSpinBox->setMaximum(1000000);
    priceSpinBox->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setColumnStretch(1, 2);
    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameLineEdit, 0, 1);
    gridLayout->addWidget(quantityLabel, 1, 0);
    gridLayout->addWidget(quantitySpinBox, 1, 1);
    gridLayout->addWidget(amountLabel, 2, 0);
    gridLayout->addWidget(amountSpinBox, 2, 1);
    gridLayout->addWidget(costLabel, 3, 0);
    gridLayout->addWidget(costSpinBox, 3, 1);
    gridLayout->addWidget(modifyPriceCheckBox, 4, 1);
    gridLayout->addWidget(priceLabel, 5, 0);
    gridLayout->addWidget(priceSpinBox, 5, 1);
    gridLayout->addLayout(buttonLayout, 6, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    // See <https://wiki.qt.io/New_Signal_Slot_Syntax#Overload> for more information
    connect(quantitySpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PurchaseDialog::purchaseChanged);
    connect(amountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PurchaseDialog::purchaseChanged);
    connect(modifyPriceCheckBox, &QCheckBox::toggled, this, &PurchaseDialog::modifyPriceToggled);
    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}

void PurchaseDialog::purchaseChanged()
{
    int qty = quantitySpinBox->value();
    int total = amountSpinBox->value();
    double unitCostPrice = (double) total / qty;
    costSpinBox->setValue(unitCostPrice);
}

void PurchaseDialog::modifyPriceToggled()
{
    if (modifyPriceCheckBox->isChecked()) {
        currentPrice = priceSpinBox->value();
        priceSpinBox->setEnabled(true);
    } else {
        priceSpinBox->setEnabled(false);
        priceSpinBox->setValue(currentPrice);
    }
}
