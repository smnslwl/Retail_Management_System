#include "changepassworddialog.h"
#include <QtWidgets>

/**
 * Constructs a change password dialog.
**/
ChangePasswordDialog::ChangePasswordDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));

    QLabel *oldPasswordLabel = new QLabel(tr("Current Password"));
    QLabel *newPassword1Label = new QLabel(tr("New Password"));
    QLabel *newPassword2Label = new QLabel(tr("Confirm Password"));
    QPushButton *okButton = new QPushButton(QIcon(":/icons/ok.png"), tr("OK"));
    QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cancel.png"), tr("Cancel"));

    oldPassword = new QLineEdit;
    oldPassword->setEchoMode(QLineEdit::Password);
    newPassword1 = new QLineEdit;
    newPassword1->setEchoMode(QLineEdit::Password);
    newPassword2 = new QLineEdit;
    newPassword2->setEchoMode(QLineEdit::Password);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setColumnStretch(1, 2);
    gridLayout->addWidget(oldPasswordLabel, 0, 0);
    gridLayout->addWidget(oldPassword, 0, 1);
    gridLayout->addWidget(newPassword1Label, 1, 0);
    gridLayout->addWidget(newPassword1, 1, 1);
    gridLayout->addWidget(newPassword2Label, 2, 0);
    gridLayout->addWidget(newPassword2, 2, 1);
    gridLayout->addLayout(buttonLayout, 4, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
}
