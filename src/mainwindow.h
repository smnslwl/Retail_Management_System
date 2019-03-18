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
    void displayReportProduct(int productId);
    void displayReportCategory(int categoryId);
    void displayReportUser(int userId);
    void displayReportShift(int shiftId);
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
    void groupSaleRemove();
    void groupSaleReceipt();
    void saleEdit();
    void saleRemove();
    void purchaseEdit();
    void purchaseRemove();
    void productAdd();
    void productEdit();
    void productRemove();
    void productPurchase();
    void productReport();
    void categoryAdd();
    void categoryEdit();
    void categoryRemove();
    void categoryReport();
    void userEdit();
    void userRemove();
    void userReport();
    void shiftReport();
    void storeNameEdit();
    void storeNameRefresh();
    void defaultDiscountEdit();
    void defaultTaxEdit();
    QAction *toggleMainToolbarAction;
    QAction *toggleStatusBarAction;
    QTableWidget *groupSaleTable;
    QLineEdit *groupSaleSearchEdit;
    QPushButton *groupSaleRemoveButton;
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
    QPushButton *productReportButton;
    QStringList productHeaderLabels;
    QList<Product> productList;
    QTableWidget *categoryTable;
    QLineEdit *categorySearchEdit;
    QPushButton *categoryAddButton;
    QPushButton *categoryEditButton;
    QPushButton *categoryRemoveButton;
    QPushButton *categoryReportButton;
    QStringList categoryHeaderLabels;
    QList<Category> categoryList;
    QTableWidget *userTable;
    QLineEdit *userSearchEdit;
    QPushButton *userEditButton;
    QPushButton *userRemoveButton;
    QPushButton *userReportButton;
    QStringList userHeaderLabels;
    QList<User> userList;
    QTableWidget *shiftTable;
    QLineEdit *shiftSearchEdit;
    QPushButton *shiftReportButton;
    QStringList shiftHeaderLabels;
    QList<Shift> shiftList;
};

#endif // MAINWINDOW_H
