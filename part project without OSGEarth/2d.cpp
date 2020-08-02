#include "2d.h"
#include "ui_2d.h"

C_2d::C_2d(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_2d)
{
    ui->setupUi(this);
    d_plot->setAutoReplot(true);
    create_plot();
    //main_curve->attach( d_plot ); // отобразить кривую на графике
}



void C_2d::clear_gra()
{
    d_plot->detachItems(QwtPlotItem::Rtti_PlotCurve, false);
}

void C_2d::interpol_move(double x1, double x2, QVector <double> y1, QVector <double> y2,double x)
{
//    inter_points<<QPointF(C_calculation().line_int(x1,x2,y1[0],y2[0],x),C_calculation().line_int(x1,x2,y1[1],y2[1],x));
//    satelliteImage->loadFile(QRectF(inter_points.operator [](inter_points.size()-1).x()-size_satellite/2,inter_points.operator [](inter_points.size()-1).y()-size_satellite/2, size_satellite, size_satellite), "./satellite.svg");

//        inter_curve->setSamples(inter_points ); // ассоциировать набор точек с кривой
//          inter_curve->attach(d_plot ); // отобразить кривую на графике
//        d_plot->replot();
}
void C_2d::paint_main_cur(QPolygonF points,int i)
{
    curves[i]->setSamples( points ); // ассоциировать набор точек с кривой
    d_plot->replot();
}

void C_2d::attach_paint_main_cur(C_SpaceCraft SC)
{
    SC.main_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    SC.main_curve->setPen(QPen(Qt::gray,0.4,Qt::SolidLine));
    SC.main_curve->attach(d_plot);
}

void C_2d::attach_paint_main_cur()
{
    QwtPlotCurve * curve = new QwtPlotCurve();
    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curve->setPen(QPen(Qt::gray,0.4,Qt::SolidLine));
    curve->attach(d_plot);
    curves.append(curve);
}

void C_2d::create_plot()
{
    // Создать поле со шкалами для отображения графика
    QwtPlotSvgItem* mapItem = new QwtPlotSvgItem();
    mapItem->loadFile(QRectF(-180 , -90.0, 360.0, 180.0), "./map.svg");
    mapItem->attach(d_plot);
    ui->horizontalLayout->addWidget(d_plot);// привязать поле к границам окна
    d_plot->setTitle( "Трасса полета" ); // заголовок

    // Параметры осей координат
    d_plot->setAxisTitle(QwtPlot::yLeft, "Широта, град");
    d_plot->setAxisTitle(QwtPlot::xBottom, "Долгота, град");
    d_plot->insertLegend( new QwtLegend() );

//    inter_curve->attach(d_plot ); // отобразить кривую на графике
//    inter_curve->setSamples(inter_points ); // ассоциировать набор точек с кривой

    d_plot->setAxisScale(QwtPlot::xBottom, -180, 360-180);
    d_plot->setAxisScale(QwtPlot::yLeft, -90, 90);

    // Включить возможность приближения/удаления графика
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(d_plot->canvas());
         // клавиша, активирующая приближение/удаление
    magnifier->setMouseButton(Qt::MidButton);

         // Включить возможность перемещения по графику
    QwtPlotPanner *d_panner = new QwtPlotPanner( d_plot->canvas() );
         // клавиша, активирующая перемещение
    d_panner->setMouseButton( Qt::LeftButton );

    //main_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // сглаживание кривой
    //main_curve->setPen(QPen(Qt::gray,0.4,Qt::SolidLine));
//    satelliteImage->attach(d_plot);
//    satelliteImage->loadFile(QRectF(), "./satellite.svg");
}

void C_2d::object_icon_display(QVector<double> geo)
{
    if (geo[0]>M_PI)
        satelliteImage->loadFile(QRectF(geo[0]/M_PI*180.-360-size_object/2,geo[1]/M_PI*180.-size_object/2, size_object, size_object), "./rockets_launcher.svg");
    else
        satelliteImage->loadFile(QRectF(geo[0]/M_PI*180.-size_object/2,geo[1]/M_PI*180.-size_object/2, size_object, size_object), "./rockets_launcher.svg");
    satelliteImage->attach(d_plot);
}

void C_2d::SC_icon_display(QVector<double> geo,int i,QString type)
{
    if (type == OEN)
    {
        if (geo[0]>180) icons[i]->loadFile(QRectF(geo[0]-360-size_satellite/2,geo[1]-size_satellite/2, size_satellite, size_satellite), "./satelliteOEN.svg");
        else icons[i]->loadFile(QRectF(geo[0]-size_satellite/2,geo[1]-size_satellite/2, size_satellite, size_satellite), "./satelliteOEN.svg");
    }
    else
    {
        if (geo[0]>180) icons[i]->loadFile(QRectF(geo[0]-360-size_satellite/2,geo[1]-size_satellite/2, size_satellite, size_satellite), "./satelliteRLN.svg");
        else icons[i]->loadFile(QRectF(geo[0]-size_satellite/2,geo[1]-size_satellite/2, size_satellite, size_satellite), "./satelliteRLN.svg");
    }
    }

void C_2d::attach_icon()
{
    QwtPlotSvgItem *icon = new QwtPlotSvgItem();
    icon->attach(d_plot);
    icons.push_back(icon);
}

C_2d::~C_2d()
{
    delete ui;
}
