#ifndef GPSCALCULATOR_H
#define GPSCALCULATOR_H

#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QSettings>
#include <QPointer>
#include <QButtonGroup>
#include <QLabel>

class Coord_QW;

class GPScalculator : public QWidget
{
    Q_OBJECT
public:
    explicit GPScalculator(QSettings*, QWidget* parent = nullptr);
    ~GPScalculator();

private slots:
    void slotOpenBenchmarkFile();
    void slotCalculateGPS();
    void slotShowMap();
    //    void nmeaFromCSV();

private:
    QGroupBox* createDynamicCoordGroup();
    QLabel* dynamicGPSFileNameLabel;
    QString dynamicFile;
    QButtonGroup* type_bgroup;

    QGroupBox* createBenchmarkCoordGroup();
    QLineEdit* longitude_lineEdit;
    QLineEdit* latitude_lineEdit;
    QLabel* staticGPSFileNameLabel;
    QString staticFile;

    QGroupBox* createResultsGroup();
    QButtonGroup* format_bgroup;
    QButtonGroup* geometry_bgroup;

    double m_benchmark_longitude;
    double m_benchmark_latitude;
    QMap<int, QList<double> > m_dynamic_data;
    QMap<int, QList<double> > m_static_data;

    QPointer<Coord_QW> coord;
    void calcCoordinates();
    void saveToFile();
    void createData(const QString&, QMap<int, QList<double> >*);

    QSettings* settings;
    void readSettings();
    void writeSettings();
//    QString genGGA(const QByteArray& time, const double& lat, const double& lng);
};

#endif // GPSCALCULATOR_H
