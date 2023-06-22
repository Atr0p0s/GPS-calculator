#include <QDoubleValidator>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDate>
#include <QRadioButton>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>

#include "Koord/Coord_QW.h"
#include "gpscalculator.h"
#include "lib/NMEA_Struct.h"

GPScalculator::GPScalculator(QSettings* S, QWidget* parent)
    : QWidget{parent}, settings{S}
{
    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addWidget(createDynamicCoordGroup());
    vbox->addWidget(createBenchmarkCoordGroup());
    vbox->addWidget(createResultsGroup());
    setLayout(vbox);
}

//void GPScalculator::closeEvent(QCloseEvent* event)
//{
//    Q_UNUSED(event)
//    if (coord) {
//        coord->close();
//    }
//}

QGroupBox* GPScalculator::createDynamicCoordGroup()
{
    QGroupBox* groupBox = new QGroupBox(tr("GPS Coordinates"), this);

    QLabel* dynamicGPSLabel = new QLabel(tr("File from moving point:"));
    dynamicGPSFileNameLabel = new QLabel(tr("File is not loaded"));

    QPushButton* dynamicGPSButton = new QPushButton(tr("Load File"));
    connect(dynamicGPSButton, &QPushButton::pressed, this,
            [this](){  dynamicFile = QFileDialog::getOpenFileName(this, tr("Open File"), "", /*"NME (*.nme);;*/"All Files (*)");
                if (!dynamicFile.isEmpty()) {
                    dynamicGPSFileNameLabel->setText(QFileInfo(dynamicFile).fileName());
                }
            } );

    QLabel* fileTypeLabel = new QLabel(tr("File type:"));
    type_bgroup = new QButtonGroup(this);
    QRadioButton* nmeaButton = new QRadioButton("NMEA", this);
    QRadioButton* csvButton = new QRadioButton("CSV", this);
    type_bgroup->addButton(nmeaButton, 1);
    type_bgroup->addButton(csvButton, 2);
    nmeaButton->setChecked(true);
    nmeaButton->setEnabled(false);
    csvButton->setEnabled(false);
//QPushButton* createNMEAButton = new QPushButton("create nmea");
//    connect(createNMEAButton, &QPushButton::pressed, this, &GPScalculator::nmeaFromCSV);
    QGridLayout* grid = new QGridLayout();
    grid->addWidget(dynamicGPSLabel, 0, 0, Qt::AlignCenter);
    grid->addWidget(dynamicGPSFileNameLabel, 1, 0, Qt::AlignCenter);
    grid->addWidget(dynamicGPSButton, 2, 0, Qt::AlignCenter);
    grid->addWidget(fileTypeLabel, 0, 1, Qt::AlignCenter);
    grid->addWidget(nmeaButton, 1, 1, Qt::AlignCenter);
    grid->addWidget(csvButton, 2, 1, Qt::AlignCenter);
//    grid->addWidget(createNMEAButton, 3, 1, Qt::AlignCenter);
    groupBox->setLayout(grid);

    return groupBox;
}

//void GPScalculator::nmeaFromCSV()
//{
//    if (dynamicFile.isEmpty()) {
//        return;
//    }
//    QFile fileCSV;
//    fileCSV.setFileName(dynamicFile);
//    if (!fileCSV.open(QIODevice::ReadOnly)) {
//        QMessageBox::critical(0, "Error", "Cannot open source file "+dynamicFile);
//        return;
//    }
//    QList<QByteArray> csvList = fileCSV.readAll().split('\n');
//    for(int i = 0; i < csvList.size(); i++) {
//        if (!csvList[i].isEmpty()) {
//            QList<QByteArray> data = csvList[i].split(',');
//            qDebug() << csvList[i];
//            QByteArray time = data.value(0).split(' ')[3];
//            bool hasLatitude = false;
//            bool hasLongitude = false;
//            double lat = data.value(1).toDouble(&hasLatitude);
//            double lng = data.value(2).toDouble(&hasLongitude);
//            qDebug() << time << lat << lng;
//            qDebug() << "full" << genGGA(time, lat, lng);
//        }
//    }
//}

//QString GPScalculator::genGGA(const QByteArray& time, const double& lat, const double& lng)
//{
//    QList<QByteArray> timeList = time.split(':');
//    QString timeFormatted = QString("%1%2%3.00").arg(timeList[0], 2, '0').arg(timeList[1], 2, '0').arg(timeList[2], 2, '0');

//    uint lat_abs = abs(lat);
//    double lat_min = (abs(lat) - lat_abs) * 60;
//    char lat_pol = (lat > 0)? 'N' : 'S';
//    QString latFormatted = QString("%1%2").arg(lat_abs, 2, 10, QChar('0')).arg(lat_min, -8, 'g', 7, '0');

