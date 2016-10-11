#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>

QPushButton *button;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "This is Digital Photo Frame, Welcome";
    MainWindow w;
    w.setWindowFlags(Qt::WindowMaximizeButtonHint);
    w.show();
    qDebug() << "Window is opened";
    QObject::connect(button, SIGNAL(clicked()), &a, SLOT(quit()));
    return a.exec();
}
