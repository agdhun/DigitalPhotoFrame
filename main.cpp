#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>

QPushButton *button;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setFixedSize(800,480);
    w.setWindowFlags(Qt::WindowMaximizeButtonHint);
    w.show();
    QObject::connect(button, SIGNAL(clicked()), &a, SLOT(quit()));
    return a.exec();
}
