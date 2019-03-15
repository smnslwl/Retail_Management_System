#include "spinboxdialog.h"
#include <QtWidgets>

/**
 * Constructs a dialog to edit a spinbox.
**/
SpinBoxDialog::SpinBoxDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));

    spinBoxLabel = new QLabel(tr("Value"));
    spinBox = new QSpinBox;

    QPushButton *okButton = new QPushButton(QIcon(":/icons/ok.png"), tr("OK"));
    QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cancel.png"), tr("Cancel"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(spinBoxLabel, spinBox);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}
