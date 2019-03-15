#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "product.h"
#include "category.h"
#include "purchase.h"
#include "groupsale.h"
#include "sale.h"
#include "user.h"
#include "shift.h"
#include <QMainWindow>
#include <QStringList>
#include <QList>

QT_BEGIN_NAMESPACE
class QAction;
class QTableWidget;
class QLineEdit;
class QPushButton;
class QSpinBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const User &user, QWidget *parent = 0);
    ~MainWindow();
    void groupSaleSelectionRefresh();
    void groupSaleRefresh();
    void saleSelectionRefresh();
    void saleRefresh();
    void purchaseSelectionRefresh();
    void purchaseRefresh();
    void productSelectionRefresh();
    void productRefresh();
    void categorySelectionRefresh();
    void categoryRefresh();
    void userSelectionRefresh();
    void userRefresh();
    void shiftSelectionRefresh();
    void shiftRefresh();
    void displayReceipt(int groupSaleId);
    int currentBalance;
    User currentUser;
    Shift currentShift;
    QString storeName;
    int defaultTax;
    int defaultDiscount;

private:
    void closeEvent(QCloseEvent *event);
    void displayError(const QString &message);
    void displayInfo(const QString &message);
    void createMenuAndActions();
    void readSettings();
    void writeSettings();
    void newSale();
    void importDatabase();
    void exportDatabase();
    void about();
    void groupSaleReceipt();
    void saleEdit();
    void saleRemove();
    void purchaseEdit();
    void purchaseRemove();
    void productAdd();
    void productEdit();
    void productRemove();
    void productPurchase();
    void categoryAdd();
    void categoryEdit();
    void categoryRemove();
    void userEdit();
    void userRemove();
    void storeNameEdit();
    void storeNameRefresh();
    void defaultDiscountEdit();
    void defaultTaxEdit();
    QAction *toggleMainToolbarAction;
    QAction *toggleStatusBarAction;
    QTableWidget *groupSaleTable;
    QLineEdit *groupSaleSearchEdit;
    QPushButton *groupSaleReceiptButton;
    QStringList groupSaleHeaderLabels;
    QList<GroupSale> groupSaleList;
    QTableWidget *saleTable;
    QLineEdit *saleSearchEdit;
    QPushButton *saleEditButton;
    QPushButton *saleRemoveButton;
    QStringList saleHeaderLabels;
    QList<Sale> saleList;
    QTableWidget *purchaseTable;
    QLineEdit *purchaseSearchEdit;
    QPushButton *purchaseEditButton;
    QPushButton *purchaseRemoveButton;
    QStringList purchaseHeaderLabels;
    QList<Purchase> purchaseList;
    QTableWidget *productTable;
    QLineEdit *productSearchEdit;
    QPushButton *productAddButton;
    QPushButton *productEditButton;
    QPushButton *productRemoveButton;
    QPushButton *productPurchaseButton;
    QStringList productHeaderLabels;
    QList<Product> productList;
    QTableWidget *categoryTable;
    QLineEdit *categorySearchEdit;
    QPushButton *categoryAddButton;
    QPushButton *categoryEditButton;
    QPushButton *categoryRemoveButton;
    QStringList categoryHeaderLabels;
    QList<Category> categoryList;
    QTableWidget *userTable;
    QLineEdit *userSearchEdit;
    QPushButton *userEditButton;
    QPushButton *userRemoveButton;
    QStringList userHeaderLabels;
    QList<User> userList;
    QTableWidget *shiftTable;
    QLineEdit *shiftSearchEdit;
    QStringList shiftHeaderLabels;
    QList<Shift> shiftList;
};

#endif // MAINWINDOW_H
