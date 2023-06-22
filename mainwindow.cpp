#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QActionGroup>

#include "mainwindow.h"
#include "gpscalculator.h"
#include "howtouse.h"

MainWindow::MainWindow(QSettings* S, QWidget* parent)
    : QMainWindow{parent},
    settings{S},
    gpscalc{new GPScalculator(settings, this)}
{
    setCentralWidget(gpscalc);

    createActions();
    createStatusBar();

    readSettings();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    event->accept();
}

void MainWindow::changeLanguage(QAction* action)
{
    action->setChecked(true);
    QMessageBox::information(0, "Information", tr("Restart the app to apply the changes."));
}

void MainWindow::createActions()
{
    QMenu* languageMenu = menuBar()->addMenu(tr("&Language"));
    languageGroup = new QActionGroup(this);
    QAction* englishAct = languageMenu->addAction(tr("English"));
    englishAct->setData(EN);
    QAction* ukrainianAct = languageMenu->addAction(tr("Ukrainian"));
    ukrainianAct->setData(UA);
    languageGroup->addAction(englishAct);
    languageGroup->addAction(ukrainianAct);
    for(QAction* act : languageGroup->actions()) {
        act->setCheckable(true);
    }
    ukrainianAct->setChecked(true);
    connect(languageMenu, SIGNAL(triggered(QAction*)), this, SLOT(changeLanguage(QAction*)));

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* htuAct = helpMenu->addAction(tr("How to use"), this, &MainWindow::howToUse);
    htuAct->setStatusTip(tr("Get help using the app"));
//    QAction* sizeAct = languageMenu->addAction("Size", this, &MainWindow::sizeqwe);

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);

//    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
//    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::howToUse()
{
    HowToUse* htu = new HowToUse(this, Qt::Window);
    htu->setAttribute(Qt::WA_DeleteOnClose);
    htu->setWindowTitle(tr("How to use"));
    htu->setVisible(true);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About GPS-calculator"),
                       tr("The <b>GPS-calculator</b> allows you to adjust GPS coordinates "
                          "using data from an additional GPS sensor."));
}

void MainWindow::sizeqwe()
{
    qDebug() << size();
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    const QByteArray geometry = settings->value("mainWindow/geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    } else {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(430, 424);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }

    const int lang = settings->value("language", 0).toInt();
    if (lang != 0) {
        for(QAction* langAction : languageGroup->actions()) {
            if (langAction->data() == lang) {
                langAction->setChecked(true);
                break;
            }
        }
    }
}

void MainWindow::writeSettings()
{
    settings->setValue("mainWindow/geometry", saveGeometry());
    settings->setValue("language", languageGroup->checkedAction()->data());
}
