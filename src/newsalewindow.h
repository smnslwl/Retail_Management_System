#ifndef NEWSALEWINDOW_H
#define NEWSALEWINDOW_H

#include "mainwindow.h"
#include "product.h"
#include "sale.h"
#include <QMainWindow>
#include <QStringList>
#include <QList>
#include <QHash>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QSpinBox;
class QPushButton;
class QLabel;
class QTableWidget;
QT_END_NAMESPACE

class NewSaleWindow : public QMainWindow
{
    Q_OBJECT

public:
    NewSaleWindow(QWidget *parent = 0);

private:
    void closeEvent(QCloseEvent *event);
    void displayError(const QString &message);
    void displayInfo(const QString &message);
    void timeRefresh();
    void searchSelectionRefresh();
    void searchRefresh();
    void saleSelectionRefresh();
    void saleRefresh();
    void saleUpdateTotals();
    void add();
    void edit();
    void remove();
    void confirmSale();
    void clearSale();
    MainWindow *mainWindow;
    QLabel *timeLabel;
    QTableWidget *searchTable;
    QTableWidget *saleTable;
    QLineEdit *searchEdit;
    QLabel *productNameLabel;
    QLabel *productPriceLabel;
    QSpinBox *quantitySpinBox;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QPushButton *confirmButton;
    QPushButton *clearButton;
    QPushButton *exitButton;
    QLineEdit *customerNameEdit;
    QSpinBox *discountSpinBox;
    QSpinBox *taxSpinBox;
    QLabel *subtotalLabel;
    QLabel *totalLabel;    
    QStringList searchHeaderLabels;
    QStringList saleHeaderLabels;
    QList<Product> searchList;
    QList<Sale> saleList;
    QHash<int, int> productQuantities;
};

#endif // NEWSALEWINDOW_H
