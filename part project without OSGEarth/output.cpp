#include "output.h"
#include "ui_output.h"

C_output::C_output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_output)
{
    ui->setupUi(this);
    ui->horizontalLayout_6->addWidget(d_plot_probability_distribution_gaps);
    //d_plot_probability_distribution_gaps->setTitle( "Функция распределения вероятности разрывов в наблюдении объекта" );
    d_plot_probability_distribution_gaps->setAxisTitle(QwtPlot::yLeft, "Вероятность наблюдения");
    d_plot_probability_distribution_gaps->setAxisTitle(QwtPlot::xBottom, "Разрыв наблюдения, час");
    d_plot_probability_distribution_gaps->setCanvasBackground(Qt::white);
    curve_probability_distribution_gaps->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curve_probability_distribution_gaps->setPen(QPen(Qt::black,0.4,Qt::SolidLine));
    curve_probability_distribution_gaps->attach(d_plot_probability_distribution_gaps);


    ui->horizontalLayout_2->addWidget(d_plot_probability_distribution_efficiency);
    d_plot_probability_distribution_efficiency->setAxisTitle(QwtPlot::yLeft, "Вероятность просмотра");
    d_plot_probability_distribution_efficiency->setAxisTitle(QwtPlot::xBottom, "Оперативность наблюдения, час");
    d_plot_probability_distribution_efficiency->setCanvasBackground(Qt::white);
    curve_probability_distribution_efficiency->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curve_probability_distribution_efficiency->setPen(QPen(Qt::black,0.4,Qt::SolidLine));
    curve_probability_distribution_efficiency->attach(d_plot_probability_distribution_efficiency);

}

void C_output::input_data_histogram(QVector <QwtIntervalSample> intervals, int i)
{
    hystograms[i]->setSamples(intervals);
    hysto_plot->replot();
}

C_output::~C_output()
{
    delete ui;
}

void C_output::create_histogram(int size)
{
    //ui->horizontalLayout_4->addWidget(hysto_plot);
    hysto_plot->setAxisScale(QwtPlot::yLeft,0,size);
    hysto_plot->setCanvasBackground(Qt::white);
    //hysto_plot->setTitle("Гистограмма зон видимости");
    QVector<QwtIntervalSample> h_intervals ;
    QwtPlotHistogram *hystogram = new QwtPlotHistogram;
    hystogram->setSamples(h_intervals);
    hystogram->attach(hysto_plot);
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(hysto_plot->canvas());
    magnifier->setMouseButton(Qt::MidButton);
    QwtPlotPanner *d_panner = new QwtPlotPanner( hysto_plot->canvas() );
    d_panner->setMouseButton( Qt::LeftButton );
}

void C_output::create_1_hysto()
{
    QwtPlotHistogram *hystogram = new QwtPlotHistogram;
    hystogram->attach(hysto_plot);
    hystograms.push_back(hystogram);
}

void C_output::create_probability_distribution_gaps(QVector<QVector<double> > points)
{
    QPolygonF Points;
    for (int zz =0;zz<points.size();zz++)
        Points<<QPointF(points[zz][0],points[zz][1]);
    curve_probability_distribution_gaps->setSamples(Points);
    d_plot_probability_distribution_gaps->replot();
}

void C_output::create_probability_distribution_efficiency(QVector<int> times)
{
    QPolygonF points;
    double global_var = 0;double step = 0.5;int check_all = 0;int intermediat_var = 0;
    while (times.size()>check_all)
    {
        for (int var = 0; var < times.size(); ++var)
            if (times[var]/3600.>global_var && times[var]/3600.<=global_var+step)
            {
                intermediat_var ++;
                check_all ++;
            }
        points<<QPointF(global_var,1.*intermediat_var/times.size());
        points<<QPointF(global_var+step,1.*intermediat_var/times.size());
        intermediat_var = 0;
        global_var += step;
    }
    points<<QPointF(global_var,0);
    curve_probability_distribution_efficiency->setSamples(points);
    d_plot_probability_distribution_efficiency->replot();
    QVector <double> times1;
    for (int var = 0; var < times.size(); ++var)
        times1.append(times[var]/3600.);
    QVector <double> characteristics = C_calculation::count_evaluation(times1);
    //create_probability_distribution_efficiency_normal(characteristics[0],characteristics[1]);
    ui->label->setText("Математическое\nожидание:\nСКО:\nАсимметрия:\nЭксцесс:");
    ui->label_2->setText("\n"+QString::number(characteristics[0])+"\n"+QString::number(characteristics[1])+"\n"+QString::number(characteristics[2])+"\n"+QString::number(characteristics[3]));
}

void C_output::create_probability_distribution_efficiency_normal(double mathematical_expectation, double square_deviation)
{    
    curve_probability_distribution_efficiency_normal->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curve_probability_distribution_efficiency_normal->setPen(QPen(Qt::black,0.4,Qt::SolidLine));
    curve_probability_distribution_efficiency_normal->attach(d_plot_probability_distribution_efficiency);
    QPolygonF points;
    double x = 0;
    double koef = 1 / (square_deviation * sqrt(2*M_PI));
    while (x < 2 * mathematical_expectation)
    {
        points << QPointF(x, koef*exp( -pow( (x - mathematical_expectation) / square_deviation, 2)) / 2);
        x += 0.125;
    }
    curve_probability_distribution_efficiency_normal->setSamples(points);
    d_plot_probability_distribution_efficiency->replot();
}

