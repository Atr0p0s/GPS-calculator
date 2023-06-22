#include "Coord_QW.h"
#include <QFont>
#include <math.h>
#include <QByteArray>

Coord_QW::Coord_QW(QSettings* S, QWidget* parent, Qt::WindowFlags f) :
    QWidget(parent, f), settings{S}, A_Lat{0}, A_Long{0}, size{0}
{
    QLabel* Time_L         = new QLabel(tr("Time:"));
    QLabel* Satelits_L     = new QLabel(tr("Satelits:"));
    QLabel* Current_Coord  = new QLabel(tr("Current coordinates: "));

    FON.setBold(true);
    Current_Coord->setFont(FON);
    QLabel* Lat_L          = new QLabel(tr("Latitude: "));
    QLabel* Long_L         = new QLabel(tr("Longitude: "));
    QLabel* Average_Coord  = new QLabel(tr("Average coordinates: "));
    Average_Coord->setFont(FON);
    QLabel* Average_Lat_L  = new QLabel(tr("Latitude: "));
    QLabel* Average_Long_L = new QLabel(tr("Longitude: "));
    QLabel* RMS_L          = new QLabel(tr("RMSD: "));
    QLabel* Scale          = new QLabel(tr("Scale: "));

    Time_VL         = new QLabel("0");
    Satelits_VL     = new QLabel("0");
    Dost_VL         = new QLabel("___________________");
    Long_VL         = new QLabel("0");
    Lat_VL          = new QLabel("0");
    Average_Long_VL = new QLabel("0");
    Average_Lat_VL  = new QLabel("0");
    RMS_VL          = new QLabel("0");

    QLabel* Lat_Spin_Label = new QLabel(tr("Lat:"));
    Lat_Spinbox = new QDoubleSpinBox();
        Lat_Spinbox->setMaximum(-90);
        Lat_Spinbox->setMaximum(90);
        Lat_Spinbox->setMinimumWidth(100);
        Lat_Spinbox->setDecimals(7);
        Lat_Spinbox->setSingleStep(0.0000001);

    QLabel* Long_Spin_Label = new QLabel(tr("Long:"));
    Long_Spinbox = new QDoubleSpinBox();
        Long_Spinbox->setMaximum(-180);
        Long_Spinbox->setMaximum(180);
        Long_Spinbox->setMinimumWidth(100);
        Long_Spinbox->setDecimals(7);
        Long_Spinbox->setSingleStep(0.0000001);

    QPushButton* Set_Center_Button = new QPushButton(tr("Set Center"));

    Scale_Spinbox    = new QDoubleSpinBox();
       Scale_Spinbox->setMaximum(100000);
       Scale_Spinbox->setMinimum(0.1);
       Scale_Spinbox->setValue(2);
//       Scale_Spinbox->setFixedWidth(100);

    QPushButton* Clear_B = new QPushButton(tr("Clear"));

    QGridLayout* grid_l = new QGridLayout();
    grid_l->addWidget(Time_L,           1, 0);
    grid_l->addWidget(Time_VL,          1, 1);
    grid_l->addWidget(Satelits_L,       2, 0);
    grid_l->addWidget(Satelits_VL,      2, 1);
    grid_l->addWidget(Dost_VL,          3, 0, 1, 2);
    grid_l->addWidget(Current_Coord,    4, 0, 1, 2);
    grid_l->addWidget(Lat_L,            5, 0);
    grid_l->addWidget(Lat_VL,           5, 1);
    grid_l->addWidget(Long_L,           6, 0);
    grid_l->addWidget(Long_VL,          6, 1);
    grid_l->addWidget(Average_Coord,    7, 0, 1, 2);
    grid_l->addWidget(Average_Lat_L,    8, 0);
    grid_l->addWidget(Average_Lat_VL,   8, 1);
    grid_l->addWidget(Average_Long_L,   9, 0);
    grid_l->addWidget(Average_Long_VL,  9, 1);
    grid_l->addWidget(Set_Center_Button,10, 0);
    grid_l->addWidget(RMS_L,            11, 0);
    grid_l->addWidget(RMS_VL,           11, 1);
    grid_l->addWidget(Scale,            12, 0);
    grid_l->addWidget(Scale_Spinbox,    12, 1);
    grid_l->addWidget(Clear_B,          13, 0);

    visual_coord = new Visual_Coord();

    QGridLayout* grid_Graf = new QGridLayout();
        grid_Graf->addWidget(visual_coord,     0, 0, 1, 4);
        grid_Graf->addWidget(Lat_Spin_Label,   1, 0, Qt::AlignRight);
        grid_Graf->addWidget(Lat_Spinbox,      1, 1, Qt::AlignLeft);
        grid_Graf->addWidget(Long_Spin_Label,  1, 2, Qt::AlignRight);
        grid_Graf->addWidget(Long_Spinbox,     1, 3, Qt::AlignLeft);
        grid_Graf->setColumnMinimumWidth(0, 30);
        grid_Graf->setColumnStretch(1, 2);
        grid_Graf->setColumnStretch(3, 2);

    QHBoxLayout* qhb = new QHBoxLayout();
    qhb->addLayout(grid_Graf);
    qhb->addLayout(grid_l);
    qhb->addStretch(1);
    setLayout(qhb);

    visual_coord->Set_Center_Lat_Slot(Lat_Spinbox->value());
    visual_coord->Set_Center_Long_Slot(Long_Spinbox->value());

//    setFixedSize(525, 364);

    connect(Scale_Spinbox,  SIGNAL(valueChanged(double)),    visual_coord,  SLOT(Change_Mash_Slot(double)));
    connect(Clear_B,        SIGNAL(clicked()),               visual_coord,  SLOT(Clear_Slot()));
    connect(Clear_B,        SIGNAL(clicked()),               this,          SLOT(Clear_Slot()));
    connect(Lat_Spinbox,    SIGNAL(valueChanged(double)),    visual_coord,  SLOT(Set_Center_Lat_Slot(double)));
    connect(Long_Spinbox,   SIGNAL(valueChanged(double)),    visual_coord,  SLOT(Set_Center_Long_Slot(double)));
    connect(Set_Center_Button, SIGNAL(clicked()),            this,          SLOT(Set_Center()));

    Read_Setting();
}

