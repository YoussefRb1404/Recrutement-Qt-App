#ifndef GESOFF_H
#define GESOFF_H

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

QT_BEGIN_NAMESPACE
namespace Ui { class GesOff; }
QT_END_NAMESPACE

class GesOff : public QMainWindow
{
    Q_OBJECT

public:
    explicit GesOff(QWidget *parent = nullptr);  // Utilisation de 'explicit' pour éviter les conversions implicites
    ~GesOff();

private slots:
    void on_pushButton_ajoutrO_clicked();  // Vérifiez l'orthographe de la méthode (ajoutrrf -> ajouter)

    void on_pushButton_supprimerO_clicked();

    void on_pushButton_modifierO_clicked();

    void on_pushButton_confirmer_clicked();

private:
    Ui::GesOff *ui;
    offre of;
    QByteArray data;
    QtCharts::QPieSeries *PieSeries;
    QtCharts::QChartView *ChartView;
    void rechercherOffreParId(int id);
};

#endif // GESOFF_H
