#include "loginwindow.h"
#include "mainwindow.h"
#include <QtWidgets>
#include <QtSql>

/**
 * Constructs a new LoginWindow and performs initial window setup.
**/
LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /******************************************************************
                                INITIAL SETUP
    ******************************************************************/

    setWindowIcon(QIcon(":/icons/store.png"));
    setMinimumSize(QSize(360, 240));

    hasNoUserAccounts = false;
    connectToDatabase();

    /******************************************************************
                                LOGIN
    ******************************************************************/

    loginUsernameEdit = new QLineEdit;
    loginPasswordEdit = new QLineEdit;
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    loginButton = new QPushButton(QIcon(":/icons/user_ok.png"), tr("Login"));
    loginExitButton = new QPushButton(QIcon(":/icons/exit.png"), tr("Exit"));

    QFormLayout *loginFormLayout = new QFormLayout;
    loginFormLayout->addRow(tr("Username"), loginUsernameEdit);
    loginFormLayout->addRow(tr("Password"), loginPasswordEdit);

    QHBoxLayout *loginButtonLayout = new QHBoxLayout;
    loginButtonLayout->setAlignment(Qt::AlignRight);
    loginButtonLayout->addWidget(loginButton);
    loginButtonLayout->addWidget(loginExitButton);

    QVBoxLayout *loginLayout = new QVBoxLayout;
    loginLayout->addLayout(loginFormLayout);
    loginLayout->addLayout(loginButtonLayout);

    QGroupBox *loginGroup = new QGroupBox;
    loginGroup->setLayout(loginLayout);

    /******************************************************************
                                SIGNUP
    ******************************************************************/

    signupNameEdit = new QLineEdit;
    signupUsernameEdit = new QLineEdit;
    signupPasswordEdit = new QLineEdit;
    signupPasswordEdit->setEchoMode(QLineEdit::Password);
    signupConfirmPasswordEdit = new QLineEdit;
    signupConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    signupButton = new QPushButton(QIcon(":/icons/user_add.png"), tr("Signup"));
    signupExitButton = new QPushButton(QIcon(":/icons/exit.png"), tr("Exit"));

    QFormLayout *signupFormLayout = new QFormLayout;
    signupFormLayout->addRow(tr("Name"), signupNameEdit);
    signupFormLayout->addRow(tr("Username"), signupUsernameEdit);
    signupFormLayout->addRow(tr("Password"), signupPasswordEdit);
    signupFormLayout->addRow(tr("Confirm Password"), signupConfirmPasswordEdit);

    QHBoxLayout *signupButtonLayout = new QHBoxLayout;
    signupButtonLayout->setAlignment(Qt::AlignRight);
    signupButtonLayout->addWidget(signupButton);
    signupButtonLayout->addWidget(signupExitButton);

    QVBoxLayout *signupLayout = new QVBoxLayout;
    signupLayout->addLayout(signupFormLayout);
    signupLayout->addLayout(signupButtonLayout);

    QGroupBox *signupGroup = new QGroupBox;
    signupGroup->setLayout(signupLayout);

    /******************************************************************
                                FINAL SETUP
    ******************************************************************/

    tabWidget = new QTabWidget;
    tabWidget->addTab(loginGroup, QIcon(":/icons/user_ok.png"), tr("Login"));
    tabWidget->addTab(signupGroup, QIcon(":/icons/user_add.png"), tr("Signup"));

    setCentralWidget(tabWidget);

    connect(signupButton, &QPushButton::clicked, this, &LoginWindow::signup);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::login);
    connect(loginExitButton, &QPushButton::clicked, this, &LoginWindow::close);
    connect(signupExitButton, &QPushButton::clicked, this, &LoginWindow::close);

    /******************************************************************
                                FIRST RUN
    ******************************************************************/

    if (hasNoUserAccounts) {
        displayInfo(tr("Since there are no user accounts currently in the database, you need to create a new one. "
                       "This new account will automatically be set up as an administrator account."));
        tabWidget->setCurrentIndex(1);
    }
}

/**
 * Displays a message box with information about an error.
**/
void LoginWindow::displayError(const QString &message)
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
void LoginWindow::displayInfo(const QString &message)
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
 * Connects to the database, creating a new database if required.
 * Returns an error code on errors or QSqlError::NoError on success.
