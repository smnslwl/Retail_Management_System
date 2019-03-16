#include "newsalewindow.h"
#include "mainwindow.h"
#include "product.h"
#include "groupsale.h"
#include "sale.h"
#include "quantitydialog.h"
#include <cmath>
#include <QtWidgets>

/**
 * Constructs a new NewSaleWindow and performs initial window setup.
**/
NewSaleWindow::NewSaleWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/icons/store.png"));
    setWindowTitle(tr("New Sale"));
    setWindowModality(Qt::ApplicationModal);
    setMinimumSize(QSize(640, 480));
    mainWindow = (MainWindow *) parent;

    QLabel *storeNameLabel = new QLabel(tr("<font color='red'>%1</font>").arg(mainWindow->storeName));
    storeNameLabel->setAlignment(Qt::AlignLeft);
    QLabel *currentUserLabel = new QLabel(tr("<font color='blue'>%1</font>").arg(mainWindow->currentUser.name()));
    currentUserLabel->setAlignment(Qt::AlignRight);
    timeLabel = new QLabel;
    timeLabel->setAlignment(Qt::AlignCenter);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NewSaleWindow::timeRefresh);
    timer->start(1000);
    timeRefresh();

    searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText(tr("Search"));
    productNameLabel = new QLabel;
    productNameLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    productPriceLabel = new QLabel;
    quantitySpinBox = new QSpinBox;
    addButton = new QPushButton(QIcon(":/icons/add.png"), tr("Add..."));
    editButton = new QPushButton(QIcon(":/icons/edit.png"), tr("Edit..."));
    removeButton = new QPushButton(QIcon(":/icons/remove.png"), tr("Remove"));

    customerNameEdit = new QLineEdit;
    customerNameEdit->setPlaceholderText("Customer Name");
    customerNameEdit->setAlignment(Qt::AlignRight);
    subtotalLabel = new QLabel;
    subtotalLabel->setAlignment(Qt::AlignRight);
    discountSpinBox = new QSpinBox;
    discountSpinBox->setMinimum(0);
    discountSpinBox->setMaximum(20);
    discountSpinBox->setAlignment(Qt::AlignRight);
    taxSpinBox = new QSpinBox;
    taxSpinBox->setMinimum(0);
    taxSpinBox->setMaximum(20);
    taxSpinBox->setEnabled(false);
    taxSpinBox->setAlignment(Qt::AlignRight);
    totalLabel = new QLabel;
    totalLabel->setAlignment(Qt::AlignRight);

    confirmButton = new QPushButton(QIcon(":/icons/ok.png"), tr("Confirm"));
    clearButton = new QPushButton(QIcon(":/icons/clear.png"), tr("Clear"));
    exitButton = new QPushButton(QIcon(":/icons/exit.png"), tr("Exit"));
    exitButton->setShortcut(QKeySequence(Qt::Key_Escape));

    searchTable = new QTableWidget;
    searchTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable in-place editing
    searchTable->setSelectionBehavior(QAbstractItemView::SelectRows);  // only rows can be selected, not columns or sells
    searchTable->setSelectionMode(QAbstractItemView::SingleSelection);  // disable selection of multiple rows
    searchTable->setColumnCount(4);  // product id, name, price and stock
    searchHeaderLabels.clear();
    searchHeaderLabels << tr("ID") << tr("Name") << tr("Price") << tr("Stock");
    searchTable->setHorizontalHeaderLabels(searchHeaderLabels);
    searchTable->hideColumn(0);  // hide the ID column

    saleTable = new QTableWidget;
    saleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable in-place editing
    saleTable->setSelectionBehavior(QAbstractItemView::SelectRows);  // only rows can be selected, not columns or sells
    saleTable->setSelectionMode(QAbstractItemView::SingleSelection);  // disable selection of multiple rows
    saleTable->setColumnCount(6);  // product id, name, price, quantity, amount and index in saleList
    saleHeaderLabels.clear();
    saleHeaderLabels << tr("ID") << tr("Name") << tr("Price") << tr("Quantity") << tr("Amount") << tr("Index");
    saleTable->setHorizontalHeaderLabels(saleHeaderLabels);
    saleTable->hideColumn(0);  // hide the ID column
    saleTable->hideColumn(5);  // hide the index column

    QFont bigFont = totalLabel->font();
    bigFont.setPointSize(18);
    bigFont.setBold(true);

    storeNameLabel->setFont(bigFont);
    currentUserLabel->setFont(bigFont);
    timeLabel->setFont(bigFont);

    subtotalLabel->setFont(bigFont);
    totalLabel->setFont(bigFont);
    taxSpinBox->setFont(bigFont);
    discountSpinBox->setFont(bigFont);

    bigFont.setPointSize(18);
    bigFont.setBold(false);

    searchEdit->setFont(bigFont);
    customerNameEdit->setFont(bigFont);
    productNameLabel->setFont(bigFont);
    quantitySpinBox->setFont(bigFont);

    QSize buttonSize(18, 18);

    confirmButton->setFont(bigFont);
    confirmButton->setIconSize(buttonSize);
    clearButton->setFont(bigFont);
    clearButton->setIconSize(buttonSize);
    exitButton->setFont(bigFont);
    exitButton->setIconSize(buttonSize);
    addButton->setFont(bigFont);
    addButton->setIconSize(buttonSize);

    QHBoxLayout *productLayout = new QHBoxLayout;
    productLayout->addWidget(productNameLabel, 1);
    productLayout->addWidget(productPriceLabel);
    productLayout->addWidget(quantitySpinBox);
    productLayout->addWidget(addButton);

    QHBoxLayout *saleActionsLayout = new QHBoxLayout;
    saleActionsLayout->setAlignment(Qt::AlignRight);
    saleActionsLayout->addWidget(editButton);
    saleActionsLayout->addWidget(removeButton);

    QFormLayout *saleFormLayout = new QFormLayout;
    saleFormLayout->addRow(tr("Customer"), customerNameEdit);
    saleFormLayout->addRow(tr("Subtotal"), subtotalLabel);
    saleFormLayout->addRow(tr("Discount"), discountSpinBox);
    saleFormLayout->addRow(tr("Tax"), taxSpinBox);
    saleFormLayout->addRow(tr("Total"), totalLabel);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(searchEdit);
    leftLayout->addWidget(searchTable);
    leftLayout->addLayout(productLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(exitButton);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(saleTable);
    rightLayout->addLayout(saleActionsLayout);
    rightLayout->addLayout(saleFormLayout);
    rightLayout->addLayout(buttonLayout);

    QSplitter *splitter = new QSplitter;
    QWidget *leftWidget = new QWidget;
    QWidget *rightWidget = new QWidget;
    leftWidget->setLayout(leftLayout);
    rightWidget->setLayout(rightLayout);
    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);

    QHBoxLayout *headerBarLayout = new QHBoxLayout;
    headerBarLayout->addWidget(storeNameLabel);
    headerBarLayout->addWidget(timeLabel);
    headerBarLayout->addWidget(currentUserLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(headerBarLayout);
    mainLayout->addWidget(splitter, 1);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    connect(addButton, &QPushButton::clicked, this, &NewSaleWindow::add);
    connect(editButton, &QPushButton::clicked, this, &NewSaleWindow::edit);
    connect(removeButton, &QPushButton::clicked, this, &NewSaleWindow::remove);
    connect(confirmButton, &QPushButton::clicked, this, &NewSaleWindow::confirmSale);
    connect(clearButton, &QPushButton::clicked, this, &NewSaleWindow::clearSale);
    connect(exitButton, &QPushButton::clicked, this, &NewSaleWindow::close);
    connect(searchEdit, &QLineEdit::textEdited, this, &NewSaleWindow::searchRefresh);
    connect(searchTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &NewSaleWindow::searchSelectionRefresh);
    connect(saleTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &NewSaleWindow::saleSelectionRefresh);
    // See <https://wiki.qt.io/New_Signal_Slot_Syntax#Overload> for more information
    connect(discountSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &NewSaleWindow::saleUpdateTotals);
    connect(taxSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &NewSaleWindow::saleUpdateTotals);
    clearSale();
}

