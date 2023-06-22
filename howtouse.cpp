#include <QPixMap>
#include <QLabel>
#include <QHBoxLayout>
#include <QScreen>

#include "howtouse.h"

HowToUse::HowToUse(QWidget* parent, Qt::WindowFlags f)
    : QWidget{parent, f}
{
    QHBoxLayout* vbox = new QHBoxLayout(this);

    QLabel* imageLabel = new QLabel(this);
    QPixmap image(":/images/howtouse2.png");
    imageLabel->setPixmap(image);
    imageLabel->setAlignment(Qt::AlignTop);

    QLabel* textLabel = new QLabel(this);
    QString htmlText("<html><head><style>body { text-align: justify; }</style></head><body>"
                    "<p>Програма генерує GeoJSON файл з файлу, який містить GPS координати <b>(1)</b>.</p>"
                    "<p>Для позбавлення атмосферних похибок необхідно надати додаткову інформацію зі стаціонарної точки:<br>"
                    "а) точні значення широти та довготи стац. точки (можна ввести вручну або розрахувати завантаживши файл <b>(2)</b> з GPS координатами, які накопичувались принаймні одну добу);<br>"
                    "б) завантажити файл зі стац. точки <b>(3)</b>, яка отримувала дані у той же проміжок часу, що і розрахункова точка.</p>"
                    "<p>Формат <b>(4)</b> визначає формат запису координат: [Довгота, Широта] чи [Широта, Довгота].</p>"
                    "<p>Тип геометрії <b>(5)</b> визначає як буде виглядати згенерована фігура.</p>"
                    "<p>Створений GeoJSON файл можна завантажити у додатки (QGIS, Google Earth та інші) або на онлайн ресурсах (наприклад, http://geojson.io/).</p>"
                    "</body></html>");

    textLabel->setTextFormat(Qt::RichText);
    textLabel->setText(htmlText);
    textLabel->setMinimumWidth(350);
    textLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    textLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    textLabel->setWordWrap(true);

    vbox->addWidget(imageLabel);
    vbox->addWidget(textLabel);
    vbox->setStretchFactor(textLabel, 1);
}
