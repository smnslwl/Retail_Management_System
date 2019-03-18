#include "mainwindow.h"
#include "newsalewindow.h"
#include "category.h"
#include "product.h"
#include "purchase.h"
#include "groupsale.h"
#include "sale.h"
#include "user.h"
#include "spinboxdialog.h"
#include "textinputdialog.h"
#include "productdialog.h"
#include "purchasedialog.h"
#include "purchaseeditdialog.h"
#include "saleeditdialog.h"
#include "userdialog.h"
#include "webviewdialog.h"
#include <QtWidgets>
#include <QtSql>

/**
 * Defines how long a temporary message will be shown in the status bar.
**/
#define STATUSBAR_MS 2000

/**
 * Constructs a new MainWindow and performs initial window setup.
**/
MainWindow::MainWindow(const User &user, QWidget *parent)
    : QMainWindow(parent)
{
    /******************************************************************
                                INITIAL SETUP
    ******************************************************************/

    currentUser = user;
    currentShift.setCashier(user.id());
    QDateTime dt;
    dt.setTime_t(QDateTime::currentSecsSinceEpoch());
    currentShift.setStartDate(dt.toString("yyyy-MM-dd hh:mm:ss"));
    currentShift.save();

    setWindowIcon(QIcon(":/icons/store.png"));
    setMinimumSize(720, 480);
    createMenuAndActions();

    /******************************************************************
                                GROUPSALES
    ******************************************************************/

    groupSaleSearchEdit = new QLineEdit;
    groupSaleSearchEdit->setPlaceholderText(tr("Search by Customer or Cashier"));
    groupSaleRemoveButton = new QPushButton(QIcon(":/icons/remove.png"), tr("Remove"));
    groupSaleReceiptButton = new QPushButton(QIcon(":/icons/receipt.png"), tr("Receipt..."));

    groupSaleTable = new QTableWidget;
    groupSaleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable in-place editing
    groupSaleTable->setSelectionBehavior(QAbstractItemView::SelectRows);  // only rows can be selected, not columns or sells
    groupSaleTable->setSelectionMode(QAbstractItemView::SingleSelection);  // disable selection of multiple rows
    groupSaleTable->setColumnCount(GroupSale::columnCount);  // assign the number of columns in the table
    groupSaleList.clear();
    groupSaleHeaderLabels << tr("ID") << tr("Customer") << tr("Subtotal") << tr("Discount") << tr("Tax") << tr("Total") << tr("Cashier") << tr("Date");
    groupSaleTable->setHorizontalHeaderLabels(groupSaleHeaderLabels);
    groupSaleTable->hideColumn(0);  // hide the ID column
    groupSaleSelectionRefresh();
    groupSaleRefresh();

    QHBoxLayout *groupSaleButtonLayout = new QHBoxLayout;
    groupSaleButtonLayout->setAlignment(Qt::AlignLeft);
    groupSaleButtonLayout->addWidget(groupSaleRemoveButton);
    groupSaleButtonLayout->addWidget(groupSaleReceiptButton);

    QGroupBox *groupSaleGroup = new QGroupBox;
    QVBoxLayout *groupSaleLayout = new QVBoxLayout;
    groupSaleLayout->addWidget(groupSaleSearchEdit);
    groupSaleLayout->addWidget(groupSaleTable);
    groupSaleLayout->addLayout(groupSaleButtonLayout);
    groupSaleGroup->setLayout(groupSaleLayout);

    connect(groupSaleRemoveButton, &QPushButton::clicked, this, &MainWindow::groupSaleRemove);
    connect(groupSaleReceiptButton, &QPushButton::clicked, this, &MainWindow::groupSaleReceipt);
    connect(groupSaleTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::groupSaleSelectionRefresh);
    connect(groupSaleSearchEdit, &QLineEdit::textEdited, this, &MainWindow::groupSaleRefresh);

    /******************************************************************
                                SALES
    ******************************************************************/

    saleSearchEdit = new QLineEdit;
    saleSearchEdit->setPlaceholderText(tr("Search by Product"));
    saleEditButton = new QPushButton(QIcon(":/icons/edit.png"), tr("Edit..."));
    saleRemoveButton = new QPushButton(QIcon(":/icons/remove.png"), tr("Remove"));

    saleTable = new QTableWidget;
    saleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable in-place editing
    saleTable->setSelectionBehavior(QAbstractItemView::SelectRows);  // only rows can be selected, not columns or sells
    saleTable->setSelectionMode(QAbstractItemView::SingleSelection);  // disable selection of multiple rows
    saleTable->setColumnCount(Sale::columnCount);  // assign the number of columns in the table
    saleHeaderLabels.clear();
    saleHeaderLabels << tr("ID") << tr("Product") << tr("Quantity") << tr("Amount") << tr("Group") << tr("Index");
    saleTable->setHorizontalHeaderLabels(saleHeaderLabels);
    saleTable->hideColumn(0);  // hide the ID column
    saleSelectionRefresh();
    saleRefresh();

    QHBoxLayout *saleButtonLayout = new QHBoxLayout;
    saleButtonLayout->setAlignment(Qt::AlignLeft);
    saleButtonLayout->addWidget(saleEditButton);
    saleButtonLayout->addWidget(saleRemoveButton);

    QGroupBox *saleGroup = new QGroupBox;
    QVBoxLayout *saleLayout = new QVBoxLayout;
    saleLayout->addWidget(saleSearchEdit);
    saleLayout->addWidget(saleTable);
    saleLayout->addLayout(saleButtonLayout);
    saleGroup->setLayout(saleLayout);

    connect(saleEditButton, &QPushButton::clicked, this, &MainWindow::saleEdit);
    connect(saleRemoveButton, &QPushButton::clicked, this, &MainWindow::saleRemove);
    connect(saleTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::saleSelectionRefresh);
    connect(saleSearchEdit, &QLineEdit::textEdited, this, &MainWindow::saleRefresh);

    /******************************************************************
                                PURCHASES
    ******************************************************************/

    purchaseSearchEdit = new QLineEdit;
    purchaseSearchEdit->setPlaceholderText(tr("Search by Product"));
    purchaseEditButton = new QPushButton(QIcon(":/icons/edit.png"), tr("Edit..."));
    purchaseRemoveButton = new QPushButton(QIcon(":/icons/remove.png"), tr("Remove"));

    purchaseTable = new QTableWidget;
    purchaseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable in-place editing
    purchaseTable->setSelectionBehavior(QAbstractItemView::SelectRows);  // only rows can be selected, not columns or sells
    purchaseTable->setSelectionMode(QAbstractItemView::SingleSelection);  // disable selection of multiple rows
    purchaseTable->setColumnCount(Purchase::columnCount);  // assign the number of columns in the table
    purchaseHeaderLabels.clear();
    purchaseHeaderLabels << tr("ID") << tr("Product") << tr("Quantity") << tr("Amount") << tr("Date");
    purchaseTable->setHorizontalHeaderLabels(purchaseHeaderLabels);
    purchaseTable->hideColumn(0);  // hide the ID column
    purchaseSelectionRefresh();
    purchaseRefresh();

    QHBoxLayout *purchaseButtonLayout = new QHBoxLayout;
    purchaseButtonLayout->setAlignment(Qt::AlignLeft);
    purchaseButtonLayout->addWidget(purchaseEditButton);
    purchaseButtonLayout->addWidget(purchaseRemoveButton);

    QGroupBox *purchaseGroup = new QGroupBox;
    QVBoxLayout *purchaseLayout = new QVBoxLayout;
    purchaseLayout->addWidget(purchaseSearchEdit);
    purchaseLayout->addWidget(purchaseTable);
    purchaseLayout->addLayout(purchaseButtonLayout);
    purchaseGroup->setLayout(purchaseLayout);

    connect(purchaseEditButton, &QPushButton::clicked, this, &MainWindow::purchaseEdit);
    connect(purchaseRemoveButton, &QPushButton::clicked, this, &MainWindow::purchaseRemove);
    connect(purchaseTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::purchaseSelectionRefresh);
    connect(purchaseSearchEdit, &QLineEdit::textEdited, this, &MainWindow::purchaseRefresh);

    /******************************************************************
                                PRODUCTS
    ******************************************************************/

    productSearchEdit = new QLineEdit;
    productSearchEdit->setPlaceholderText(tr("Search by Name"));
    productAddButton = new QPushButton(QIcon(":/icons/add.png"), tr("Add..."));
    productEditButton = new QPushButton(QIcon(":/icons/edit.png"), tr("Edit..."));
    productRemoveButton = new QPushButton(QIcon(":/icons/remove.png"), tr("Remove"));
    productPurchaseButton = new QPushButton(QIcon(":/icons/dollar.png"), tr("Purchase..."));
    productReportButton = new QPushButton(QIcon(":/icons/manual.png"), tr("Report..."));

    productTable = new QTableWidget;
    productTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable in-place editing
    productTable->setSelectionBehavior(QAbstractItemView::SelectRows);  // only rows can be selected, not columns or sells
    productTable->setSelectionMode(QAbstractItemView::SingleSelection);  // disable selection of multiple rows
    productTable->setColumnCount(Product::columnCount);  // assign the number of columns in the table
    productHeaderLabels.clear();
    productHeaderLabels << tr("ID") << tr("Name") << tr("Category") << tr("Price") << tr("Stock");
    productTable->setHorizontalHeaderLabels(productHeaderLabels);
    productTable->hideColumn(0);  // hide the ID column
    productSelectionRefresh();
    productRefresh();

    QHBoxLayout *productButtonLayout = new QHBoxLayout;
    productButtonLayout->setAlignment(Qt::AlignLeft);
    productButtonLayout->addWidget(productAddButton);
    productButtonLayout->addWidget(productEditButton);
    productButtonLayout->addWidget(productRemoveButton);
    productButtonLayout->addWidget(productPurchaseButton);
    productButtonLayout->addWidget(productReportButton);

    QGroupBox *productGroup = new QGroupBox;
    QVBoxLayout *productLayout = new QVBoxLayout;
    productLayout->addWidget(productSearchEdit);
    productLayout->addWidget(productTable);
    productLayout->addLayout(productButtonLayout);
    productGroup->setLayout(productLayout);

    connect(productAddButton, &QPushButton::clicked, this, &MainWindow::productAdd);
    connect(productEditButton, &QPushButton::clicked, this, &MainWindow::productEdit);
    connect(productRemoveButton, &QPushButton::clicked, this, &MainWindow::productRemove);
    connect(productPurchaseButton, &QPushButton::clicked, this, &MainWindow::productPurchase);
    connect(productReportButton, &QPushButton::clicked, this, &MainWindow::productReport);
    connect(productTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::productSelectionRefresh);
    connect(productSearchEdit, &QLineEdit::textEdited, this, &MainWindow::productRefresh);

    /******************************************************************
                                CATEGORIES
    ******************************************************************/

    categorySearchEdit = new QLineEdit;
    categorySearchEdit->setPlaceholderText(tr("Search by Name"));
    categoryAddButton = new QPushButton(QIcon(":/icons/add.png"), tr("Add..."));
    categoryEditButton = new QPushButton(QIcon(":/icons/edit.png"), tr("Edit..."));
    categoryRemoveButton = new QPushButton(QIcon(":/icons/remove.png"), tr("Remove"));
    categoryReportButton = new QPushButton(QIcon(":/icons/manual.png"), tr("Report..."));

    categoryTable = new QTableWidget;
    categoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable in-place editing
    categoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);  // only rows can be selected, not columns or sells
    categoryTable->setSelectionMode(QAbstractItemView::SingleSelection);  // disable selection of multiple rows
    categoryTable->setColumnCount(Category::columnCount);  // assign the number of columns in the table
    categoryHeaderLabels.clear();
    categoryHeaderLabels << tr("ID") << tr("Name");
    categoryTable->setHorizontalHeaderLabels(categoryHeaderLabels);
    categoryTable->hideColumn(0);  // hide the ID column
    categorySelectionRefresh();
    categoryRefresh();

    QHBoxLayout *categoryButtonLayout = new QHBoxLayout;
    categoryButtonLayout->setAlignment(Qt::AlignLeft);
    categoryButtonLayout->addWidget(categoryAddButton);
    categoryButtonLayout->addWidget(categoryEditButton);
    categoryButtonLayout->addWidget(categoryRemoveButton);
    categoryButtonLayout->addWidget(categoryReportButton);

    QGroupBox *categoryGroup = new QGroupBox;
    QVBoxLayout *categoryLayout = new QVBoxLayout;
    categoryLayout->addWidget(categorySearchEdit);
    categoryLayout->addWidget(categoryTable);
    categoryLayout->addLayout(categoryButtonLayout);
    categoryGroup->setLayout(categoryLayout);

    connect(categoryAddButton, &QPushButton::clicked, this, &MainWindow::categoryAdd);
    connect(categoryEditButton, &QPushButton::clicked, this, &MainWindow::categoryEdit);
    connect(categoryRemoveButton, &QPushButton::clicked, this, &MainWindow::categoryRemove);
    connect(categoryReportButton, &QPushButton::clicked, this, &MainWindow::categoryReport);
    connect(categoryTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::categorySelectionRefresh);
    connect(categorySearchEdit, &QLineEdit::textEdited, this, &MainWindow::categoryRefresh);

    /******************************************************************
                                USERS
    ******************************************************************/

    userSearchEdit = new QLineEdit;
    userSearchEdit->setPlaceholderText(tr("Search by Name or Username"));
    userEditButton = new QPushButton(QIcon(":/icons/edit.png"), tr("Edit..."));
    userRemoveButton = new QPushButton(QIcon(":/icons/remove.png"), tr("Remove"));
    userReportButton = new QPushButton(QIcon(":/icons/manual.png"), tr("Report..."));

    userTable = new QTableWidget;
    userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable in-place editing
    userTable->setSelectionBehavior(QAbstractItemView::SelectRows);  // only rows can be selected, not columns or sells
    userTable->setSelectionMode(QAbstractItemView::SingleSelection);  // disable selection of multiple rows
    userTable->setColumnCount(User::columnCount);  // assign the number of columns in the table
    userHeaderLabels.clear();
    userHeaderLabels << tr("ID") << tr("Name") << tr("Username") << tr("Password") << tr("Is Admin");
    userTable->setHorizontalHeaderLabels(userHeaderLabels);
    userTable->hideColumn(0);  // hide the ID column
    userTable->hideColumn(3);  // hide the Password column
    userSelectionRefresh();
    userRefresh();

    QHBoxLayout *userButtonLayout = new QHBoxLayout;
    userButtonLayout->setAlignment(Qt::AlignLeft);
    userButtonLayout->addWidget(userEditButton);
    userButtonLayout->addWidget(userRemoveButton);
    userButtonLayout->addWidget(userReportButton);

    QGroupBox *userGroup = new QGroupBox;
    QVBoxLayout *userLayout = new QVBoxLayout;
    userLayout->addWidget(userSearchEdit);
    userLayout->addWidget(userTable);
    userLayout->addLayout(userButtonLayout);
    userGroup->setLayout(userLayout);

    connect(userEditButton, &QPushButton::clicked, this, &MainWindow::userEdit);
    connect(userRemoveButton, &QPushButton::clicked, this, &MainWindow::userRemove);
    connect(userReportButton, &QPushButton::clicked, this, &MainWindow::userReport);
    connect(userTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::userSelectionRefresh);
    connect(userSearchEdit, &QLineEdit::textEdited, this, &MainWindow::userRefresh);

    /******************************************************************
                                SHIFT
    ******************************************************************/

    shiftSearchEdit = new QLineEdit;
    shiftSearchEdit->setPlaceholderText(tr("Search by Name"));
    shiftReportButton = new QPushButton(QIcon(":/icons/manual.png"), tr("Report..."));

    shiftTable = new QTableWidget;
    shiftTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable in-place editing
    shiftTable->setSelectionBehavior(QAbstractItemView::SelectRows);  // only rows can be selected, not columns or sells
    shiftTable->setSelectionMode(QAbstractItemView::SingleSelection);  // disable selection of multiple rows
    shiftTable->setColumnCount(Shift::columnCount);  // assign the number of columns in the table
    shiftHeaderLabels.clear();
    shiftHeaderLabels << tr("ID") << tr("Name") << tr("Start Date") << tr("End Date") << tr("Start Balance") << tr("End Balance") << tr("Net Balance");
    shiftTable->setHorizontalHeaderLabels(shiftHeaderLabels);
    shiftTable->hideColumn(0);  // hide the ID column
    shiftSelectionRefresh();
    shiftRefresh();

    QHBoxLayout *shiftButtonLayout = new QHBoxLayout;
    shiftButtonLayout->setAlignment(Qt::AlignLeft);
    shiftButtonLayout->addWidget(shiftReportButton);

    QGroupBox *shiftGroup = new QGroupBox;
    QVBoxLayout *shiftLayout = new QVBoxLayout;
    shiftLayout->addWidget(shiftSearchEdit);
    shiftLayout->addWidget(shiftTable);
    shiftLayout->addLayout(shiftButtonLayout);
    shiftGroup->setLayout(shiftLayout);

    connect(shiftReportButton, &QPushButton::clicked, this, &MainWindow::shiftReport);
    connect(shiftTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::shiftSelectionRefresh);
    connect(shiftSearchEdit, &QLineEdit::textEdited, this, &MainWindow::shiftRefresh);

    /******************************************************************
                                FINAL SETUP
    ******************************************************************/

    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->setIconSize(QSize(24, 24));
    tabWidget->addTab(groupSaleGroup, QIcon(":/icons/cash_register.png"), tr("Group Sales"));
    tabWidget->addTab(saleGroup, QIcon(":/icons/trolley.png"), tr("Sales"));
    tabWidget->addTab(purchaseGroup, QIcon(":/icons/dollar.png"), tr("Purchases"));
    tabWidget->addTab(productGroup, QIcon(":/icons/product.png"), tr("Products"));
    tabWidget->addTab(categoryGroup, QIcon(":/icons/category.png"), tr("Categories"));
    tabWidget->addTab(userGroup, QIcon(":/icons/users.png"), tr("Users"));
    tabWidget->addTab(shiftGroup, QIcon(":/icons/user_menu.png"), tr("Shifts"));
    setCentralWidget(tabWidget);

    QDate date = QDate::currentDate();
    QString dateString = QLocale().toString(date);
    QLabel *dateLabel = new QLabel;
    dateLabel->setText(dateString);
    statusBar()->addPermanentWidget(dateLabel);

    readSettings();
    statusBar()->showMessage(tr("Ready."), STATUSBAR_MS);
}

