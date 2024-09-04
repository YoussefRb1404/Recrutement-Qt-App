#ifndef GESCOND_H
#define GESCOND_H

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
#include <QSortFilterProxyModel>

#include "camera.h"
#include "candidat.h"
#include "exportexcelobject.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GesCond; }
QT_END_NAMESPACE

class GesCond : public QMainWindow
{
    Q_OBJECT

public:
    GesCond(QWidget *parent = nullptr);
    ~GesCond();

private slots:
    void on_pushButton_ajoutrC_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_supprimerC_clicked();

    void show_tables();

    void show_tablesA();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_modifierC_clicked();

    void on_pushButton_rechercheC_clicked();

    void on_pushButton_triC_clicked();

    void on_pushButton_stats_clicked();

    void on_pushButton_refresh_clicked();

    void on_pushButton_excel_clicked();

    void on_pushButton_ArchiverC_clicked();

    void on_table_clicked(const QModelIndex &index);

    void on_pushButtonDesarchiver_clicked();

    void on_pushButton_camera_clicked();

private:
    Ui::GesCond *ui;
    candidat cad;
    camera *c;
    int selected;
    QByteArray data;
    void rechercherCandidatParId(int id);
    void rechercherCandidatParDate(const QDate &dateN);
    QtCharts::QPieSeries *PieSeries;
    QtCharts::QChartView *ChartView;
    candidat tmp;
    QSortFilterProxyModel *proxy;
    QList<candidat> candidatsActifs;
    QList<candidat> candidatsArchives;

};
#endif // GESCOND_H
