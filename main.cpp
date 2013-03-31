#include <QtGui/QApplication>
#include <QPlastiqueStyle>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();
    QObject::connect(&w, SIGNAL(appQuit()), &app, SLOT(quit()));

    QApplication::setStyle(new QPlastiqueStyle);

    return app.exec();
}
