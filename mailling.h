#ifndef MAILLING_H
#define MAILLING_H

#include <QMainWindow>

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
};

#endif // MAILLING_H
