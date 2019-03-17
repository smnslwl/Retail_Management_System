QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Retail_Management_System
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    category.cpp \
    product.cpp \
    productdialog.cpp \
    purchasedialog.cpp \
    purchase.cpp \
    purchaseeditdialog.cpp \
    sale.cpp \
    saleeditdialog.cpp \
    newsalewindow.cpp \
    loginwindow.cpp \
    user.cpp \
    userdialog.cpp \
    groupsale.cpp \
    textinputdialog.cpp \
    shift.cpp \
    webviewdialog.cpp \
    spinboxdialog.cpp

HEADERS += \
    mainwindow.h \
    category.h \
    product.h \
    productdialog.h \
    purchasedialog.h \
    purchase.h \
    purchaseeditdialog.h \
    sale.h \
    saleeditdialog.h \
    newsalewindow.h \
    loginwindow.h \
    user.h \
    userdialog.h \
    groupsale.h \
    textinputdialog.h \
    shift.h \
    webviewdialog.h \
    spinboxdialog.h

RESOURCES += \
    Retail_Management_System.qrc

RC_FILE += \
    Retail_Management_System.rc
