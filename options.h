#ifndef OPTIONS_H
#define OPTIONS_H

#include <QMainWindow>

namespace Ui {
class options;
}

class options : public QMainWindow
{
    Q_OBJECT

public:
    explicit options(QWidget *parent = nullptr);
    ~options();

private slots:
    void on_pushButton_Offre_clicked();

    void on_pushButton_Candidat_clicked();

private:
    Ui::options *ui;
};

#endif // OPTIONS_H