**/
void LoginWindow::connectToDatabase()
{
    // Exit if the SQLite3 driver is not available.
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        displayError(tr("This application needs the SQLITE driver to run properly."));
        exit(EXIT_FAILURE);
    }

    // Open an existing SQLite database or create a new one.
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Retail_Management_System.db";
    db.setDatabaseName(path);

    QString errorMessage = tr("Could not connect to the database.\n\n");

    // Return error code if the database could not be created/opened.
    if (!db.open()) {
        displayError(errorMessage + db.lastError().text());
        exit(EXIT_FAILURE);
    }

    // Return if the database already existed.
    QStringList tables = db.tables();
    if (tables.contains("users", Qt::CaseInsensitive)
            && tables.contains("shifts", Qt::CaseInsensitive)
            && tables.contains("products", Qt::CaseInsensitive)
            && tables.contains("categories", Qt::CaseInsensitive)
            && tables.contains("purchases", Qt::CaseInsensitive)
            && tables.contains("sales", Qt::CaseInsensitive)
            && tables.contains("groupsales", Qt::CaseInsensitive))
        return;

    // Create tables as a new database was created.

    QSqlQuery q;

    if (!q.exec(QLatin1String("CREATE TABLE users("
                                    "id INTEGER PRIMARY KEY ASC, "
                                    "name VARCHAR(255) NOT NULL, "
                                    "username VARCHAR(255) UNIQUE NOT NULL, "
                                    "password VARCHAR(255) NOT NULL, "
                                    "isadmin INTEGER DEFAULT 0"
                              ")"))) {
        displayError(errorMessage + q.lastError().text());
        exit(EXIT_FAILURE);
    }

    if (!q.exec(QLatin1String("CREATE TABLE shifts("
                                    "id INTEGER PRIMARY KEY ASC, "
                                    "cashier INTEGER, "
                                    "start_date DATE NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                                    "end_date DATE NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                                    "FOREIGN KEY (cashier) REFERENCES users(id)"
                              ")"))) {
        displayError(errorMessage + q.lastError().text());
        exit(EXIT_FAILURE);
    }

    if (!q.exec(QLatin1String("CREATE TABLE categories("
                                    "id INTEGER PRIMARY KEY ASC, "
                                    "name VARCHAR(255) NOT NULL"
                              ")"))) {
        displayError(errorMessage + q.lastError().text());
        exit(EXIT_FAILURE);
    }

    if (!q.exec(QLatin1String("CREATE TABLE products("
                                    "id INTEGER PRIMARY KEY ASC, "
                                    "name VARCHAR(255) NOT NULL, "
                                    "category INTEGER, "
                                    "price INTEGER DEFAULT 0, "
                                    "stock INTEGER DEFAULT 0, "
                                    "FOREIGN KEY (category) REFERENCES categories(id)"
                              ")"))) {
        displayError(errorMessage + q.lastError().text());
        exit(EXIT_FAILURE);
    }

    if (!q.exec(QLatin1String("CREATE TABLE purchases("
                                    "id INTEGER PRIMARY KEY ASC, "
                                    "product INTEGER, "
                                    "quantity INTEGER DEFAULT 0, "
                                    "amount INTEGER DEFAULT 0, "
                                    "date DATE NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                                    "FOREIGN KEY (product) REFERENCES products(id)"
                              ")"))) {
        displayError(errorMessage + q.lastError().text());
        exit(EXIT_FAILURE);
    }

    if (!q.exec(QLatin1String("CREATE TABLE groupsales("
                                    "id INTEGER PRIMARY KEY ASC, "
                                    "name VARCHAR(255), "
                                    "discount INTEGER DEFAULT 0, "
                                    "tax INTEGER DEFAULT 0, "
                                    "shift INTEGER, "
                                    "date DATE NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                                    "FOREIGN KEY (shift) REFERENCES shifts(id)"
                              ")"))) {
        displayError(errorMessage + q.lastError().text());
        exit(EXIT_FAILURE);
    }

    if (!q.exec(QLatin1String("CREATE TABLE sales("
                                    "id INTEGER PRIMARY KEY ASC, "
                                    "product INTEGER, "
                                    "quantity INTEGER DEFAULT 0, "
                                    "amount INTEGER DEFAULT 0, "
                                    "groupsale INTEGER, "
                                    "saleindex INTEGER DEFAULT 0, "
                                    "FOREIGN KEY (product) REFERENCES products(id), "
                                    "FOREIGN KEY (groupsale) REFERENCES groupsales(id)"
                              ")"))) {
        displayError(errorMessage + q.lastError().text());
        exit(EXIT_FAILURE);
    }

    hasNoUserAccounts = true;
}

/**
 * Processes user login.
**/
void LoginWindow::login()
{
    bool success = false;
    QString username = loginUsernameEdit->text().trimmed();
    QString hashedPassword = QString(QCryptographicHash::hash(loginPasswordEdit->text().toLatin1(), QCryptographicHash::Md5).toHex());
    User user = User::getByUsername(username);
    if (username.isEmpty() || hashedPassword.isEmpty()) {
        displayError(tr("All fields are required and cannot be empty."));
    } else if (user.id() == 0) {
        displayError(tr("Invalid username."));
    } else if (user.password().compare(hashedPassword) != 0) {
        displayError(tr("Invalid username/password."));
    } else {
        success = true;
    }

    loginPasswordEdit->clear();

    if (success) {
        MainWindow *window = new MainWindow(user);
        window->show();
        hide();
    }
}

/**
 * Processes user registration.
**/
void LoginWindow::signup()
{
    bool success = false;
    QString name = signupNameEdit->text().trimmed();
    QString username = signupUsernameEdit->text().trimmed();
    QString password = signupPasswordEdit->text();
    QString password2 = signupConfirmPasswordEdit->text();
    if (name.isEmpty() || username.isEmpty() || password.isEmpty() || password2.isEmpty()) {
        displayError(tr("All fields are required and cannot be empty."));
    } else if (password.compare(password2) != 0) {
        displayError(tr("Password and confirm password do not match."));
    } else if (User::getByUsername(username).id() != 0) {
        displayError(tr("That username already exists."));
    } else {
        QString hashedPassword = QString(QCryptographicHash::hash(signupPasswordEdit->text().toLatin1(), QCryptographicHash::Md5).toHex());
        User user;
        user.setName(name);
        user.setUsername(username);
        user.setPassword(hashedPassword);
        user.setIsAdmin(hasNoUserAccounts);
        user.save();
        displayInfo(tr("Signup successful. Please login to continue."));
        success = true;
        if (hasNoUserAccounts) {
            hasNoUserAccounts = false;
        }
    }

    signupPasswordEdit->clear();
    signupConfirmPasswordEdit->clear();
    if (success) {
        signupNameEdit->clear();
        signupUsernameEdit->clear();
        loginUsernameEdit->setText(username);
        tabWidget->setCurrentIndex(0);
    }
}
