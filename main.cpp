#include "mainwindow.h"
#include <QApplication>
#include <openssl/aes.h>

#define HEIGHT 629
#define WIDTH 620
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(WIDTH,HEIGHT);
    w.show();

    return a.exec();
}
