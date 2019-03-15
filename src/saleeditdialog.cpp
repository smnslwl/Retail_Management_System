#include "saleeditdialog.h"
#include <QtWidgets>

/**
 * Constructs a sale edit dialog.
**/
SaleEditDialog::SaleEditDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));

    QLabel *productLabel = new QLabel(tr("Product"));
    QLabel *quantityLabel = new QLabel(tr("Quantity"));
    QLabel *amountLabel = new QLabel(tr("Amount"));
    QPushButton *okButton = new QPushButton(QIcon(":/icons/ok.png"), tr("OK"));
    QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cancel.png"), tr("Cancel"));

    productLineEdit = new QLineEdit;
    productLineEdit->setEnabled(false);
    quantitySpinBox = new QSpinBox;
    quantitySpinBox->setMinimum(0);
    quantitySpinBox->setMaximum(1000000);
    amountSpinBox = new QSpinBox;
    amountSpinBox->setMinimum(0);
    amountSpinBox->setMaximum(1000000);
    updateDateCheckBox = new QCheckBox(tr("Update date"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setColumnStretch(1, 2);
    gridLayout->addWidget(productLabel, 0, 0);
    gridLayout->addWidget(productLineEdit, 0, 1);
    gridLayout->addWidget(quantityLabel, 1, 0);
    gridLayout->addWidget(quantitySpinBox, 1, 1);
    gridLayout->addWidget(amountLabel, 2, 0);
    gridLayout->addWidget(amountSpinBox, 2, 1);
    gridLayout->addWidget(updateDateCheckBox, 3, 1);
    gridLayout->addLayout(buttonLayout, 4, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}