//    uint lng_abs = abs(lng);
//    double lng_min = (abs(lng) - lng_abs) * 60;
//    char lng_pol = (lng > 0)? 'E' : 'W';
//    QString lngFormatted = QString("%1%2").arg(lng_abs, 3, 10, QChar('0')).arg(lng_min, -8, 'g', 7, '0');

//    QByteArray full = "$GPGGA,"+timeFormatted.toUtf8()+','+latFormatted.toUtf8()+','+lat_pol+','+\
//                            lngFormatted.toUtf8()+','+lng_pol+",1,01,1.0,0.0,M,0.0,M,,*";
//    int crc = 0;
//    for (qsizetype i = 1; i < full.size()-1; ++i) {
//        crc = crc ^ full.at(i);
//    }
//    crc = crc & 0xFF;

////    QString crcStr = QString::asprintf("%0.2X", crc);
////    full.append(crcStr.toUtf8());
//    full.append(QString::asprintf("%.2X", crc).toUtf8());
////    full.append(QByteArray::number(crc, 16).toUpper());
//    full.append("\r\n");
//    return full;
//}

QGroupBox* GPScalculator::createBenchmarkCoordGroup()
{
    QGroupBox* groupBox = new QGroupBox(tr("Benchmark Coordinates"), this);

    latitude_lineEdit = new QLineEdit();
    latitude_lineEdit->setPlaceholderText("12.3456789");
    latitude_lineEdit->setValidator(new QDoubleValidator(-90, 90, 10));
    QLabel* latitudeLabel = new QLabel(tr("&Latitude:"));
    latitudeLabel->setBuddy(latitude_lineEdit);

    longitude_lineEdit = new QLineEdit();
    longitude_lineEdit->setPlaceholderText("12.3456789");
    longitude_lineEdit->setValidator(new QDoubleValidator(-180, 180, 10));
    QLabel* longitudeLabel = new QLabel(tr("L&ongitude:"));
    longitudeLabel->setBuddy(longitude_lineEdit);

    QPushButton* benchmarkFileButton = new QPushButton(tr("Calculate from file"));
    connect(benchmarkFileButton, SIGNAL(pressed()), this, SLOT(slotOpenBenchmarkFile()));

    QLabel* staticGPSLabel = new QLabel(tr("File from static point:"));
    staticGPSFileNameLabel = new QLabel(tr("File is not loaded"));
    QPushButton* staticGPSButton = new QPushButton(tr("Load File"));
    connect(staticGPSButton, &QPushButton::pressed, this,
            [this](){  staticFile = QFileDialog::getOpenFileName(this, tr("Open File"), "", /*"NME (*.nme);;*/"All Files (*)");
                if (!staticFile.isEmpty()) {
                    staticGPSFileNameLabel->setText(QFileInfo(staticFile).fileName());
                }
            } );

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(latitudeLabel, 0, 0);
    grid->addWidget(latitude_lineEdit, 1, 0);
    grid->addWidget(longitudeLabel, 0, 1);
    grid->addWidget(longitude_lineEdit, 1, 1);
    grid->addWidget(benchmarkFileButton, 2, 0, 1, 2);
    grid->addWidget(staticGPSLabel, 0, 2);
    grid->addWidget(staticGPSFileNameLabel, 1, 2, Qt::AlignCenter);
    grid->addWidget(staticGPSButton, 2, 2);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);
    groupBox->setLayout(grid);

    return groupBox;
}

QGroupBox* GPScalculator::createResultsGroup()
{
    QGroupBox* groupBox = new QGroupBox(tr("Calculations"), this);

    QLabel* formatLabel = new QLabel(tr("Format:"));
    format_bgroup = new QButtonGroup(this);
    QRadioButton* latFirstButton = new QRadioButton(tr("Latitude first"), this);
    QRadioButton* longFirstButton = new QRadioButton(tr("Longitude first"), this);
    format_bgroup->addButton(latFirstButton, 1);
    format_bgroup->addButton(longFirstButton, 2);
    longFirstButton->setChecked(true);

    QLabel* geometryLabel = new QLabel(tr("Geometry type:"));
    geometry_bgroup = new QButtonGroup(this);
    QRadioButton* lineButton = new QRadioButton(tr("Line"), this);
    QRadioButton* polygonButton = new QRadioButton(tr("Polygon"), this);
    geometry_bgroup->addButton(lineButton, 1);
    geometry_bgroup->addButton(polygonButton, 2);
    lineButton->setChecked(true);

    QPushButton* saveButton = new QPushButton(tr("Save as..."));
    connect(saveButton, SIGNAL(pressed()), this, SLOT(slotCalculateGPS()));
    QPushButton* mapButton = new QPushButton(tr("Show map"));
    connect(mapButton, SIGNAL(pressed()), this, SLOT(slotShowMap()));

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(formatLabel, 0, 0);
    grid->addWidget(latFirstButton, 1, 0);
    grid->addWidget(longFirstButton, 2, 0);
    grid->addWidget(geometryLabel, 0, 1);
    grid->addWidget(lineButton, 1, 1);
    grid->addWidget(polygonButton, 2, 1);
    grid->addWidget(saveButton, 3, 0);
    grid->addWidget(mapButton, 3, 1);
    groupBox->setLayout(grid);

    return groupBox;
}

