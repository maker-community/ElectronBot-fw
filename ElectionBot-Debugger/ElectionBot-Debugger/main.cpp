#include "mainwindow.h"
#include <QApplication>
#include <QThread>
#include "usb_user.h"
#include "protocol.h"

//MainWindow w;
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
