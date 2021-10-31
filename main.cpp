#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Icon.png"));
    a.setOrganizationName("mnk7");
    a.setApplicationName("Editor");

    QString currentFile;

    if(argc > 1) {
        currentFile = a.arguments().at(1);
    }

    MainWindow w(currentFile);
    w.show();
    return a.exec();
}
