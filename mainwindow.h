#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

class GPScalculator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Language_t {
        EN = 1,
        UA
    };

    explicit MainWindow(QSettings*, QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void changeLanguage(QAction*);

private:
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    void about();
    void howToUse();
    void sizeqwe();

    QSettings* settings;
    GPScalculator* gpscalc;
    QActionGroup* languageGroup;
};

#endif // MAINWINDOW_H
