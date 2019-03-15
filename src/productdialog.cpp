#include "productdialog.h"
#include <QtWidgets>

/**
 * Constructs a product add/edit dialog.
**/
ProductDialog::ProductDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));

    QLabel *nameLabel = new QLabel(tr("Name"));
    QLabel *categoryLabel = new QLabel(tr("Category"));
    QLabel *priceLabel = new QLabel(tr("Price"));
    QPushButton *okButton = new QPushButton(QIcon(":/icons/ok.png"), tr("OK"));
    QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cancel.png"), tr("Cancel"));

    nameLineEdit = new QLineEdit;
    categoryComboBox = new QComboBox;
    priceSpinBox = new QSpinBox;
    priceSpinBox->setMinimum(0);
    priceSpinBox->setMaximum(1000000);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setColumnStretch(1, 2);
    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameLineEdit, 0, 1);
    gridLayout->addWidget(categoryLabel, 1, 0);
    gridLayout->addWidget(categoryComboBox, 1, 1);
    gridLayout->addWidget(priceLabel, 2, 0);
    gridLayout->addWidget(priceSpinBox, 2, 1);
    gridLayout->addLayout(buttonLayout, 3, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}