/**
 * Destroys all resources.
**/
MainWindow::~MainWindow()
{
}

/**
 * Performs some actions when the window close button is clicked.
**/
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox box;
    box.setWindowTitle(tr("Are you sure?"));
    box.setWindowIcon(QIcon(":/icons/store.png"));
    box.setIcon(QMessageBox::Question);
    box.setText(tr("Do you really want to quit?"));
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setDefaultButton(QMessageBox::Yes);
    box.button(QMessageBox::Yes)->setText(tr("Yes"));
    box.button(QMessageBox::Yes)->setIcon(QIcon(":/icons/ok.png"));
    box.button(QMessageBox::No)->setText(tr("No"));
    box.button(QMessageBox::No)->setIcon(QIcon(":/icons/cancel.png"));

    int reply = box.exec();
    if (reply == QMessageBox::Yes) {
        QDateTime dt;
        dt.setTime_t(QDateTime::currentSecsSinceEpoch());
        currentShift.setEndDate(dt.toString("yyyy-MM-dd hh:mm:ss"));
        currentShift.save();
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

/**
 * Displays a message box with information about an error.
**/
void MainWindow::displayError(const QString &message)
{
    QMessageBox box;
    box.setWindowTitle(tr("Error"));
    box.setWindowIcon(QIcon(":/icons/store.png"));
    box.setIcon(QMessageBox::Critical);
    box.setText(message);
    box.setStandardButtons(QMessageBox::Ok);
    box.setDefaultButton(QMessageBox::Ok);
    box.button(QMessageBox::Ok)->setText(tr("OK"));
    box.button(QMessageBox::Ok)->setIcon(QIcon(":/icons/ok.png"));
    box.exec();
}

/**
 * Displays a message box with some information.
**/
void MainWindow::displayInfo(const QString &message)
{
    QMessageBox box;
    box.setWindowTitle(tr("Information"));
    box.setWindowIcon(QIcon(":/icons/store.png"));
    box.setIcon(QMessageBox::Information);
    box.setText(message);
    box.setStandardButtons(QMessageBox::Ok);
    box.setDefaultButton(QMessageBox::Ok);
    box.button(QMessageBox::Ok)->setText(tr("OK"));
    box.button(QMessageBox::Ok)->setIcon(QIcon(":/icons/ok.png"));
    box.exec();
}

/**
 * Creates actions and populates the menu bar.
**/
void MainWindow::createMenuAndActions()
{
    QAction *newSaleAction = new QAction(QIcon(":/icons/pos.png"), tr("&New sale..."), this);
    newSaleAction->setShortcut(QKeySequence("Ctrl+N"));
    QAction *importDatabaseAction = new QAction(QIcon(":/icons/import.png"), tr("&Import from file..."), this);
    QAction *exportDatabaseAction = new QAction(QIcon(":/icons/export.png"), tr("&Export to file..."), this);
    QAction *exitAction = new QAction(QIcon(":/icons/exit.png"), tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    toggleStatusBarAction = new QAction(tr("Status Bar"), this);
    toggleStatusBarAction->setCheckable(true);
    toggleStatusBarAction->setChecked(true);
    toggleMainToolbarAction = new QAction(tr("Main Toolbar"), this);
    toggleMainToolbarAction->setCheckable(true);
    toggleMainToolbarAction->setChecked(true);
    QAction *editStoreNameAction = new QAction(tr("Edit store &name..."), this);
    QAction *editDefaultDiscountAction = new QAction(tr("Edit default &discount..."), this);
    QAction *editDefaultTaxAction = new QAction(tr("Edit default &tax..."), this);
    QAction *aboutAction = new QAction(QIcon(":/icons/about.png"), tr("&About..."), this);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    QMenu *manageMenu = menuBar()->addMenu(tr("&Manage"));
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    fileMenu->addAction(newSaleAction);
    fileMenu->addSeparator();
    fileMenu->addAction(importDatabaseAction);
    fileMenu->addAction(exportDatabaseAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    viewMenu->addAction(toggleMainToolbarAction);
    viewMenu->addAction(toggleStatusBarAction);
    manageMenu->addAction(editStoreNameAction);
    manageMenu->addAction(editDefaultDiscountAction);
    manageMenu->addAction(editDefaultTaxAction);
    helpMenu->addAction(aboutAction);

    QToolBar *mainToolBar = addToolBar(tr("Main Toolbar"));
    mainToolBar->addAction(newSaleAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(importDatabaseAction);
    mainToolBar->addAction(exportDatabaseAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(aboutAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(exitAction);
    mainToolBar->setFloatable(false);
    mainToolBar->setMovable(false);
    mainToolBar->setIconSize(QSize(24, 24));
    mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    connect(newSaleAction, &QAction::triggered, this, &MainWindow::newSale);
    connect(importDatabaseAction, &QAction::triggered, this, &MainWindow::importDatabase);
    connect(exportDatabaseAction, &QAction::triggered, this, &MainWindow::exportDatabase);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(toggleMainToolbarAction, &QAction::toggled, mainToolBar, &QStatusBar::setVisible);
    connect(toggleStatusBarAction, &QAction::toggled, statusBar(), &QStatusBar::setVisible);
    connect(editStoreNameAction, &QAction::triggered, this, &MainWindow::storeNameEdit);
    connect(editDefaultDiscountAction, &QAction::triggered, this, &MainWindow::defaultDiscountEdit);
    connect(editDefaultTaxAction, &QAction::triggered, this, &MainWindow::defaultTaxEdit);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}

/**
 * Read settings from file.
**/
void MainWindow::readSettings()
{
    QSettings settings("Retail_Management_System");
    settings.beginGroup("Retail_Management_System");
    restoreGeometry(settings.value("windowGeometry").toByteArray());
    toggleMainToolbarAction->setChecked(settings.value("mainToolBarVisible").toBool());
    toggleStatusBarAction->setChecked(settings.value("statusBarVisible").toBool());
    storeName = settings.value("storeName", "My Store").toString();
    defaultDiscount = settings.value("defaultDiscount", 5).toInt();
    defaultTax = settings.value("defaultTax", 10).toInt();
    settings.endGroup();
    storeNameRefresh();
}

/**
 * Write settings to file.
**/
void MainWindow::writeSettings()
{
    QSettings settings("Retail_Management_System");
    settings.beginGroup("Retail_Management_System");
    settings.setValue("windowGeometry", saveGeometry());
    settings.setValue("mainToolBarVisible", toggleMainToolbarAction->isChecked());
    settings.setValue("statusBarVisible", toggleStatusBarAction->isChecked());
    settings.setValue("storeName", storeName);
    settings.setValue("defaultDiscount", defaultDiscount);
    settings.setValue("defaultTax", defaultTax);
    settings.endGroup();
}

/**
 * Displays the new sale window.
**/
void MainWindow::newSale()
{
    NewSaleWindow *win = new NewSaleWindow(this);
    win->showFullScreen();
}

/**
 * Imports data into the database from an external file.
 *
 * QSqlQuery cannot execute multiple SQL statements at once.
 * So we read the statements line by line and execute them one at a time.
**/
void MainWindow::importDatabase()
{
    QMessageBox box;
    box.setWindowTitle(tr("Are you sure?"));
    box.setWindowIcon(QIcon(":/icons/store.png"));
    box.setIcon(QMessageBox::Question);
    box.setText(tr("Importing into the database will delete everything currently in the database. You should export the current database to a file first.\n\nDo you really want to continue?"));
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setDefaultButton(QMessageBox::Ok);
    box.button(QMessageBox::Yes)->setText(tr("Yes"));
    box.button(QMessageBox::No)->setText(tr("No"));
    box.button(QMessageBox::Yes)->setIcon(QIcon(":/icons/ok.png"));
    box.button(QMessageBox::No)->setIcon(QIcon(":/icons/cancel.png"));
    if (box.exec() == QMessageBox::No) {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open((QIODevice::ReadOnly | QIODevice::Text))) {
            displayError(tr("Could not open file %1:\n%2").arg(QDir::toNativeSeparators(fileName), file.errorString()));
            return;
        }

        QStringList tables = QSqlDatabase::database().tables();
        for (int i = 0; i < tables.size(); i++) {
            QString table = tables[i];

            QSqlQuery query;
            query.exec(QString("DELETE FROM %1").arg(table));
            query.exec(QString("DELETE FROM 'sqlite_sequence' WHERE NAME = %1").arg(table));
        }

        QSqlDatabase::database().transaction();
        QSqlQuery q;
        bool success = true;
        QTextStream in(&file);

        QString line = in.readLine();
        while (!line.isNull()) {
            line = line.split("--")[0].trimmed();
            if (line.endsWith(";")) {
                line.chop(1);
            }

            if (!line.isEmpty()) {
                if (!q.exec(line.toLatin1())) {
                    success = false;
                    break;
                }
            }

            line = in.readLine();
        }

        file.close();

        if (success) {
            QSqlDatabase::database().commit();
            displayInfo(tr("The file was imported to the database. The application will now restart to apply changes."));
            qApp->exit(42);
        } else {
            QSqlDatabase::database().rollback();
            displayError(tr("SQL Query failed:\n%1\n\n%2").arg(q.lastError().text(), line));
            return;
        }
    }
}

/**
 * Exports data from the database to an external file.
 * The output file can then later be imported into the database.
**/
void MainWindow::exportDatabase()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open((QFile::WriteOnly | QFile::Text))) {
        displayError(tr("Could not create file %1:\n%2").arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QDateTime dt;
    dt.setTime_t(QDateTime::currentSecsSinceEpoch());
    currentShift.setEndDate(dt.toString("yyyy-MM-dd hh:mm:ss"));
    currentShift.save();

    QStringList tables = QSqlDatabase::database().tables();
    for (int i = 0; i < tables.size(); i++) {
        QString table = tables[i];

        QSqlQuery q;
        q.exec(QString("PRAGMA TABLE_INFO(%1)").arg(table));
        QStringList fields;
        while (q.next()) {
            fields << q.value(1).toString();
        }
        QString fieldNames = fields.join(", ");

        q.exec(QString("SELECT %1 FROM %2").arg(fieldNames, table));
        while (q.next()) {
            QString values = "";
            for (int j = 0; j < fields.size(); j++) {
                QString value = q.value(j).toString();
                if (value.isEmpty()) {
                    value = "NULL";
                }

                if (value != "NULL") {
                    value = QString("'%1'").arg(value);
                }
                values += value + ", ";
            }
            values.chop(2);

            QString stmt = QString("INSERT INTO %1 (%2) VALUES(%3)").arg(table, fieldNames, values);
            file.write(stmt.toLatin1());
            file.write("\n");
        }
    }

    file.close();
    displayInfo(tr("The database was exported to the file."));
}

/**
 * Displays a message box with information about the application.
**/
void MainWindow::about()
{
    QMessageBox box;
    box.setWindowTitle(tr("About"));
    box.setWindowIcon(QIcon(":/icons/store.png"));
    box.setIcon(QMessageBox::Information);
    box.setText(tr("A simple application to manage a retail store."));
    box.setStandardButtons(QMessageBox::Ok);
    box.setDefaultButton(QMessageBox::Ok);
    box.button(QMessageBox::Ok)->setText(tr("OK"));
    box.button(QMessageBox::Ok)->setIcon(QIcon(":/icons/ok.png"));
    box.exec();
}


/**
 * Enables/disables buttons based on whether a groupSale is selected or not.
**/
void MainWindow::groupSaleSelectionRefresh()
{
    if (groupSaleTable->selectionModel()->hasSelection()) {
        int row = groupSaleTable->selectionModel()->selectedRows().first().row();
        int id = groupSaleTable->item(row, 0)->text().toInt();
        GroupSale groupSale = GroupSale::getById(id);
        groupSaleReceiptButton->setEnabled(true);
        if (groupSale.shift() == currentShift.id()) {
            groupSaleRemoveButton->setEnabled(true);
        } else {
            groupSaleRemoveButton->setEnabled(false);
        }
    } else {
        groupSaleRemoveButton->setEnabled(false);
        groupSaleReceiptButton->setEnabled(false);
    }
}

/**
 * Refreshes (updates) the data in the sale table by getting items from the database.
 *
 * This method is called whenever a sale is added, edited or removed.
 * This method is also called whenever the sale search box text is changed.
 *
 * If the sale search box text is empty, all sales are displayed.
 * If there is some text in the sale search box, only matching sales are displayed.
**/
void MainWindow::groupSaleRefresh()
{
    groupSaleList = GroupSale::getAll();

    QString searchText = groupSaleSearchEdit->text().trimmed();
    if (!searchText.isEmpty()) {
        QList<GroupSale> matches;
        for (int i = 0; i < groupSaleList.size(); i++) {
            QString customerName = groupSaleList[i].name();
            QString cashierName = User::getById(Shift::getById(groupSaleList[i].shift()).cashier()).name();
            if (customerName.contains(searchText, Qt::CaseInsensitive) ||
                    cashierName.contains(searchText, Qt::CaseInsensitive)) {
                matches.append(groupSaleList[i]);
            }
        }

        groupSaleList = matches;
    }

    groupSaleTable->setSortingEnabled(false);
    groupSaleTable->clearContents();
    groupSaleTable->setRowCount(groupSaleList.size());
    for (int i = 0; i < groupSaleList.size(); i++) {
        groupSaleList[i].calculateTotals();

        QTableWidgetItem *idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole, groupSaleList[i].id());
        groupSaleTable->setItem(i, 0, idItem);

        QString customerName = groupSaleList[i].name();
        groupSaleTable->setItem(i, 1, new QTableWidgetItem(customerName));

        QTableWidgetItem *subtotalItem = new QTableWidgetItem;
        subtotalItem->setData(Qt::EditRole, groupSaleList[i].subtotal());
        groupSaleTable->setItem(i, 2, subtotalItem);

        QTableWidgetItem *discountItem = new QTableWidgetItem;
        discountItem->setData(Qt::EditRole, groupSaleList[i].discount());
        groupSaleTable->setItem(i, 3, discountItem);

        QTableWidgetItem *taxItem = new QTableWidgetItem;
        taxItem->setData(Qt::EditRole, groupSaleList[i].tax());
        groupSaleTable->setItem(i, 4, taxItem);

        QTableWidgetItem *totalItem = new QTableWidgetItem;
        totalItem->setData(Qt::EditRole, groupSaleList[i].total());
        groupSaleTable->setItem(i, 5, totalItem);

        QString cashierName = User::getById(Shift::getById(groupSaleList[i].shift()).cashier()).name();
        groupSaleTable->setItem(i, 6, new QTableWidgetItem(cashierName));

        QDateTime date = QDateTime::fromString(groupSaleList[i].date(), "yyyy-MM-dd hh:mm:ss");
        QString dateString = QLocale().toString(date, QLocale::ShortFormat);
        QTableWidgetItem *dateItem = new QTableWidgetItem;
        dateItem->setData(Qt::EditRole, dateString);
        groupSaleTable->setItem(i, 7, dateItem);
    }
    groupSaleTable->setSortingEnabled(true);
}

/**
 * Displays the receipt for this group sale.
**/
void MainWindow::displayReceipt(int groupSaleId)
{
    GroupSale groupSale = GroupSale::getById(groupSaleId);
    QList<Sale> sales = Sale::getAllByGroup(groupSale.id());
    QString cashierName = User::getById(Shift::getById(groupSale.shift()).cashier()).name();
    QString html;
    html += "<html><head><style>body{margin:10px; padding:10px;} table, td, th {border: 1px; text-align: left;} table {border-collapse: collapse; width: 100%; margin: 10px 0;} th, td {padding: 0 10px;}</style></head><body>";
    html += QString("<h1>%1</h1>").arg(storeName);
    html += QString("<p>Date: %1<br/>").arg(groupSale.date());
    html += QString("Cashier: %1<br/>").arg(cashierName);
    html += QString("Customer: %1</p>").arg(groupSale.name());
    html += "<table>";
    html += "<tr>";
    html += "<th>SN</th>";
    html += "<th>PRODUCT</th>";
    html += "<th>RATE</th>";
    html += "<th>QTY</th>";
    html += "<th>AMOUNT</th>";
    html += "</tr>";
    html += QString("<tr><td colspan='5'></td></tr>");
    for (int i = 0; i < sales.size(); i++) {
        html += "<tr>";
        html += QString("<td>%1</td>").arg(i + 1);
        html += QString("<td>%1</td>").arg(Product::getById(sales[i].product()).name());
        html += QString("<td>%1</td>").arg(sales[i].amount() / sales[i].quantity());
        html += QString("<td>%1</td>").arg(sales[i].quantity());
        html += QString("<td>%1</td>").arg(sales[i].amount());
        html += "</tr>";
    }
    html += QString("<tr><td colspan='5'></td></tr>");
    html += QString("<tr><td></td><td><strong>Subtotal</strong></td><td></td><td></td><td><strong>%1</strong></td></tr>").arg(groupSale.subtotal());
    html += QString("<tr><td></td><td><strong>Discount</strong></td><td></td><td></td><td><strong>%1</strong></td></tr>").arg(groupSale.discount());
    html += QString("<tr><td></td><td><strong>Tax</strong></td><td></td><td></td><td><strong>%1</strong></td></tr>").arg(groupSale.tax());
    html += QString("<tr><td colspan='5'></td></tr>");
    html += QString("<tr><td></td><td><strong>TOTAL</strong></td><td></td><td></td><td><strong>%1</strong></td></tr>").arg(groupSale.total());
    html += "</table>";
    html += QString("<p><strong>THANK YOU FOR SHOPPING WITH US</strong></p>");
    html += "</body></html>";

    WebViewDialog dialog;
    dialog.setWindowTitle(tr("Receipt"));
    dialog.webView->setHtml(html);
    dialog.exec();
}

/**
 * Removes the selected group sale.
**/
void MainWindow::groupSaleRemove()
{
    if (groupSaleTable->selectionModel()->hasSelection()) {
        int row = groupSaleTable->selectionModel()->selectedRows().first().row();
        int id = groupSaleTable->item(row, 0)->text().toInt();
        GroupSale groupSale = GroupSale::getById(id);
        QList<Sale> sales = Sale::getAllByGroup(groupSale.id());
        for (int i = 0; i < sales.size(); i++) {
            Product product = Product::getById(sales[i].product());
            product.setStock(product.stock() + sales[i].quantity());
            product.save();
            sales[i].remove();
        }
        groupSale.remove();
        productRefresh();
        saleRefresh();
        groupSaleRefresh();
        statusBar()->showMessage(tr("Group sale removed."), STATUSBAR_MS);
        groupSaleTable->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Displays the receipt for this group sale.
**/
void MainWindow::groupSaleReceipt()
{
    if (groupSaleTable->selectionModel()->hasSelection()) {
        int row = groupSaleTable->selectionModel()->selectedRows().first().row();
        int id = groupSaleTable->item(row, 0)->text().toInt();
        displayReceipt(id);
        groupSaleTable->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Enables/disables the edit and remove buttons based on whether a sale is selected or not.
 *
 * A sale can only be edited / deleted if it belongs to the current shift.
**/
void MainWindow::saleSelectionRefresh()
{
    if (saleTable->selectionModel()->hasSelection()) {
        int row = saleTable->selectionModel()->selectedRows().first().row();
        int id = saleTable->item(row, 0)->text().toInt();
        Sale sale = Sale::getById(id);
        GroupSale groupSale = GroupSale::getById(sale.group());
        if (groupSale.shift() == currentShift.id()) {
            saleEditButton->setEnabled(true);
            saleRemoveButton->setEnabled(true);
        } else {
            saleEditButton->setEnabled(false);
            saleRemoveButton->setEnabled(false);
        }
    } else {
        saleEditButton->setEnabled(false);
        saleRemoveButton->setEnabled(false);
    }
}

/**
 * Refreshes (updates) the data in the sale table by getting items from the database.
 *
 * This method is called whenever a sale is added, edited or removed.
 * This method is also called whenever the sale search box text is changed.
 *
 * If the sale search box text is empty, all sales are displayed.
 * If there is some text in the sale search box, only matching sales are displayed.
**/
void MainWindow::saleRefresh()
{
    saleList = Sale::getAll();

    QString searchText = saleSearchEdit->text().trimmed();
    if (!searchText.isEmpty()) {
        QList<Sale> matches;
        for (int i = 0; i < saleList.size(); i++) {
            QString thisSaleProductName = Product::getById(saleList[i].product()).name();
            if (thisSaleProductName.contains(searchText, Qt::CaseInsensitive)) {
                matches.append(saleList[i]);
            }
        }

        saleList = matches;
    }

    saleTable->setSortingEnabled(false);
    saleTable->clearContents();
    saleTable->setRowCount(saleList.size());
    for (int i = 0; i < saleList.size(); i++) {
        QTableWidgetItem *idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole, saleList[i].id());
        saleTable->setItem(i, 0, idItem);

        QString productName = Product::getById(saleList[i].product()).name();
        saleTable->setItem(i, 1, new QTableWidgetItem(productName));

        QTableWidgetItem *quantityItem = new QTableWidgetItem;
        quantityItem->setData(Qt::EditRole, saleList[i].quantity());
        saleTable->setItem(i, 2, quantityItem);
        if (saleList[i].quantity() == 0) {
            saleTable->item(i, 2)->setBackground(Qt::red);
        }

        QTableWidgetItem *amountItem = new QTableWidgetItem;
        amountItem->setData(Qt::EditRole, saleList[i].amount());
        saleTable->setItem(i, 3, amountItem);
        if (saleList[i].amount() == 0) {
            saleTable->item(i, 3)->setBackground(Qt::red);
        }

        QTableWidgetItem *groupItem = new QTableWidgetItem;
        groupItem->setData(Qt::EditRole, saleList[i].group());
        saleTable->setItem(i, 4, groupItem);

        QTableWidgetItem *indexItem = new QTableWidgetItem;
        indexItem->setData(Qt::EditRole, saleList[i].index());
        saleTable->setItem(i, 5, indexItem);
    }
    saleTable->setSortingEnabled(true);
}

/**
 * Displays a dialog box where the user can edit the currently selected sale.
**/
void MainWindow::saleEdit()
{
    if (saleTable->selectionModel()->hasSelection()) {
        int row = saleTable->selectionModel()->selectedRows().first().row();
        int id = saleTable->item(row, 0)->text().toInt();
        Sale sale = Sale::getById(id);
        Product product = Product::getById(sale.product());

        SaleEditDialog dialog;
        dialog.setWindowTitle(tr("Edit sale"));
        dialog.productLineEdit->setText(product.name());
        dialog.quantitySpinBox->setValue(sale.quantity());
        dialog.amountSpinBox->setValue(sale.amount());
        int previousQuantity = dialog.quantitySpinBox->value();

        if (dialog.exec()) {
            if (dialog.quantitySpinBox->value() == 0) {
                displayError(tr("Quantity cannot be 0."));
                return;
            }

            int newStock = product.stock() + previousQuantity - dialog.quantitySpinBox->value();
            if (newStock < 0) {
                displayError(tr("Not enough stock of product."));
                return;
            }

            product.setStock(newStock);
            sale.setProduct(product.id());
            sale.setQuantity(dialog.quantitySpinBox->value());
            sale.setAmount(dialog.amountSpinBox->value());
            if (dialog.updateDateCheckBox->isChecked()) {
                GroupSale group = GroupSale::getById(sale.group());
                QDateTime dt;
                dt.setTime_t(QDateTime::currentSecsSinceEpoch());
                group.setDate(dt.toString("yyyy-MM-dd hh:mm:ss"));
            }
            product.save();
            sale.save();
            productRefresh();
            saleRefresh();
            groupSaleRefresh();
            statusBar()->showMessage(tr("Sale edited."), STATUSBAR_MS);
        }
        saleTable->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Removes the currently selected sale from the database.
**/
void MainWindow::saleRemove()
{
    if (saleTable->selectionModel()->hasSelection()) {
        int row = saleTable->selectionModel()->selectedRows().first().row();
        int id = saleTable->item(row, 0)->text().toInt();
        Sale sale = Sale::getById(id);

        Product product = Product::getById(sale.product());
        product.setStock(product.stock() + sale.quantity());
        product.save();
        sale.remove();
        productRefresh();
        saleRefresh();
        groupSaleRefresh();
        statusBar()->showMessage(tr("Sale removed."), STATUSBAR_MS);
        saleTable->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Enables/disables the edit and remove buttons based on whether a purchase is selected or not.
**/
void MainWindow::purchaseSelectionRefresh()
{
    if (purchaseTable->selectionModel()->hasSelection()) {
        purchaseEditButton->setEnabled(true);
        purchaseRemoveButton->setEnabled(true);
    } else {
        purchaseEditButton->setEnabled(false);
        purchaseRemoveButton->setEnabled(false);
    }
}

/**
 * Refreshes (updates) the data in the purchase table by getting items from the database.
 *
 * This method is called whenever a purchase is added, edited or removed.
 * This method is also called whenever the purchase search box text is changed.
 *
 * If the purchase search box text is empty, all purchases are displayed.
 * If there is some text in the purchase search box, only matching purchases are displayed.
**/
void MainWindow::purchaseRefresh()
{
    purchaseList = Purchase::getAll();

    QString searchText = purchaseSearchEdit->text().trimmed();
    if (!searchText.isEmpty()) {
        QList<Purchase> matches;
        for (int i = 0; i < purchaseList.size(); i++) {
            QString thisPurchaseProductName = Product::getById(purchaseList[i].product()).name();
            if (thisPurchaseProductName.contains(searchText, Qt::CaseInsensitive)) {
                matches.append(purchaseList[i]);
            }
        }

        purchaseList = matches;
    }

    purchaseTable->setSortingEnabled(false);
    purchaseTable->clearContents();
    purchaseTable->setRowCount(purchaseList.size());
    for (int i = 0; i < purchaseList.size(); i++) {
        QTableWidgetItem *idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole, purchaseList[i].id());
        purchaseTable->setItem(i, 0, idItem);

        QString productName = Product::getById(purchaseList[i].product()).name();
        purchaseTable->setItem(i, 1, new QTableWidgetItem(productName));

        QTableWidgetItem *quantityItem = new QTableWidgetItem;
        quantityItem->setData(Qt::EditRole, purchaseList[i].quantity());
        purchaseTable->setItem(i, 2, quantityItem);
        if (purchaseList[i].quantity() == 0) {
            purchaseTable->item(i, 3)->setBackground(Qt::red);
        }

        QTableWidgetItem *amountItem = new QTableWidgetItem;
        amountItem->setData(Qt::EditRole, purchaseList[i].amount());
        purchaseTable->setItem(i, 3, amountItem);
        if (purchaseList[i].amount() == 0) {
            purchaseTable->item(i, 3)->setBackground(Qt::red);
        }

        QDateTime date = QDateTime::fromString(purchaseList[i].date(), "yyyy-MM-dd hh:mm:ss");
        QString dateString = QLocale().toString(date, QLocale::ShortFormat);
        QTableWidgetItem *dateItem = new QTableWidgetItem;
        dateItem->setData(Qt::EditRole, dateString);
        purchaseTable->setItem(i, 4, dateItem);
    }
    purchaseTable->setSortingEnabled(true);
}

/**
 * Displays a dialog box where the user can edit the currently selected purchase.
**/
void MainWindow::purchaseEdit()
{
    if (purchaseTable->selectionModel()->hasSelection()) {
        int row = purchaseTable->selectionModel()->selectedRows().first().row();
        int id = purchaseTable->item(row, 0)->text().toInt();
        Purchase purchase = Purchase::getById(id);
        Product product = Product::getById(purchase.product());

        PurchaseEditDialog dialog;
        dialog.setWindowTitle(tr("Edit purchase"));
        dialog.productLineEdit->setText(product.name());
        dialog.quantitySpinBox->setValue(purchase.quantity());
        dialog.amountSpinBox->setValue(purchase.amount());
        int previousQuantity = dialog.quantitySpinBox->value();

        if (dialog.exec()) {
            if (dialog.quantitySpinBox->value() == 0) {
                displayError(tr("Quantity cannot be 0."));
                return;
            }

            int newStock = product.stock() - previousQuantity + dialog.quantitySpinBox->value();
            if (newStock < 0) {
                displayError(tr("Not enough stock of product."));
                return;
            }

            product.setStock(newStock);
            purchase.setProduct(product.id());
            purchase.setQuantity(dialog.quantitySpinBox->value());
            purchase.setAmount(dialog.amountSpinBox->value());
            if (dialog.updateDateCheckBox->isChecked()) {
                QDateTime dt;
                dt.setTime_t(QDateTime::currentSecsSinceEpoch());
                purchase.setDate(dt.toString("yyyy-MM-dd hh:mm:ss"));
            }
            product.save();
            purchase.save();
            productRefresh();
            purchaseRefresh();
            statusBar()->showMessage(tr("Purchase edited."), STATUSBAR_MS);
            purchaseTable->clearSelection();
        }
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Removes the currently selected purchase from the database.
**/
void MainWindow::purchaseRemove()
{
    if (purchaseTable->selectionModel()->hasSelection()) {
        int row = purchaseTable->selectionModel()->selectedRows().first().row();
        int id = purchaseTable->item(row, 0)->text().toInt();
        Purchase purchase = Purchase::getById(id);
        Product product = Product::getById(purchase.product());

        if (purchase.quantity() > product.stock()) {
            displayError(tr("Not enough stock of product."));
        }

        product.setStock(product.stock() - purchase.quantity());
        product.save();
        purchase.remove();
        productRefresh();
        purchaseRefresh();
        statusBar()->showMessage(tr("Purchase removed."), STATUSBAR_MS);
        purchaseTable->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Enables/disables the edit, remove, purchase and sell buttons based on whether a product is selected or not.
**/
void MainWindow::productSelectionRefresh()
{
    if (productTable->selectionModel()->hasSelection()) {
        productEditButton->setEnabled(true);
        productRemoveButton->setEnabled(true);
        productPurchaseButton->setEnabled(true);
    } else {
        productEditButton->setEnabled(false);
        productRemoveButton->setEnabled(false);
        productPurchaseButton->setEnabled(false);
    }
}

/**
 * Refreshes (updates) the data in the product table by getting items from the database.
 *
 * This method is called whenever a product is added, edited or removed.
 * This method is also called whenever the product search box text is changed.
 *
 * If the product search box text is empty, all products are displayed.
 * If there is some text in the product search box, only matching products are displayed.
**/
void MainWindow::productRefresh()
{
    productList = Product::getAll();

    QString searchText = productSearchEdit->text().trimmed();
    if (!searchText.isEmpty()) {
        QList<Product> matches;
        for (int i = 0; i < productList.size(); i++) {
            QString thisProductName = productList[i].name();
            if (thisProductName.contains(searchText, Qt::CaseInsensitive)) {
                matches.append(productList[i]);
            }
        }

        productList = matches;
    }

    productTable->setSortingEnabled(false);
    productTable->clearContents();
    productTable->setRowCount(productList.size());
    for (int i = 0; i < productList.size(); i++) {
        QTableWidgetItem *idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole, productList[i].id());
        productTable->setItem(i, 0, idItem);

        productTable->setItem(i, 1, new QTableWidgetItem(productList[i].name()));

        QString categoryName = Category::getById(productList[i].category()).name();
        productTable->setItem(i, 2, new QTableWidgetItem(categoryName));

        QTableWidgetItem *priceItem = new QTableWidgetItem;
        priceItem->setData(Qt::EditRole, productList[i].price());
        productTable->setItem(i, 3, priceItem);
        if (productList[i].price() == 0) {
            productTable->item(i, 3)->setBackground(Qt::red);
        }

        QTableWidgetItem *stockItem = new QTableWidgetItem;
        stockItem->setData(Qt::EditRole, productList[i].stock());
        productTable->setItem(i, 4, stockItem);
        if (productList[i].stock() == 0) {
            productTable->item(i, 4)->setBackground(Qt::red);
        }
    }
    productTable->setSortingEnabled(true);
}

/**
 * Displays a dialog box where the user can add a new product.
**/
void MainWindow::productAdd()
{
    ProductDialog dialog;
    dialog.setWindowTitle(tr("Add product"));
    QList<Category> categories = Category::getAll();
    for (int i = 0; i < categories.size(); i++) {
        dialog.categoryComboBox->addItem(categories[i].name(), categories[i].id());
    }
    dialog.categoryComboBox->setCurrentIndex(-1);

    if (dialog.exec()) {
        if (dialog.nameLineEdit->text().isEmpty()) {
            displayError(tr("Product name cannot be empty."));
            return;
        }
        Product product;
        product.setName(dialog.nameLineEdit->text());
        product.setCategory(dialog.categoryComboBox->currentData().toInt());
        product.setPrice(dialog.priceSpinBox->value());
        product.setStock(0);
        product.save();
        productRefresh();
        statusBar()->showMessage(tr("Product added."), STATUSBAR_MS);
        productTable->clearSelection();
    }
}

/**
 * Displays a dialog box where the user can edit the currently selected product.
**/
void MainWindow::productEdit()
{
    if (productTable->selectionModel()->hasSelection()) {
        int row = productTable->selectionModel()->selectedRows().first().row();
        int id = productTable->item(row, 0)->text().toInt();
        Product product = Product::getById(id);

        ProductDialog dialog;
        dialog.setWindowTitle(tr("Edit product"));
        QList<Category> categories = Category::getAll();
        for (int i = 0; i < categories.size(); i++) {
            dialog.categoryComboBox->addItem(categories[i].name(), categories[i].id());
        }
        dialog.nameLineEdit->setText(product.name());
        int index = dialog.categoryComboBox->findData(product.category());
        dialog.categoryComboBox->setCurrentIndex(index);
        dialog.priceSpinBox->setValue(product.price());

        if (dialog.exec() && !dialog.nameLineEdit->text().isEmpty()) {
            if (dialog.nameLineEdit->text().isEmpty()) {
                displayError(tr("Product name cannot be empty."));
                return;
            }
            product.setName(dialog.nameLineEdit->text());
            product.setCategory(dialog.categoryComboBox->currentData().toInt());
            product.setPrice(dialog.priceSpinBox->value());
            product.save();
            productRefresh();
            statusBar()->showMessage(tr("Product edited."), STATUSBAR_MS);
            productTable->clearSelection();
        }
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Removes the currently selected product from the database.
**/
void MainWindow::productRemove()
{
    if (productTable->selectionModel()->hasSelection()) {
        int row = productTable->selectionModel()->selectedRows().first().row();
        int id = productTable->item(row, 0)->text().toInt();
        Product product = Product::getById(id);
        product.remove();
        productRefresh();
        statusBar()->showMessage(tr("Product removed."), STATUSBAR_MS);
        productTable->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Purchases the currently selected product.
**/
void MainWindow::productPurchase()
{
    if (productTable->selectionModel()->hasSelection()) {
        int row = productTable->selectionModel()->selectedRows().first().row();
        int id = productTable->item(row, 0)->text().toInt();
        Product product = Product::getById(id);

        PurchaseDialog dialog;
        dialog.setWindowTitle(tr("Purchase product"));
        dialog.nameLineEdit->setText(product.name());
        dialog.priceSpinBox->setValue(product.price());
        dialog.quantitySpinBox->setValue(1);
        dialog.amountSpinBox->setValue(0);

        if (dialog.exec()) {
            if (dialog.quantitySpinBox->value() == 0) {
                displayError(tr("Quantity cannot be 0."));
                return;
            }

            product.setStock(product.stock() + dialog.quantitySpinBox->value());
            if (dialog.modifyPriceCheckBox->isEnabled()) {
                product.setPrice(dialog.priceSpinBox->value());
            }
            product.save();

            Purchase purchase;
            purchase.setProduct(product.id());
            purchase.setQuantity(dialog.quantitySpinBox->value());
            purchase.setAmount(dialog.amountSpinBox->value());
            QDateTime dt;
            dt.setTime_t(QDateTime::currentSecsSinceEpoch());
            purchase.setDate(dt.toString("yyyy-MM-dd hh:mm:ss"));
            purchase.save();

            productRefresh();
            purchaseRefresh();
            statusBar()->showMessage(tr("Product purchased."), STATUSBAR_MS);
            productTable->clearSelection();
        }
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Displays the report for this product.
**/
void MainWindow::displayReportProduct(int productId)
{
    Product product = Product::getById(productId);

    QList<Purchase> all_purchases = Purchase::getAll();
    QList<Purchase> purchases;
    int purchase_amount_sum = 0;
    int purchase_quantity_sum = 0;
    for (int i = 0; i < all_purchases.size(); i++) {
        if (all_purchases[i].product() == product.id()) {
            purchases.append(all_purchases[i]);
            purchase_amount_sum += all_purchases[i].amount();
            purchase_quantity_sum += all_purchases[i].quantity();
        }
    }

    QList<Sale> all_sales = Sale::getAll();
    QList<Sale> sales;
    int sale_amount_sum = 0;
    int sale_quantity_sum = 0;
    for (int i = 0; i < all_sales.size(); i++) {
        if (all_sales[i].product() == product.id()) {
            sales.append(all_sales[i]);
            sale_amount_sum += all_sales[i].amount();
            sale_quantity_sum += all_sales[i].quantity();
        }
    }

    int future_sales_amount = product.stock() * product.price();
    int projected_sales_amount = sale_amount_sum + future_sales_amount;
    int profit_amount = projected_sales_amount - purchase_amount_sum;
    double profit_percent = ((double) profit_amount / purchase_amount_sum) * 100;

    QString html;
    html += "<html><head><style>body{margin:10px; padding:10px;} table, td, th {border: 1px; text-align: left;} table {border-collapse: collapse; width: 100%; margin: 10px 0;} th, td {padding: 0 10px;}</style></head><body>";
    html += QString("<h1>Product Report for %1</h1>").arg(product.name());
    html += "<h3>Purchases</h3>";
    html += QString("<p>Total purchase quantity: %1</p>").arg(purchase_quantity_sum);
    html += QString("<p>Total purchase amount: %1</p>").arg(purchase_amount_sum);
    html += "<h3>Sales</h3>";
    html += QString("<p>Total sales quantity: %1</p>").arg(sale_quantity_sum);
    html += QString("<p>Total sales amount: %1</p>").arg(sale_amount_sum);
    html += "<h3>Future Sales</h3>";
    html += QString("<p>Available stock: %1</p>").arg(product.stock());
    html += QString("<p>Current price: %1</p>").arg(product.price());
    html += QString("<p>Expected sales amount from stock: %1</p>").arg(future_sales_amount);
    html += "<h3>Profit/Loss</h3>";
    if (profit_amount > 0) {
        html += QString("<p>Profit amount: %1</p>").arg(profit_amount);
        html += QString("<p>Profit percent: %1</p>").arg(profit_percent);
    } else {
        html += QString("<p>Loss amount: %1</p>").arg(profit_amount);
        html += QString("<p>Loss percent: %1</p>").arg(profit_percent);
    }
    html += "</body></html>";

    WebViewDialog dialog;
    dialog.setWindowTitle(tr("Product Report"));
    dialog.webView->setHtml(html);
    dialog.exec();
}

/**
 * Displays a report for the currently selected product.
**/
void MainWindow::productReport()
{
    if (productTable->selectionModel()->hasSelection()) {
        int row = productTable->selectionModel()->selectedRows().first().row();
        int id = productTable->item(row, 0)->text().toInt();
        Product product = Product::getById(id);
        displayReportProduct(product.id());
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Enables/disables the edit and remove buttons based on whether a category is selected or not.
**/
void MainWindow::categorySelectionRefresh()
{
    if (categoryTable->selectionModel()->hasSelection()) {
        categoryEditButton->setEnabled(true);
        categoryRemoveButton->setEnabled(true);
    } else {
        categoryEditButton->setEnabled(false);
        categoryRemoveButton->setEnabled(false);
    }
}

/**
 * Refreshes (updates) the data in the category table by getting items from the database.
 *
 * This method is called whenever a category is added, edited or removed.
 * This method is also called whenever the category search box text is changed.
 *
 * If the category search box text is empty, all categories are displayed.
 * If there is some text in the category search box, only matching categories are displayed.
**/
void MainWindow::categoryRefresh()
{
    categoryList = Category::getAll();

    QString searchText = categorySearchEdit->text().trimmed();
    if (!searchText.isEmpty()) {
        QList<Category> matches;
        for (int i = 0; i < categoryList.size(); i++) {
            QString thisCategoryName = categoryList[i].name();
            if (thisCategoryName.contains(searchText, Qt::CaseInsensitive)) {
                matches.append(categoryList[i]);
            }
        }

        categoryList = matches;
    }

    categoryTable->setSortingEnabled(false);
    categoryTable->clearContents();
    categoryTable->setRowCount(categoryList.size());
    for (int i = 0; i < categoryList.size(); i++) {
        QTableWidgetItem *idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole, categoryList[i].id());
        categoryTable->setItem(i, 0, idItem);
        categoryTable->setItem(i, 1, new QTableWidgetItem(categoryList[i].name()));
    }
    categoryTable->setSortingEnabled(true);
}

/**
 * Displays a dialog box where the user can add a new category.
**/
void MainWindow::categoryAdd()
{
    TextInputDialog dialog;
    dialog.setWindowTitle(tr("Add category"));
    if (dialog.exec()) {
        if (dialog.lineEdit->text().isEmpty()) {
            displayError(tr("Category name cannot be empty."));
            return;
        }
        Category category;
        category.setName(dialog.lineEdit->text());
        category.save();
        categoryRefresh();
        statusBar()->showMessage(tr("Category added."), STATUSBAR_MS);
        categoryTable->clearSelection();
    }
}

/**
 * Displays a dialog box where the user can edit the currently selected category.
**/
void MainWindow::categoryEdit()
{
    if (categoryTable->selectionModel()->hasSelection()) {
        int row = categoryTable->selectionModel()->selectedRows().first().row();
        int id = categoryTable->item(row, 0)->text().toInt();
        Category category = Category::getById(id);
        TextInputDialog dialog;
        dialog.setWindowTitle(tr("Edit category"));
        dialog.lineEdit->setText(category.name());
        if (dialog.exec()) {
            if (dialog.lineEdit->text().isEmpty()) {
                displayError(tr("Category name cannot be empty."));
                return;
            }
            category.setName(dialog.lineEdit->text());
            category.save();
            categoryRefresh();
            productRefresh();
            statusBar()->showMessage(tr("Category edited."), STATUSBAR_MS);
            categoryTable->clearSelection();
        }
    } else {
        displayError(tr("Nothing selected."));
        return;
    }
}

/**
 * Removes the currently selected category from the database.
**/
void MainWindow::categoryRemove()
{
    if (categoryTable->selectionModel()->hasSelection()) {
        int row = categoryTable->selectionModel()->selectedRows().first().row();
        int id = categoryTable->item(row, 0)->text().toInt();
        Category category = Category::getById(id);
        category.remove();
        categoryRefresh();
        productRefresh();
        statusBar()->showMessage(tr("Category removed."), STATUSBAR_MS);
        categoryTable->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Displays the report for this category.
**/
void MainWindow::displayReportCategory(int categoryId)
{
    Category category = Category::getById(categoryId);

    QList<Purchase> all_purchases = Purchase::getAll();
    QList<Purchase> purchases;
    int purchase_amount_sum = 0;
    int purchase_quantity_sum = 0;
    for (int i = 0; i < all_purchases.size(); i++) {
        Product product = Product::getById(all_purchases[i].product());
        if (product.category() == category.id()) {
            purchases.append(all_purchases[i]);
            purchase_amount_sum += all_purchases[i].amount();
            purchase_quantity_sum += all_purchases[i].quantity();
        }
    }

    QList<Sale> all_sales = Sale::getAll();
    QList<Sale> sales;
    int sale_amount_sum = 0;
    int sale_quantity_sum = 0;
    for (int i = 0; i < all_sales.size(); i++) {
        Product product = Product::getById(all_sales[i].product());
        if (product.category() == category.id()) {
            sales.append(all_sales[i]);
            sale_amount_sum += all_sales[i].amount();
            sale_quantity_sum += all_sales[i].quantity();
        }
    }

    QList<Product> all_products = Product::getAll();
    int future_sales_amount = 0;
    for (int i = 0; i < all_products.size(); i++) {
        if (all_products[i].category() == category.id()) {
            future_sales_amount += all_products[i].stock() * all_products[i].price();
        }
    }

    int projected_sales_amount = sale_amount_sum + future_sales_amount;
    int profit_amount = projected_sales_amount - purchase_amount_sum;
    double profit_percent = ((double) profit_amount / purchase_amount_sum) * 100;

    QString html;
    html += "<html><head><style>body{margin:10px; padding:10px;} table, td, th {border: 1px; text-align: left;} table {border-collapse: collapse; width: 100%; margin: 10px 0;} th, td {padding: 0 10px;}</style></head><body>";
    html += QString("<h1>Category Report for %1</h1>").arg(category.name());
    html += "<h3>Purchases</h3>";
    html += QString("<p>Total purchase quantity: %1</p>").arg(purchase_quantity_sum);
    html += QString("<p>Total purchase amount: %1</p>").arg(purchase_amount_sum);
    html += "<h3>Sales</h3>";
    html += QString("<p>Total sales quantity: %1</p>").arg(sale_quantity_sum);
    html += QString("<p>Total sales amount: %1</p>").arg(sale_amount_sum);
    html += "<h3>Future Sales</h3>";
    html += QString("<p>Expected sales amount from stock: %1</p>").arg(future_sales_amount);
    html += "<h3>Profit/Loss</h3>";
    if (profit_amount > 0) {
        html += QString("<p>Profit amount: %1</p>").arg(profit_amount);
        html += QString("<p>Profit percent: %1</p>").arg(profit_percent);
    } else {
        html += QString("<p>Loss amount: %1</p>").arg(profit_amount);
        html += QString("<p>Loss percent: %1</p>").arg(profit_percent);
    }
    html += "</body></html>";

    WebViewDialog dialog;
    dialog.setWindowTitle(tr("Category Report"));
    dialog.webView->setHtml(html);
    dialog.exec();
}

/**
 * Displays a report for the currently selected category.
**/
void MainWindow::categoryReport()
{
    if (categoryTable->selectionModel()->hasSelection()) {
        int row = categoryTable->selectionModel()->selectedRows().first().row();
        int id = categoryTable->item(row, 0)->text().toInt();
        Category category = Category::getById(id);
        displayReportCategory(category.id());
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Enables/disables the edit and remove buttons based on whether a user is selected or not.
**/
void MainWindow::userSelectionRefresh()
{
    if (userTable->selectionModel()->hasSelection()) {
        int row = userTable->selectionModel()->selectedRows().first().row();
        int id = userTable->item(row, 0)->text().toInt();
        User user = User::getById(id);
        userEditButton->setEnabled(true);
        if (currentUser.id() != user.id()) {
            userRemoveButton->setEnabled(true);
        }
    } else {
        userEditButton->setEnabled(false);
        userRemoveButton->setEnabled(false);
    }
}

/**
 * Refreshes (updates) the data in the user table by getting items from the database.
 *
 * This method is called whenever a user is added, edited or removed.
 * This method is also called whenever the user search box text is changed.
 *
 * If the user search box text is empty, all users are displayed.
 * If there is some text in the user search box, only matching users are displayed.
**/
void MainWindow::userRefresh()
{
    userList = User::getAll();

    QString searchText = userSearchEdit->text().trimmed();
    if (!searchText.isEmpty()) {
        QList<User> matches;
        for (int i = 0; i < userList.size(); i++) {
            if (userList[i].name().contains(searchText, Qt::CaseInsensitive)
                    || userList[i].username().contains(searchText, Qt::CaseInsensitive)) {
                matches.append(userList[i]);
            }
        }

        userList = matches;
    }

    userTable->setSortingEnabled(false);
    userTable->clearContents();
    userTable->setRowCount(userList.size());
    for (int i = 0; i < userList.size(); i++) {
        QTableWidgetItem *idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole, userList[i].id());
        userTable->setItem(i, 0, idItem);
        userTable->setItem(i, 1, new QTableWidgetItem(userList[i].name()));
        userTable->setItem(i, 2, new QTableWidgetItem(userList[i].username()));
        userTable->setItem(i, 3, new QTableWidgetItem(userList[i].password()));
        QTableWidgetItem *isAdminItem = new QTableWidgetItem;
        isAdminItem->setData(Qt::EditRole, userList[i].isAdmin());
        userTable->setItem(i, 4, isAdminItem);
    }
    userTable->setSortingEnabled(true);
}

/**
 * Displays a dialog box where the user can edit the currently selected user.
**/
void MainWindow::userEdit()
{
    if (userTable->selectionModel()->hasSelection()) {
        int row = userTable->selectionModel()->selectedRows().first().row();
        int id = userTable->item(row, 0)->text().toInt();
        User user = User::getById(id);
        UserDialog dialog;
        dialog.setWindowTitle(tr("Edit user"));
        dialog.nameLineEdit->setText(user.name());
        dialog.isAdminCheckBox->setChecked(user.isAdmin());
        if (currentUser.id() == user.id()) {
            dialog.isAdminCheckBox->setEnabled(false);
        }
        if (dialog.exec()) {
            if (dialog.nameLineEdit->text().isEmpty()) {
                displayError(tr("Name cannot be empty."));
                return;
            }
            user.setName(dialog.nameLineEdit->text());
            user.setIsAdmin(dialog.isAdminCheckBox->isChecked());
            user.save();
            userRefresh();
            productRefresh();
            statusBar()->showMessage(tr("User edited."), STATUSBAR_MS);
            userTable->clearSelection();
        }
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Removes the currently selected user from the database.
**/
void MainWindow::userRemove()
{
    if (userTable->selectionModel()->hasSelection()) {
        int row = userTable->selectionModel()->selectedRows().first().row();
        int id = userTable->item(row, 0)->text().toInt();
        User user = User::getById(id);
        user.remove();
        userRefresh();
        productRefresh();
        statusBar()->showMessage(tr("User removed."), STATUSBAR_MS);
        userTable->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Displays the report for this user.
**/
void MainWindow::displayReportUser(int userId)
{
    User user = User::getById(userId);

    int subtotal_sum = 0;
    int total_sum = 0;
    int transactions = 0;
    QList<Shift> shifts = Shift::getAllByCashier(user.id());
    for (int i = 0; i < shifts.size(); i++) {
        QList<GroupSale> groupSales = GroupSale::getAllByShift(shifts[i].id());
        for (int j = 0; j < groupSales.size(); j++) {
            subtotal_sum += groupSales[j].subtotal();
            total_sum += groupSales[j].total();
        }
        transactions += groupSales.size();
    }

    QString html;
    html += "<html><head><style>body{margin:10px; padding:10px;} table, td, th {border: 1px; text-align: left;} table {border-collapse: collapse; width: 100%; margin: 10px 0;} th, td {padding: 0 10px;}</style></head><body>";
    html += QString("<h1>User Report for %1</h1>").arg(user.name());
    if (user.isAdmin()) {
        html += "<p>Admin User</p>";
    }
    html += "<h3>Sales</h3>";
    if (transactions == 0) {
        html += "<p>No sales found by this user.</p>";
    } else {
        html += QString("<p>Number of transactions: %1</p>").arg(transactions);
        html += QString("<p>Net sales (before discount and tax): %1</p>").arg(subtotal_sum);
        html += QString("<p>Gross sales (after discount and tax): %1</p>").arg(total_sum);
    }
    html += "</body></html>";

    WebViewDialog dialog;
    dialog.setWindowTitle(tr("User Report"));
    dialog.webView->setHtml(html);
    dialog.exec();
}

/**
 * Displays a report for the currently selected user.
**/
void MainWindow::userReport()
{
    if (userTable->selectionModel()->hasSelection()) {
        int row = userTable->selectionModel()->selectedRows().first().row();
        int id = userTable->item(row, 0)->text().toInt();
        User user = User::getById(id);
        displayReportUser(user.id());
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Enables/disables buttons based on whether a shift is selected or not.
**/
void MainWindow::shiftSelectionRefresh()
{
    if (shiftTable->selectionModel()->hasSelection()) {
        // Enable some buttons
    } else {
        // Disable some buttons
    }
}

/**
 * Refreshes (updates) the data in the shift table by getting items from the database.
 *
 * This method is also called whenever the shift search box text is changed.
 *
 * If the shift search box text is empty, all shifts are displayed.
 * If there is some text in the shift search box, only matching shifts are displayed.
**/
void MainWindow::shiftRefresh()
{
    shiftList = Shift::getAll();

    QString searchText = shiftSearchEdit->text().trimmed();
    if (!searchText.isEmpty()) {
        QList<Shift> matches;
        for (int i = 0; i < shiftList.size(); i++) {
            QString cashierName = User::getById(shiftList[i].cashier()).name();
            if (cashierName.contains(searchText, Qt::CaseInsensitive)) {
                matches.append(shiftList[i]);
            }
        }

        shiftList = matches;
    }

    shiftTable->setSortingEnabled(false);
    shiftTable->clearContents();
    shiftTable->setRowCount(shiftList.size());
    for (int i = 0; i < shiftList.size(); i++) {
        QTableWidgetItem *idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole, shiftList[i].id());
        shiftTable->setItem(i, 0, idItem);

        QString cashierName = User::getById(shiftList[i].cashier()).name();
        shiftTable->setItem(i, 1, new QTableWidgetItem(cashierName));

        QDateTime startDate = QDateTime::fromString(shiftList[i].startDate(), "yyyy-MM-dd hh:mm:ss");
        QString startDateString = QLocale().toString(startDate, QLocale::ShortFormat);
        QTableWidgetItem *startDateItem = new QTableWidgetItem;
        startDateItem->setData(Qt::EditRole, startDateString);
        shiftTable->setItem(i, 2, startDateItem);

        QDateTime endDate = QDateTime::fromString(shiftList[i].endDate(), "yyyy-MM-dd hh:mm:ss");
        QString endDateString = QLocale().toString(endDate, QLocale::ShortFormat);
        QTableWidgetItem *endDateItem = new QTableWidgetItem;
        endDateItem->setData(Qt::EditRole, endDateString);
        shiftTable->setItem(i, 3, endDateItem);
    }
    shiftTable->setSortingEnabled(true);
}

/**
 * Displays the report for this shift.
**/
void MainWindow::displayReportShift(int shiftId)
{
    Shift shift = Shift::getById(shiftId);

    QList<GroupSale> groupSales = GroupSale::getAllByShift(shift.id());
    int subtotal_sum = 0;
    int total_sum = 0;
    for (int i = 0; i < groupSales.size(); i++) {
        subtotal_sum += groupSales[i].subtotal();
        total_sum += groupSales[i].total();
    }

    QString html;
    html += "<html><head><style>body{margin:10px; padding:10px;} table, td, th {border: 1px; text-align: left;} table {border-collapse: collapse; width: 100%; margin: 10px 0;} th, td {padding: 0 10px;}</style></head><body>";
    html += "<h1>Shift Report</h1>";
    html += QString("<p>Cashier: %1</p>").arg(User::getById(shift.cashier()).name());
    html += QString("<p>Start Date: %1</p>").arg(shift.startDate());
    html += QString("<p>End Date: %1</p>").arg(shift.endDate());
    html += "<h3>Sales</h3>";
    if (groupSales.size() == 0) {
        html += "<p>No sales found during this shift.</p>";
    } else {
        html += QString("<p>Number of transactions: %1</p>").arg(groupSales.size());
        html += QString("<p>Net sales (before discount and tax): %1</p>").arg(subtotal_sum);
        html += QString("<p>Gross sales (after discount and tax): %1</p>").arg(total_sum);
    }
    html += "</body></html>";

    WebViewDialog dialog;
    dialog.setWindowTitle(tr("Shift Report"));
    dialog.webView->setHtml(html);
    dialog.exec();
}

/**
 * Displays a report for the currently selected shift.
**/
void MainWindow::shiftReport()
{
    if (shiftTable->selectionModel()->hasSelection()) {
        int row = shiftTable->selectionModel()->selectedRows().first().row();
        int id = shiftTable->item(row, 0)->text().toInt();
        Shift shift = Shift::getById(id);
        displayReportShift(shift.id());
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Displays a dialog box where the user can edit the store name.
**/
void MainWindow::storeNameEdit()
{
    TextInputDialog dialog;
    dialog.setWindowTitle(tr("Edit store name"));
    dialog.lineEditLabel->setText(tr("Store name"));
    dialog.lineEdit->setText(storeName);
    if (dialog.exec()) {
        if (dialog.lineEdit->text().isEmpty()) {
            displayError(tr("Store name cannot be empty."));
            return;
        }

        storeName = dialog.lineEdit->text();
        statusBar()->showMessage(tr("Store name edited."), STATUSBAR_MS);
        storeNameRefresh();
    }
}

/**
 * Updates the new store name in various UI locations.
**/
void MainWindow::storeNameRefresh()
{
    setWindowTitle(storeName + " - " + qApp->applicationName());
}

/**
 * Displays a dialog box where the user can edit the default discount rate.
**/
void MainWindow::defaultDiscountEdit()
{
    SpinBoxDialog dialog;
    dialog.setWindowTitle(tr("Edit default discount"));
    dialog.spinBoxLabel->setText(tr("Discount rate"));
    dialog.spinBox->setMinimum(0);
    dialog.spinBox->setMaximum(20);
    dialog.spinBox->setValue(defaultDiscount);
    if (dialog.exec()) {
        defaultDiscount = dialog.spinBox->value();
        statusBar()->showMessage(tr("Discount rate edited."), STATUSBAR_MS);
    }
}

/**
 * Displays a dialog box where the user can edit the default tax rate.
**/
void MainWindow::defaultTaxEdit()
{
    SpinBoxDialog dialog;
    dialog.setWindowTitle(tr("Edit default tax"));
    dialog.spinBoxLabel->setText(tr("Tax rate"));
    dialog.spinBox->setMinimum(0);
    dialog.spinBox->setMaximum(20);
    dialog.spinBox->setValue(defaultTax);
    if (dialog.exec()) {
        defaultTax = dialog.spinBox->value();
        statusBar()->showMessage(tr("Tax rate edited."), STATUSBAR_MS);
    }
}
