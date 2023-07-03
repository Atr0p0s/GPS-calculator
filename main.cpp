#include <QApplication>
#include <QTranslator>
#include <QSettings>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Sigma");
    QCoreApplication::setApplicationName("GPS-calculator");
    QTranslator translator;

    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int lang = settings.value("language", -1).toInt();
    if (lang == -1 || lang == MainWindow::UA) {
        if (translator.load(":/translations/main_ua.qm")) {
            QCoreApplication::installTranslator(&translator);
        }
    }

    MainWindow mainWin(&settings);
    mainWin.show();
    return app.exec();
}
