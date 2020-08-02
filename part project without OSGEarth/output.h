#ifndef C_OUTPUT_H
#define C_OUTPUT_H

#include <QWidget>
#include <qwt_plot.h>
#include "spacecraft.h"
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include "qwt_plot_histogram.h"
#include "qmath.h"
#include "calculation.h"


namespace Ui {
class C_output;
}

class C_output : public QWidget
{
    Q_OBJECT

public:
    Ui::C_output *ui;
    QwtPlot *d_plot_probability_distribution_gaps = new QwtPlot();
    QwtPlot *d_plot_probability_distribution_efficiency = new QwtPlot();
    QwtPlotCurve* curve_probability_distribution_gaps = new QwtPlotCurve;
    QwtPlotCurve* curve_probability_distribution_efficiency = new QwtPlotCurve;
    QwtPlotCurve* curve_probability_distribution_efficiency_normal = new QwtPlotCurve;
    explicit C_output(QWidget *parent = 0);
    QVector <QwtPlotHistogram *> hystograms;
    QwtPlot *hysto_plot = new QwtPlot();
    void input_data_histogram(QVector <QwtIntervalSample> intervals,int i);
    void create_histogram(int size);
    void create_1_hysto();
    void create_probability_distribution_gaps(QVector<QVector< double> > points);
    void create_probability_distribution_efficiency(QVector<int> times);
    void create_probability_distribution_efficiency_normal(double mathematical_expectation, double square_deviation);
    ~C_output();

private:
    C_SpaceCraft SC;
};

#endif // C_OUTPUT_H
