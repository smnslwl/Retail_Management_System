#ifndef WEBVIEWDIALOG_H
#define WEBVIEWDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QTextBrowser;
QT_END_NAMESPACE

class WebViewDialog : public QDialog
{
    Q_OBJECT

public:
    WebViewDialog(QWidget *parent = 0);
    void print();

public:
    QTextBrowser *webView;
};

#endif // WEBVIEWDIALOG_H