void GPScalculator::slotOpenBenchmarkFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", /*"NME (*.nme);;*/"All Files (*)");
    if (fileName != "") {
        QFile file;
        file.setFileName(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Error", tr("Cannot open source file ")+fileName);
        } else {
            coord = new Coord_QW(settings, this, Qt::Window);
            coord->setAttribute(Qt::WA_DeleteOnClose);
            coord->setWindowTitle(tr("Benchmark point"));
            coord->setVisible(true);
            QByteArray data = file.readAll();
            QString Text_NMEA = QString::fromLocal8Bit(data);
            QStringList Text_NMEA_List = Text_NMEA.split("\n");

            struct NMEA_Data NMEA_D, NMEA_Data_Settings;
            memset(&NMEA_Data_Settings, 0, sizeof(struct NMEA_Data));
            NMEA_Data_Settings.GPGGA.time = 1;
            BmUartProtoNmea ctx;
            memset(&ctx, 0, sizeof(BmUartProtoNmea));

            for(int i = 0; i < Text_NMEA_List.size(); i++) {
                if(Text_NMEA_List[i].size() > 0) {
                    QString Z = *(Text_NMEA_List[i].end() - 1);

                    if(Z != "\r") {
                        Text_NMEA_List[i] += "\r";
                    }

                    Text_NMEA = Text_NMEA_List[i] + "\n";

                    NMEA_D = NMEA_Data_Settings;
                    char data2[4096];
                    memset(data2, 0, 4096);
//                    strcpy(data2, Text_NMEA.toLocal8Bit().data());
                    strncpy(data2, Text_NMEA.toLocal8Bit().data(), sizeof(data2));
                    nmea_recv(&ctx, data2, strlen(data2), &NMEA_D);
                    if(NMEA_D.GPGGA.latitude && NMEA_D.GPGGA.longitude) {
                        coord->Parse_GPGGA_Slot(NMEA_D.GPGGA);
                    }
                }
            }
            file.close();
            m_benchmark_latitude = coord->getAverageLat();
            m_benchmark_longitude = coord->getAverageLong();
            latitude_lineEdit->setText(QString::number(m_benchmark_latitude, 'g', 9));
            longitude_lineEdit->setText(QString::number(m_benchmark_longitude, 'g', 9));
        }
    }
}

void GPScalculator::createData(const QString& fileName, QMap<int, QList<double> >* pmap)
{
    if (fileName != "") {
        QFile file;
        file.setFileName(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Error", tr("Cannot open source file ")+fileName);
        } else {
            QByteArray data = file.readAll();
            QString Text_NMEA = QString::fromLocal8Bit(data);
            QStringList Text_NMEA_List = Text_NMEA.split("\n");

            struct NMEA_Data NMEA_D, NMEA_Data_Settings;
            memset(&NMEA_Data_Settings, 0, sizeof(struct NMEA_Data));
            NMEA_Data_Settings.GPGGA.time = 1;
            BmUartProtoNmea ctx;
            memset(&ctx, 0, sizeof(BmUartProtoNmea));

            for(int i = 0; i < Text_NMEA_List.size(); i++) {
                if(Text_NMEA_List[i].size() > 0) {
                    QString Z = *(Text_NMEA_List[i].end() - 1);

                    if(Z != "\r") {
                        Text_NMEA_List[i] += "\r";
                    }

                    Text_NMEA = Text_NMEA_List[i] + "\n";

                    NMEA_D = NMEA_Data_Settings;
                    char data2[4096];
                    memset(data2, 0, 4096);
                    strncpy(data2, Text_NMEA.toLocal8Bit().data(), sizeof(data2));
                    nmea_recv(&ctx, data2, strlen(data2), &NMEA_D);
                    if(NMEA_D.GPGGA.latitude && NMEA_D.GPGGA.longitude) {
                        (*pmap)[static_cast<int>(NMEA_D.GPGGA.time)] = QList<double>{NMEA_D.GPGGA.latitude, NMEA_D.GPGGA.longitude};
                    }
                }
            }
            file.close();
        }
    }
}

