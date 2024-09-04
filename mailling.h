#ifndef MAILLING_H
#define MAILLING_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QDate>
#include <QtDebug>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>

#include "offre.h"

namespace Ui {
class Mailling;
}

class Mailling : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mailling(QWidget *parent = nullptr);
    ~Mailling();

private:
    Ui::Mailling *ui;
    void sendMail();
    void mailSent(QString);
    void browse();


     QStringList files;
     QtCharts::QPieSeries *PieSeries;
     QtCharts::QChartView *ChartView;

};

#endif // MAILLING_H
