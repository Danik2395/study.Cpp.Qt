#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv); // Create instance that manages whole program
    MainWindow w;               // Program main window
    w.show();                   // Can show cause of internal mechanisms of Qt. Links all through the system state
    return a.exec();            // Run the program
}
