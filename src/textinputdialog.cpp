#include "textinputdialog.h"
#include <QtWidgets>

/**
 * Constructs a dialog to edit some text.
**/
TextInputDialog::TextInputDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));

    lineEditLabel = new QLabel(tr("Name"));
    lineEdit = new QLineEdit;

    QPushButton *okButton = new QPushButton(QIcon(":/icons/ok.png"), tr("OK"));
    QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cancel.png"), tr("Cancel"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(lineEditLabel, lineEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}
