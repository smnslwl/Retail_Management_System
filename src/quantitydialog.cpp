#include "quantitydialog.h"
#include <QtWidgets>

/**
 * Constructs a category add/edit dialog.
**/
QuantityDialog::QuantityDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));

    QLabel *quantityLabel = new QLabel(tr("Quantity"));
    QPushButton *okButton = new QPushButton(QIcon(":/icons/ok.png"), tr("OK"));
    QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cancel.png"), tr("Cancel"));

    quantitySpinBox = new QSpinBox;

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setColumnStretch(1, 2);
    gridLayout->addWidget(quantityLabel, 0, 0);
    gridLayout->addWidget(quantitySpinBox, 0, 1);
    gridLayout->addLayout(buttonLayout, 2, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}
