#ifndef C_2D_H
#define C_2D_H

#include <QWidget>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include "2d.h"
#include "ui_2d.h"
#include <qwt_plot_svgitem.h>

#include <qwt_plot_grid.h>

#include <qwt_legend.h>


#include <qwt_plot_magnifier.h>

#include <qwt_plot_panner.h>

#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

#include "QTimer"
#include "QTime"
#include <QDebug>
#include "calculation.h"
#include "constants.h"



namespace Ui {
class C_2d;
}

class C_2d : public QWidget
{
    Q_OBJECT

public:
    explicit C_2d(QWidget *parent = 0);
    QTimer *timer;
    QwtPlot *d_plot = new QwtPlot();
    QVector <QwtPlotCurve*> curves;
    void clear_gra();
    void interpol_move(double x1, double x2, QVector <double> y1, QVector <double> y2,double x);
    ~C_2d();

public slots:
    void paint_main_cur(QPolygonF points,int i);
    void attach_paint_main_cur(C_SpaceCraft SC);
    void attach_paint_main_cur();
    void object_icon_display(QVector<double> geo);
    void SC_icon_display(QVector<double> geo, int i ,QString type);
    void attach_icon();

private:
    void create_plot();
    Ui::C_2d *ui;
    QVector <QwtPlotSvgItem*> icons;
    double size_satellite = 7.5;
    double size_object = 7.5;
    QwtPlotSvgItem* satelliteImage = new QwtPlotSvgItem();
    //QwtSymbol satellite;


};

#endif // C_2D_H