void GPScalculator::slotCalculateGPS()
{
    if (!dynamicFile.isEmpty()) {
        if (!staticFile.isEmpty()) {
            if (!(latitude_lineEdit->text().isEmpty() && longitude_lineEdit->text().isEmpty())) {
                if (latitude_lineEdit->hasAcceptableInput() && longitude_lineEdit->hasAcceptableInput()) {
                    calcCoordinates();
                    saveToFile();
                } else {
                    QMessageBox::information(0, "Information", tr("Unacceptable benchmark coordinates"));
                }
            } else {
                int n = QMessageBox::warning(0, "Warning", tr("Benchmark coodinates were not provided."
                                                              "\nDo you want to generate GeoJSON without adjusting the coordinates?"),
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if (n == QMessageBox::Yes) {
                    saveToFile();
                }
            }
        } else if (!(latitude_lineEdit->text().isEmpty() && longitude_lineEdit->text().isEmpty())) {
            int n = QMessageBox::warning(0, "Warning",
                                         tr("A File to refine the coordinates was not provided."
                                            "\nDo you want to generate GeoJSON without adjusting the coordinates?"),
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (n == QMessageBox::Yes) {
                saveToFile();
            }
        } else {
            QMessageBox::information(0, "Information", tr("GeoJSON will be generated without "
                                                          "adjusting the coordinates"));
            saveToFile();
        }
    } else {
        QMessageBox::information(0, "Information", tr("Load a file for generating GeoJSON from it."));
    }
}

void GPScalculator::calcCoordinates()
{
    createData(dynamicFile, &m_dynamic_data);
    createData(staticFile, &m_static_data);
    m_benchmark_latitude = latitude_lineEdit->text().toDouble();
    m_benchmark_longitude = longitude_lineEdit->text().toDouble();
    for (auto i = m_dynamic_data.begin(), end = m_dynamic_data.end(); i != end; ++i) {
        if (m_static_data.contains(i.key())) {
            i.value()[0] += m_benchmark_latitude - m_static_data.value(i.key()).at(0);
            i.value()[1] += m_benchmark_longitude - m_static_data.value(i.key()).at(1);
        } else {
            m_dynamic_data.remove(i.key());
        }
    }
}

void GPScalculator::saveToFile()
{
    if (m_dynamic_data.isEmpty()) {
        createData(dynamicFile, &m_dynamic_data);
    }
    QString date = QDate::currentDate().toString("yyyy.MM.dd");

#ifdef linux
    QString time = QTime::currentTime().toString("hh:mm");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), date + "_" + time + ".geojson",
                                                    "GeoJSON File(*.json)");
#else
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), date + ".geojson",
                                                    "GeoJSON File(*.geojson)");
#endif

    if (fileName != "") {
        if (QFileInfo(fileName).suffix().isEmpty()) {
            fileName.append(".geojson");
        }
        QFile file;
        file.setFileName(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(0, "Error", tr("Cannot open source file ")+fileName);
        } else {
            QTextStream stream(&file);
            stream.setRealNumberPrecision(13);
            stream << "{\n  \"type\": \"FeatureCollection\",\n  \"features\": [\n";
            stream << "    {\n      \"type\": \"Feature\",\n      \"properties\": {},\n";
            stream << "      \"geometry\": {\n        \"type\": \"";
            switch (geometry_bgroup->checkedId()) {
            case 1:
                stream << "LineString";
                break;

            case 2:
                stream << "Polygon";
                break;
            }
            stream << "\",\n        \"coordinates\": [";
            if (geometry_bgroup->checkedId() == 2) {
                stream << "\n          [";
            }

            int index_1 = (0x01 & format_bgroup->checkedId())^0x01;
            int index_2 = 0x01 & format_bgroup->checkedId();
            bool comma = false;
            int count = 0;
            for (auto i = m_dynamic_data.cbegin(), end = m_dynamic_data.cend(); i != end; ++i) {
                if (comma) {
                    stream << ",";
                }
                if (count++ % 4 == 0) {
                    stream << "\n          ";
                }
                stream << "[" << i.value()[index_1] << "," << i.value()[index_2] << "]";
                comma = true;
            }
            if (geometry_bgroup->checkedId() == 2) {
                if (m_dynamic_data.first() != m_dynamic_data.last()) {
                    stream << ",[" << m_dynamic_data.first()[index_1] << "," << m_dynamic_data.first()[index_2] << "]";
                }
                stream << "\n          ]";
            }
            stream << "\n        ]\n      }\n    }\n  ]\n}\n";

            file.close();
        }
    }
}

void GPScalculator::slotShowMap()
{
    QString mapUrl = "http://geojson.io";
    if (!(latitude_lineEdit->text().isEmpty() && longitude_lineEdit->text().isEmpty())) {
        mapUrl += "/#map=5/" + latitude_lineEdit->text() + "/" + longitude_lineEdit->text();
    }
    QDesktopServices::openUrl(QUrl(mapUrl));
}
