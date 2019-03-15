#include "webviewdialog.h"
#include <QtWidgets>
#include <QtPrintSupport>

/**
 * Constructs a dialog with a web view.
**/
WebViewDialog::WebViewDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));
    setMinimumSize(QSize(480, 360));

    webView = new QTextBrowser;

    QPushButton *okButton = new QPushButton(QIcon(":/icons/ok.png"), tr("OK"));
    QPushButton *printButton = new QPushButton(QIcon(":/icons/print.png"), tr("Print"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(printButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(webView);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(printButton, &QAbstractButton::clicked, this, &WebViewDialog::print);
}

/**
 * Prints the contents of the web view.
**/
void WebViewDialog::print()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        webView->print(&printer);
    }
}
