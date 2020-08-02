#ifndef C_3D_H
#define C_3D_H
#include "qstatusbar.h"
#include "spacecraft.h"

#include <QWidget>

namespace Ui {
class C_3d;
}

class C_3d : public QWidget
{
    Q_OBJECT

public:
    explicit C_3d(QWidget *parent = 0);
    void addModelViewingAreaSC(QString name, QVector<double> gsc_SC, QVector<double> geo_SC, QString type, double angle1, double angle2);
    void addImage(QString name, QVector<double> gsc_SC, QVector<double> geo_SC, QString type);
    void addObject(QVector<double> geo,QString name,QString icon);
    void setDataTime(QDateTime time);
    void moveConus_OEN(QVector<double> gsc_SC, QVector<double> geo_SC, int i);
    void moveConus_RTN(QVector<double> gsc_SC, QVector<double> geo1_SC, QVector<double> geo2_SC, int i);
    ~C_3d();
    Ui::C_3d *ui;
     QStatusBar* br ;
private:

};

#endif // C_3D_H
