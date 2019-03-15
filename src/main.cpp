#include "loginwindow.h"
#include <QApplication>

/**
 * The entry point for the application.
 * Returns 0 if the program exited successfully, or a non-zero error code.
**/
int main(int argc, char *argv[])
{
    int exit_code;
    do {
        // Initialize the application
        QApplication application(argc, argv);
        QApplication::setApplicationName(QCoreApplication::tr("Retail Management System"));
        QApplication::setApplicationVersion("0.0.1");

        // Display the login window
        LoginWindow loginWindow;
        loginWindow.show();

        // Exit code value determines if we are actually exiting or just restarting
        exit_code = application.exec();
    } while (exit_code == 42);

    return exit_code;
}
