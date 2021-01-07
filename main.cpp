#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);

    QTranslator myappTranslator;
    myappTranslator.load(QLocale(), QLatin1String("Editor"), QLatin1String("_"), QLatin1String(":/i18n"));
    a.installTranslator(&myappTranslator);

    MainWindow w;
    w.show();
    return a.exec();
}
