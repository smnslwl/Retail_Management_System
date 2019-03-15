#ifndef TEXTINPUTDIALOG_H
#define TEXTINPUTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class TextInputDialog : public QDialog
{
    Q_OBJECT

public:
    TextInputDialog(QWidget *parent = 0);
    QLineEdit *lineEdit;
    QLabel *lineEditLabel;
};

#endif // TEXTINPUTDIALOG_H
