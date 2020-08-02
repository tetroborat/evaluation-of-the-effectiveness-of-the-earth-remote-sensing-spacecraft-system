#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "calculation.h"
#include "spacecraft.h"
#include <QMap>
#include <manager.h>
#include "constants.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    explicit MainWindow(QWidget *parent = 0);
    QDateTime time_current;
    ~MainWindow();

private slots:
    void on_commandLinkButton_output_clicked();
    void on_commandLinkButton_2d_clicked();
    void on_commandLinkButton_3d_clicked();
    void on_dateTimeEdit_begin_dateTimeChanged(const QDateTime &dateTime);
    void on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime);
    void on_commandLinkButtoncount_clicked();
    void on_horizontalSlider_sliderMoved(int position);
    void on_pushButtonplay_pressed();
    void on_verticalSlider_valueChanged(int value);
    void on_timeEdit_timeChanged(const QTime &time);
    void on_commandLinkButton_input_SC_clicked();
    void on_commandLinkButton_input_RV_clicked();
    void on_commandLinkButton_input_GCC_clicked();
    void on_commandLinkButtoncount_2_clicked();
    void on_lineEdit_variation_textChanged(const QString &arg1);
    void on_lineEdit_error_textChanged(const QString &arg1);
    void current_region();

private:
    QDateTime time_begin;
    QDateTime time_end;
    int mem;
    bool check_stop = true;
    bool check_move_step = false;
    bool check_view;
    double size;
    QVector <double> geo_object;//географ. коор. объекта наблюдения
    S_KEO keo;//КЭО на момент начала наблюдения
    S_KEO keo1;//КЭО на текущий момент наблюдения
    QVector<double> gsc1;//коор. в ГСК на текущий момент наблюдения
    QVector<double> geo0;//географ. коор. на предыдущий момент наблюдения
    QVector<double> geo1;//географ. коор. на текущий момент наблюдения
    S_VekUkhod ukhod;//вековые уходы для заданных КЭО
    QVector <QVector <double> > GEO;//посчитанный вектор географических координат
    QVector <QVector <double> > GEOcutting;//вектор географических кординат для отображения 2d
    QVector <QVector <double> > geo;//вектор географических координат
    QVector <QVector <double> > gsc;//вектор координат в ГСК
    QVector <QVector <QVector <double> > > geo_all_SC;//вектор географических координат
    QVector <QVector <QVector <double> > > gsc_all_SC;//вектор координат в ГСК
    QVector <QVector<QwtIntervalSample> > intervals_all_SC ;
    QVector <int> interval_assessment;
    double dlit;//длительность наблюдения
    double time_lag;//время запаздывания отображения
    double time_correction = 0;//поправка времени при отрицательном времени тайминга
    int sh = 60;//шаг при отображении
    int step = 1;//шаг при расчете движения
    int shbo_2d = 2;
    int shbo_3d = 25;
    int shbo = shbo_2d;//кратнсоть уменьшения шага при отображении
    QVector <C_SpaceCraft> *listKA;
    QVector < S_ground_object > *list_region;
    QVector < S_ground_object > *list_receptions_points;
    QVector < S_ground_object > *list_control_means;
    QVector <QString> types;
    int i;//положение начала отображения в 2d
    int j;//местоположение в 2d
    int value;//переменная ползунка
    QPolygonF points;
    C_SpaceCraft *ka;
    void cutting(int k);//вырезание GEOcutting из GEO
    QVector <QVector <double> > cutting(QVector <QVector <double> >geo);
    void count_coordinates_SC (C_SpaceCraft &SC, int i);
    QPolygonF create_points(QVector <QVector<double> > cr);
    void defining_location_vector (int k);
    void print_route_flight (int position);
    void print_route_flight (QVector <QVector <double> >geo,int position,int i);
    void display_iteration(int position);
    QwtIntervalSample add_interval_RV(C_SpaceCraft SC, QVector <QwtIntervalSample> &intervals, int i);
    void calculation_observation_gaps ();
    void change_list_region();
};

#endif // MAINWINDOW_H