Coord_QW::~Coord_QW()
{
    settings->beginGroup("/Coord");
//        settings->setValue("/Lat",     Lat_Spinbox->value());
//        settings->setValue("/Long",    Long_Spinbox->value());
        settings->setValue("/Scale",   Scale_Spinbox->value());
    settings->setValue("/geometry", saveGeometry());
    settings->endGroup();
}

void Coord_QW::Read_Setting()
{
    settings->beginGroup("/Coord");
//        Lat_Spinbox  ->setValue(settings->value("/Lat").toDouble());
//        Long_Spinbox ->setValue(settings->value("/Long").toDouble());
        Scale_Spinbox->setValue(settings->value("/Scale").toDouble());
    const QByteArray geometry = settings->value("/geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    settings->endGroup();
}

void Coord_QW::Parse_GPGGA_Slot(const struct GPGGA &GPGGA)
{
    this->Time = QString::number(GPGGA.time);

    if(GPGGA.time < 100000)
        Time.push_front('0');
    Time.push_back("00");

      //  Time[9] = Time[7];
       // Time[8] = Time[6];
    Time[7] = Time[5];
    Time[6] = Time[4];
    Time[5] = ':';
    Time[4] = Time[3];
    Time[3] = Time[2];
    Time[2] = ':';
    Time.push_back("  ");

    Time_VL->setText(this->Time);

    this->Lat   = QString::number(GPGGA.latitude, 'g', 9) +GPGGA.lat;
    Lat_VL->setText(this->Lat);

    this->Long  = QString::number(GPGGA.longitude, 'g', 9) + GPGGA.lon;
    Long_VL->setText(this->Long);

    this->Satelits = QString::number(GPGGA.nka);
    Satelits_VL->setText(this->Satelits);

    this->A_Lat  = (GPGGA.latitude  + this->A_Lat*size) /(size + 1);
    this->A_Long = (GPGGA.longitude + this->A_Long*size)/(size + 1);
    size++;

    visual_coord->Set_Average_Slot(A_Lat, A_Long);

    this->Average_Lat  = QString::number(A_Lat, 'g', 9);
    Average_Lat_VL->setText(Average_Lat + GPGGA.lat);

    this->Average_Long = QString::number(A_Long, 'g', 9);
    Average_Long_VL->setText(Average_Long + GPGGA.lon);

    double RMS_Lat =  (this->A_Lat  - GPGGA.latitude)*Grad_to_M;
    double RMS_Long = (this->A_Long - GPGGA.longitude)* Grad_to_M*cos(this->A_Lat);

    double Distance = RMS_Lat*RMS_Lat + RMS_Long*RMS_Long;

    RMS = (Distance + RMS*(size-1))/size;

    RMS_VL->setText(QString::number(sqrt(RMS), 'g', 2));

    switch(GPGGA.observation)
    {
        case 0:
            this->Dost = tr("Fix not available\n or invalid.");
            break;
        case 1:
            this->Dost = tr("Position fix valid.\nAutonomous mode.");
            break;
        case 2:
            this->Dost = tr("Position fix valid.\nDifferential mode.");
            break;
        case 3:
            this->Dost = tr("Estimated data\n(extrapolation, dead reckoning)");
            break;
     }
    Dost_VL->setText(this->Dost);
    Dost_VL->setFont(FON);

    visual_coord->animate(GPGGA);
    Set_Center();
}

void Coord_QW::Clear_Slot()
{
    this->A_Lat = 0;
    this->A_Long = 0;
    this->size = 0;

    Time_VL         ->setText("0");
    Long_VL         ->setText("0");
    Lat_VL          ->setText("0");
    Satelits_VL     ->setText("0");
    Dost_VL         ->setText(tr("___________________"));
    Average_Long_VL ->setText("0");
    Average_Lat_VL  ->setText("0");
    RMS_VL          ->setText("0");

    visual_coord->Set_Average_Slot(0, 0);
    visual_coord->repaint();
}

void Coord_QW::Set_Center()
{
    visual_coord->Set_Center_Lat_Slot(A_Lat);
    visual_coord->Set_Center_Long_Slot(A_Long);

    Lat_Spinbox ->setValue(A_Lat);
    Long_Spinbox->setValue(A_Long);
}
