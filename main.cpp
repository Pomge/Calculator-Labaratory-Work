#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    MainWindow main_window;
    main_window.setWindowFlag(Qt::FramelessWindowHint);
    main_window.show();
    return application.exec();
}
