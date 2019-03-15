#include "userdialog.h"
#include <QtWidgets>

/**
 * Constructs a user add/edit dialog.
**/
UserDialog::UserDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));

    QLabel *nameLabel = new QLabel(tr("Name"));
    QPushButton *okButton = new QPushButton(QIcon(":/icons/ok.png"), tr("OK"));
    QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cancel.png"), tr("Cancel"));

    nameLineEdit = new QLineEdit;
    isAdminCheckBox = new QCheckBox(tr("Administrator account"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setColumnStretch(1, 2);
    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameLineEdit, 0, 1);
    gridLayout->addWidget(isAdminCheckBox, 1, 1);
    gridLayout->addLayout(buttonLayout, 3, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}