/**
 * Updates the time display.
 *
 * A timer calls this function every second.
**/
void NewSaleWindow::timeRefresh()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    timeLabel->setText(QString("<font color='green'>%1</font>").arg(text));
}

/**
 * Performs some actions when the window close button is clicked.
 *
 * Ask for confirmation if there is an ongoing sale.
**/
void NewSaleWindow::closeEvent(QCloseEvent *event)
{
    if (!saleList.isEmpty()) {
        QMessageBox box;
        box.setWindowTitle(tr("Are you sure?"));
        box.setWindowIcon(QIcon(":/icons/store.png"));
        box.setIcon(QMessageBox::Question);
        box.setText(tr("There is a sale in progress. Do you really want to quit?"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        box.setDefaultButton(QMessageBox::Yes);
        box.button(QMessageBox::Yes)->setText(tr("Yes"));
        box.button(QMessageBox::Yes)->setIcon(QIcon(":/icons/ok.png"));
        box.button(QMessageBox::No)->setText(tr("No"));
        box.button(QMessageBox::No)->setIcon(QIcon(":/icons/cancel.png"));

        int reply = box.exec();
        if (reply == QMessageBox::Yes) {
            clearSale();
            mainWindow->show();
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        mainWindow->show();
        event->accept();
    }
}

/**
 * Displays a message box with information about an error.
**/
void NewSaleWindow::displayError(const QString &message)
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
void NewSaleWindow::displayInfo(const QString &message)
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
 * Enables/disables the add button and related widgets based on whether a product is selected or not.
**/
void NewSaleWindow::searchSelectionRefresh()
{
    if (searchTable->selectionModel()->hasSelection()) {
        int row = searchTable->selectionModel()->selectedRows().first().row();
        int id = searchTable->item(row, 0)->text().toInt();
        Product product = Product::getById(id);
        quantitySpinBox->setEnabled(true);
        addButton->setEnabled(true);
        productNameLabel->setText(QString("%1").arg(product.name()));
        quantitySpinBox->setMaximum(product.stock() - productQuantities.value(product.id(), 0));
    } else {
        quantitySpinBox->setEnabled(false);
        addButton->setEnabled(false);
        productNameLabel->setText(QString("<i>%1</i>").arg(tr("Nothing selected")));
        quantitySpinBox->setValue(1);
    }
}

/**
 * Refreshes the list of products in the search table.
**/
void NewSaleWindow::searchRefresh()
{
    QString searchText = searchEdit->text().trimmed();
    QList<Product> products = Product::getAll();

    QList<Product> matches;
    for (int i = 0; i < products.size(); i++) {
        if (products[i].stock() - productQuantities.value(products[i].id(), 0) > 0) {
            if (searchText.isEmpty()) {
                matches.append(products[i]);
            } else if (products[i].name().contains(searchText, Qt::CaseInsensitive)) {
                matches.append(products[i]);
            }
        }
    }

    products = matches;

    searchTable->setSortingEnabled(false);
    searchTable->clearContents();
    searchTable->setRowCount(products.size());
    for (int i = 0; i < products.size(); i++) {
        QTableWidgetItem *idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole, products[i].id());
        searchTable->setItem(i, 0, idItem);

        searchTable->setItem(i, 1, new QTableWidgetItem(products[i].name()));

        QTableWidgetItem *priceItem = new QTableWidgetItem;
        priceItem->setData(Qt::EditRole, products[i].price());
        searchTable->setItem(i, 2, priceItem);
        if (products[i].price() == 0) {
            searchTable->item(i, 2)->setBackground(Qt::red);
        }

        QTableWidgetItem *stockItem = new QTableWidgetItem;
        stockItem->setData(Qt::EditRole, products[i].stock() - productQuantities.value(products[i].id(), 0));
        searchTable->setItem(i, 3, stockItem);
    }
    searchTable->setSortingEnabled(true);
    searchTable->selectionModel()->clearSelection();
}

/**
 * Enables/disables buttons based on whether a sale is selected or not.
**/
void NewSaleWindow::saleSelectionRefresh()
{
    if (saleTable->selectionModel()->hasSelection()) {
        editButton->setEnabled(true);
        removeButton->setEnabled(true);
    } else {
        editButton->setEnabled(false);
        removeButton->setEnabled(false);
    }

    if (saleList.isEmpty()) {
        confirmButton->setEnabled(false);
        clearButton->setEnabled(false);
    } else {
        confirmButton->setEnabled(true);
        clearButton->setEnabled(true);
    }
 }

/**
 * Refreshes the list of sales in the search table.
**/
void NewSaleWindow::saleRefresh()
{
    saleTable->setSortingEnabled(false);
    saleTable->clearContents();
    saleTable->setRowCount(saleList.size());
    for (int i = 0; i < saleList.size(); i++) {
        Product product = Product::getById(saleList[i].product());

        QTableWidgetItem *idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole, product.id());
        saleTable->setItem(i, 0, idItem);

        saleTable->setItem(i, 1, new QTableWidgetItem(product.name()));

        QTableWidgetItem *priceItem = new QTableWidgetItem;
        priceItem->setData(Qt::EditRole, product.price());
        saleTable->setItem(i, 2, priceItem);
        if (product.price() == 0) {
            saleTable->item(i, 2)->setBackground(Qt::red);
        }

        QTableWidgetItem *quantityItem = new QTableWidgetItem;
        quantityItem->setData(Qt::EditRole, saleList[i].quantity());
        saleTable->setItem(i, 3, quantityItem);
        if (saleList[i].quantity() == 0) {
            saleTable->item(i, 3)->setBackground(Qt::red);
        }

        QTableWidgetItem *amountItem = new QTableWidgetItem;
        amountItem->setData(Qt::EditRole, saleList[i].amount());
        saleTable->setItem(i, 4, amountItem);
        if (saleList[i].amount() == 0) {
            saleTable->item(i, 4)->setBackground(Qt::red);
        }

        // Hidden column to be used as a unique identifier to help in editing/removing sales.
        // We can't use the sales id as it will only be available once the sale is committed to the database.
        // We also can't use the product id as the same product can occur multiple times.
        QTableWidgetItem *indexItem = new QTableWidgetItem;
        indexItem->setData(Qt::EditRole, i);
        saleTable->setItem(i, 5, indexItem);
    }
    saleTable->setSortingEnabled(true);
    saleTable->selectionModel()->clearSelection();
    saleUpdateTotals();
}

/**
 * Updates the subtotal and total for the current sale.
**/
void NewSaleWindow::saleUpdateTotals()
{
    int subtotal = 0;
    for (int i = 0; i < saleList.size(); i++) {
        subtotal += saleList[i].amount();
    }

    double net = subtotal;
    if (discountSpinBox->value() && subtotal) {
        net = (subtotal - discountSpinBox->value() * subtotal / 100);
    }

    double total = net;
    if (net && taxSpinBox->value()) {
        total = net + taxSpinBox->value() * net / 100;
    }

    subtotalLabel->setText(QString("%1").arg(QLocale().toString(subtotal)));
    totalLabel->setText(QString("<font color='blue'>%1</font>").arg(QLocale().toString(std::round(total))));
}

/**
 * Adds the currently selected product to the current sale.
**/
void NewSaleWindow::add()
{
    if (searchTable->selectionModel()->hasSelection()) {
        int row = searchTable->selectionModel()->selectedRows().first().row();
        int id = searchTable->item(row, 0)->text().toInt();
        Product product = Product::getById(id);
        Sale sale;
        sale.setProduct(product.id());
        sale.setQuantity(quantitySpinBox->value());
        sale.setAmount(quantitySpinBox->value() * product.price());
        saleList.append(sale);
        productQuantities.insert(sale.product(), productQuantities.value(sale.product(), 0) + sale.quantity());
        searchRefresh();
        saleRefresh();
        searchSelectionRefresh();
        saleSelectionRefresh();
        searchTable->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Edits the currently selected current sale.
**/
void NewSaleWindow::edit()
{
    if (saleTable->selectionModel()->hasSelection()) {
        int row = saleTable->selectionModel()->selectedRows().first().row();
        int i = saleTable->item(row, 5)->text().toInt();
        Product product = Product::getById(saleList[i].product());

        QuantityDialog dialog;
        dialog.setWindowTitle(tr("Edit quantity"));
        dialog.quantitySpinBox->setMinimum(1);
        dialog.quantitySpinBox->setMaximum(product.stock() - productQuantities[product.id()] + saleList[i].quantity());
        dialog.quantitySpinBox->setValue(saleList[i].quantity());

        if (dialog.exec()) {
            int addedQuantity = dialog.quantitySpinBox->value() - saleList[i].quantity();

            productQuantities[product.id()] += addedQuantity;
            saleList[i].setQuantity(dialog.quantitySpinBox->value());
            saleList[i].setAmount(dialog.quantitySpinBox->value() * product.price());

            searchRefresh();
            saleRefresh();
            searchTable->selectionModel()->clearSelection();
            saleTable->selectionModel()->clearSelection();
        }
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Removes the currently selected sale.
**/
void NewSaleWindow::remove()
{
    if (saleTable->selectionModel()->hasSelection()) {
        int row = saleTable->selectionModel()->selectedRows().first().row();
        int index = saleTable->item(row, 5)->text().toInt();
        Sale sale = saleList[index];
        productQuantities.insert(sale.product(), productQuantities.value(sale.product(), 0) - sale.quantity());
        saleList.removeAt(index);
        searchRefresh();
        saleRefresh();
        searchTable->selectionModel()->clearSelection();
        saleTable->selectionModel()->clearSelection();
    } else {
        displayError(tr("Nothing selected."));
    }
}

/**
 * Processes/completes the current sale and begins a new one.
**/
void NewSaleWindow::confirmSale()
{
    if (saleList.isEmpty()) {
        return;
    }

    GroupSale groupSale;
    groupSale.setName(customerNameEdit->text().trimmed());
    groupSale.setDiscount(discountSpinBox->value());
    groupSale.setTax(taxSpinBox->value());
    QDateTime dt;
    dt.setTime_t(QDateTime::currentSecsSinceEpoch());
    groupSale.setDate(dt.toString("yyyy-MM-dd hh:mm:ss"));
    groupSale.setShift(mainWindow->currentShift.id());
    groupSale.save();

    for (int i = 0; i < saleList.size(); i++) {
        Sale sale = saleList[i];

        Product product = Product::getById(saleList[i].product());
        product.setStock(product.stock() - sale.quantity());
        product.save();

        sale.setGroup(groupSale.id());
        sale.setIndex(i + 1);
        sale.save();
    }

    groupSale.calculateTotals();

    mainWindow->displayReceipt(groupSale.id());
    mainWindow->groupSaleRefresh();
    mainWindow->saleRefresh();
    mainWindow->productRefresh();
    clearSale();
}

/**
 * Clears the current sale and begins a new one.
**/
void NewSaleWindow::clearSale()
{
    customerNameEdit->clear();
    discountSpinBox->setValue(mainWindow->defaultDiscount);
    taxSpinBox->setValue(mainWindow->defaultTax);
    productQuantities.clear();
    saleList.clear();
    searchRefresh();
    saleRefresh();
    searchTable->selectionModel()->clearSelection();
    saleTable->selectionModel()->clearSelection();
    searchSelectionRefresh();
    saleSelectionRefresh();
}
