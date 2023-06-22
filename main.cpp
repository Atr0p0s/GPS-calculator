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
    const int lang = settings.value("language").toInt();
    if (lang != 0) {
        if (lang == MainWindow::UA) {
            if (translator.load("../GPS-calculator/translations/main_ua.qm")) {
                QCoreApplication::installTranslator(&translator);
            }
        }
    } else {
        if (translator.load("../GPS-calculator/translations/main_ua.qm")) {
            QCoreApplication::installTranslator(&translator);
        }
    }

    MainWindow mainWin(&settings);
    mainWin.show();
    return app.exec();
}
