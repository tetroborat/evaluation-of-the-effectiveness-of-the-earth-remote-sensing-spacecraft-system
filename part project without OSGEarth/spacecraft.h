#ifndef C_SPACECRAFT_H
#define C_SPACECRAFT_H

#include <QWidget>
#include <QDateTime>
#include <qwt_plot_curve.h>
#include <qwt_plot_svgitem.h>
#include <qwt_plot_histogram.h>
#include "model_viewing_areaSC.h"
#include "gui3d/osgviewerQt.h"

//enum S_type
//{
//    OEN,
//    RLN
//};
struct S_KEO
{
    double a,e,i,om,Om,tau;
    QDateTime datetime;
};
struct S_VekUkhod
{
    double dOm,dom,dtau;
};
struct S_zone_overview
{
    QDateTime entry_time,exit_time;
    QString name_SC,name_area;
};

class C_SpaceCraft
{
public:
    C_SpaceCraft();
    QString name;
    /*S_type*/ QString type;
    S_KEO KEO;
    S_VekUkhod ukhod;
    double angle1,angle2;//угол полураствора
    QVector <QVector <double> > geo;//вектор географических координат
    QVector <QVector <double> > gsc;//вектор координат в ГСК
    QwtPlotCurve *main_curve = new QwtPlotCurve();
    C_SpaceCraft copy();
};


#endif // C_SPACECRAFT_H
