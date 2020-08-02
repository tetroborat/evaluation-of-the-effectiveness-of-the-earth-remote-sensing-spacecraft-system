#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "input.h"
#include "ui_input.h"
#include "listsc.h"
#include "ui_listsc.h"
#include "listregion.h"
#include "ui_listregion.h"
#include "calculation.h"
#include "output.h"
#include "ui_output.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->setMinimum(ui->dateTimeEdit_begin->dateTime().toTime_t());
    ui->horizontalSlider->setMaximum(ui->dateTimeEdit_end->dateTime().toTime_t());
    ui->groupBox_3->setVisible(0);
    ui->groupBox_2->setEnabled(0);
    ui->stackedWidget->setCurrentWidget(ui->page_input);

    listKA = ui->page_input->ui->page_SC->listKA_usage;
    list_control_means = ui->page_input->ui->page_ground_complex->list_control_means_usage;
    list_receptions_points = ui->page_input->ui->page_ground_complex->list_receptions_points_usage;
    list_region = ui->page_input->ui->page_region->list_region_usage;

    ui->page_output->create_histogram(listKA->size());
    for (int i=0;i<listKA->size();i++)
    {
        ui->page_2d->attach_paint_main_cur();
        ui->page_output->create_1_hysto();
        ui->page_2d->attach_icon();
    }

    connect(ui->page_input->ui->page_region->ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(current_region()));

    //Место фантазий

//    S_KEO kepler;
//    QDateTime moment = QDateTime (QDateTime::fromString("2015-09-20 00:03:25", "yyyy-MM-dd hh:mm:ss"));
////////    kepler = {6819.930884,0.023556,0.899224,4.931406,0.799673,1.351751,moment};
//////    kepler = {6498,0.002,60*M_PI/180,90*M_PI/180,0.1*M_PI/180,0,moment};
//////    qDebug()<<C_convert::KepToAgsc(kepler);
//////    double E = 2*atan(sqrt((1-kepler.e)/(1+kepler.e))*tan(kepler.tau/2));
//////    if (E<0)
//////        E += 2*M_PI;
//////    double T = 2*M_PI*sqrt(pow(kepler.a,3)/398600.44);
//////    kepler.tau = T/2/M_PI*(E-kepler.e*sin(E));
////    QVector <double> geo;
////    geo.push_back(60/180*M_PI);
////    geo.push_back(60/180*M_PI);
//////    qDebug()<<kepler.a<<kepler.e<<kepler.i<<kepler.om<<kepler.Om<<kepler.tau<<kepler.datetime;
//////    qDebug()<<C_convert::KepToAgsc(kepler);
//////    qDebug()<<C_convert::AgscToGsc(C_convert::KepToAgsc(kepler),moment);
//////    qDebug()<<C_convert::GscToIsc(C_convert::AgscToGsc(C_convert::KepToAgsc(kepler),moment),geo);
//////    qDebug()<<C_convert::GscToAgsc(C_convert::AgscToGsc(C_convert::KepToAgsc(kepler),moment),moment);
//////    kepler = C_convert::AgscToKep(C_convert::KepToAgsc(kepler),moment);
//////    qDebug()<<kepler.a<<kepler.e<<kepler.i<<kepler.om<<kepler.Om<<kepler.tau<<kepler.datetime;
////    for (int var = 0; var < 86400; ++var) {
////        if (C_calculation::check_sun(geo,moment) && !C_calculation::check_sun(geo,moment.addSecs(-1)))
////            qDebug()<<C_calculation::check_sun(geo,moment)<<moment;
////        if (!C_calculation::check_sun(geo,moment) && C_calculation::check_sun(geo,moment.addSecs(-1)))
////            qDebug()<<C_calculation::check_sun(geo,moment)<<moment;
////        moment = moment.addSecs(1);
////    }
//    S_ground_object sirya;
//    sirya.longitude = 36/180*M_PI;
//    sirya.latitude = 31.5/180*M_PI;
//    qDebug() << C_calculation::count_effiency_ratio(C_calculation::count_time_viewnig_one_region(10,moment,moment.addDays(15),list_receptions_points,sirya,listKA,list_control_means),moment,moment.addDays(15));

//    //
}

MainWindow::~MainWindow()
{
    check_stop = true;
    delete ui;
}

void MainWindow::on_commandLinkButton_output_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_output);
}

void MainWindow::on_commandLinkButton_2d_clicked()
{
    shbo = shbo_2d;
    mem = 0;
    display_iteration(ui->horizontalSlider->value());
    ui->stackedWidget->setCurrentWidget(ui->page_2d);
}

void MainWindow::on_commandLinkButton_3d_clicked()
{
    shbo = shbo_3d;
    display_iteration(ui->horizontalSlider->value());
    ui->stackedWidget->setCurrentWidget(ui->page_3d);
}

void MainWindow::on_dateTimeEdit_begin_dateTimeChanged(const QDateTime &dateTime)
{
    ui->horizontalSlider->setMinimum(dateTime.toTime_t());
    ui->dateTimeEdit_current->setDateTime(dateTime);
    ui->horizontalSlider->valueChanged(dateTime.toTime_t());
    ui->groupBox_2->setEnabled(0);
}

void MainWindow::on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime)
{
    if (dateTime>ui->dateTimeEdit_begin->dateTime())
    {
        ui->horizontalSlider->setMaximum(dateTime.toTime_t());
        ui->dateTimeEdit_current->setDateTime(ui->dateTimeEdit_begin->dateTime());
        ui->horizontalSlider->valueChanged(ui->horizontalSlider->minimum());
        ui->groupBox_2->setEnabled(0);
    }
}

void MainWindow::on_commandLinkButtoncount_clicked()
{
    ui->groupBox_2->setVisible(0);
    ui->groupBox->setEnabled(false);
    ui->stackedWidget->setEnabled(false);
    geo_object.clear();
    gsc_all_SC.clear();
    geo_all_SC.clear();
    intervals_all_SC.clear();
    ui->stackedWidget->setCurrentWidget(ui->page_input);
    ui->commandLinkButton_2d->setEnabled(false);
    ui->commandLinkButton_3d->setEnabled(false);
    ui->commandLinkButton_output->setEnabled(false);
    ui->groupBox_3->setVisible(1);
    ui->groupBox_2->setVisible(0);
    ui->progressBar->setValue(0);
    time_begin = ui->dateTimeEdit_begin->dateTime();
    time_end = ui->dateTimeEdit_end->dateTime();
    dlit = time_begin.secsTo(time_end);
    geo_object.append(ui->page_input->ui->page_region->list_region_usage->at(0).longitude);
    geo_object.append(ui->page_input->ui->page_region->list_region_usage->at(0).latitude);
    ui->page_2d->object_icon_display(geo_object);
    ui->page_3d->addImage(QString("Самолет"),C_convert::GeoToGsc(geo_object),geo_object,QString("aircraft"));

    for(int numberSC = 0;numberSC<listKA->size();numberSC++)
    {
        ka = &listKA->operator [](numberSC);
        //ui->page_3d->detachConus(listKA->operator [](op));//как это делается?
        count_coordinates_SC(listKA->operator [](numberSC),numberSC);
        ui->page_3d->addModelViewingAreaSC(ka->name,gsc_all_SC[gsc_all_SC.size()-1][0],geo_all_SC[geo_all_SC.size()-1][0],types[numberSC],ka->angle1,ka->angle2);
        ui->page_output->input_data_histogram(intervals_all_SC[numberSC],numberSC);
    }

    ///////////////расчет разрывов времени наблюдения///////////////////

    calculation_observation_gaps();

    ///////////////////////////////////////////////////////////////////


    size = geo_all_SC[0].size();
    ui->progressBar->setValue(100);
    ui->groupBox_3->setVisible(0);
    ui->groupBox_2->setVisible(1);
    ui->groupBox_2->setEnabled(1);

    ui->page_3d->setDataTime(time_begin);

    ui->commandLinkButton_2d->setEnabled(true);
    ui->commandLinkButton_3d->setEnabled(true);
    ui->commandLinkButton_output->setEnabled(true);
    ui->groupBox->setEnabled(true);
    ui->stackedWidget->setEnabled(true);
    ui->groupBox_2->setVisible(1);
}

void MainWindow::cutting(int k)
{
    GEOcutting.clear();
    j = i;
    while (i!=0 && !(GEO[i][0]>180 && GEO[i-1][0]<180))
    {
        i-=1;
    }
    while (j!=GEO.size()-1 && !(GEO[j][0]<180 && GEO[j+1][0]>180))
    {
        j+=1;
    }
    for (i;i<j+1;i++)
        GEOcutting.push_back(GEO[i]);
}

QVector<QVector<double> > MainWindow::cutting(QVector <QVector <double> > geo)
{
    if (geo[0][0]<geo[1][0])
    {
        GEOcutting.clear();
        j = i;
        while (i!=0 && !(geo[i][0]>180 && geo[i-1][0]<180))
        {
            i-=1;
        }
        while (j!=geo.size()-1 && !(geo[j][0]<180 && geo[j+1][0]>180))
        {
            j+=1;
        }
        for (i;i<j+1;i++)
            GEOcutting.push_back(geo[i]);
        return GEOcutting;
    }
    else
    {
        GEOcutting.clear();
        j = i;
        while (i!=0 && !(geo[i][0]<180 && geo[i-1][0]>180))
        {
            i-=1;
        }
        while (j!=geo.size()-1 && !(geo[j][0]>180 && geo[j+1][0]<180))
        {
            j+=1;
        }
        for (i;i<j+1;i++)
            GEOcutting.push_back(geo[i]);
        return GEOcutting;
    }
}

QwtIntervalSample MainWindow::add_interval_RV(C_SpaceCraft SC, QVector <QwtIntervalSample> &intervals, int i)
{
    if (SC.type == OEN)
    {
        if (check_view)
        {
            if (!C_calculation::check_OE_zone_overview(geo_object,gsc1,SC.angle1,time_current,true))
            {
                intervals.append(QwtIntervalSample(i+1,mem-time_begin.toTime_t(),time_current.toTime_t()-time_begin.toTime_t()));
                check_view = false;
            }
        }
        else
        {
            if (C_calculation::check_OE_zone_overview(geo_object,gsc1,SC.angle1,time_current,true))
            {
                interval_assessment.push_back(time_current.toTime_t());
                qDebug()<<i<<"OEN"<<time_current;
                mem = time_current.toTime_t();
                check_view = true;
            }
        }
    }
    else if(gsc.size()>2)
    {
        if (check_view)
        {
            if (!C_calculation::check_RL_zone_overview(geo_object,gsc[gsc.size()-3],gsc[gsc.size()-2],geo[geo.size()-3],geo[geo.size()-2],geo[geo.size()-1],SC.angle1,SC.angle2))
            {
                intervals.append(QwtIntervalSample(i+1,mem-time_begin.toTime_t(),time_current.toTime_t()-time_begin.toTime_t()-2*step));
                check_view = false;
            }
        }
        else
        {
            if (C_calculation::check_RL_zone_overview(geo_object,gsc[gsc.size()-3],gsc[gsc.size()-2],geo[geo.size()-3],geo[geo.size()-2],geo[geo.size()-1],SC.angle1,SC.angle2))
            {
                interval_assessment.push_back(time_current.toTime_t());
                qDebug()<<i<<"RLN"<<time_current;
                mem = time_current.toTime_t();
                check_view = true;
            }
        }
    }
}

void MainWindow::calculation_observation_gaps()
{
    ui->page_output->create_probability_distribution_gaps(C_calculation::probability_distribution_function(C_calculation::count_variational_series(interval_assessment,time_begin,time_end)));
}

void MainWindow::change_list_region()
{
    if (list_region->size() == 1) ui->stackedWidget_button->setCurrentWidget(ui->page_efficiency_observation);
    else ui->stackedWidget_button->setCurrentWidget(ui->page_probability_observing);
}

void MainWindow::count_coordinates_SC(C_SpaceCraft &SC, int i)
{
    C_SpaceCraft sc = SC;
    QVector <QwtIntervalSample> intervals ;
    check_view = false;
    sc = C_calculation::move(sc,time_begin.toTime_t()-sc.KEO.datetime.toTime_t());
    time_current = time_begin;
    types.push_back(sc.type);
    ui->page_2d->curves[i]->setTitle(sc.name);
    if (sc.type == OEN)
        ui->page_2d->curves[i]->setPen(QPen(Qt::red,0.2,Qt::SolidLine));
    else
    {
        if (sc.type == RLN)
            ui->page_2d->curves[i]->setPen(QPen(Qt::blue,0.2,Qt::SolidLine));
        else
            ui->page_2d->curves[i]->setPen(QPen(Qt::gray,0.2,Qt::SolidLine));
    }
    while(time_current<time_begin.addDays(1) && time_current<time_end)
    {
        qApp->processEvents();
        sc = C_calculation::move(sc,step);
        gsc1 = C_convert::AgscToGsc(C_convert::KepToAgsc(sc.KEO),time_current);
        geo1 = C_convert::GscToGeo(gsc1);
        geo.push_back(geo1);
        gsc.push_back(gsc1);
        time_current = time_current.addSecs(step);
        add_interval_RV(sc, intervals,i);
        ui->progressBar->setValue(int((time_begin.secsTo(time_current)/dlit+i)/listKA->size()*100));
    }
    while(time_current<time_end)
    {
        qApp->processEvents();
        sc = C_calculation::move(sc,step);
        gsc1 = C_convert::AgscToGsc(C_convert::KepToAgsc(sc.KEO),time_current);
        geo1 = C_convert::GscToGeo(gsc1);
        time_current = time_current.addSecs(step);
        add_interval_RV(sc, intervals,i);
        ui->progressBar->setValue(int((time_begin.secsTo(time_current)/dlit+i)/listKA->size()*100));
    }

    if (check_view)
    {
        intervals.append(QwtIntervalSample(i+1,(mem-time_begin.toTime_t())/3600,(time_current.toTime_t()-time_begin.toTime_t()-step)/3600));
    }
    gsc_all_SC.push_back(gsc);
    geo_all_SC.push_back(geo);
    intervals_all_SC.push_back(intervals);
    gsc.clear();
    geo.clear();
    mem = 0;
}


QPolygonF MainWindow::create_points(QVector<QVector<double> > cr)
{
    QPolygonF points;
    points.clear();
    for (int zz =0;zz<cr.size();zz++)
    {
        if (cr[zz][0]>180) points<<QPointF(cr[zz][0]-360,cr[zz][1]);
        else points<<QPointF(cr[zz][0],cr[zz][1]);
    }
    return points;
}


//void MainWindow::create_points(QVector<QVector<double> > cr)
//{
//    points.clear();
//    for (int zz =0;zz<cr.size();zz++)
//    {
//        if (cr[zz][0]>180) points<<QPointF(cr[zz][0]-360,cr[zz][1]);
//        else points<<QPointF(cr[zz][0],cr[zz][1]);
//    }
//}

void MainWindow::defining_location_vector(int k)
{
    i = int((k-time_begin.toTime_t())/step);
    if (i+1>size) i=size-1;
    if (i<0) i=0;
}

void MainWindow::print_route_flight(int position)
{
    defining_location_vector(position);
    cutting(position);
    create_points(GEOcutting);
}

void MainWindow::print_route_flight(QVector <QVector <double> > geo, int position, int number_SC)
{
    ui->page_2d->paint_main_cur(create_points(cutting(geo)),number_SC);
    defining_location_vector(position);
}

void MainWindow::display_iteration(int position)
{
    defining_location_vector(position);
    if (shbo==shbo_2d)
    {
        for(int number_SC = 0;number_SC<listKA->size();number_SC++)
        {
            ui->page_2d->SC_icon_display(geo_all_SC[number_SC][i],number_SC,types[number_SC]);
            if ((mem==0) || (i==0) || (geo_all_SC[number_SC][mem][0]<180 && geo_all_SC[number_SC][i][0]>180) || (geo_all_SC[number_SC][mem][0]>180 && geo_all_SC[number_SC][i][0]<180))
            {
                print_route_flight(geo_all_SC[number_SC],position,number_SC);
            }
        }
        mem = i;
        ui->dateTimeEdit_current->setDateTime(QDateTime::fromTime_t(position));
    }
    else
    {
        for(int number_SC = 0;number_SC<listKA->size();number_SC++)
        {
            if (types[number_SC] == "OEN")
                ui->page_3d->moveConus_OEN(gsc_all_SC[number_SC][i],geo_all_SC[number_SC][i],number_SC);
            else
            {
                if (gsc_all_SC[0].size()-2 > i)
                    ui->page_3d->moveConus_RTN(gsc_all_SC[number_SC][i],geo_all_SC[number_SC][i],geo_all_SC[number_SC][i+1],number_SC);
                else
                    ui->page_3d->moveConus_RTN(gsc_all_SC[number_SC][i],geo_all_SC[number_SC][i-1],geo_all_SC[number_SC][i],number_SC);

            }
        }
        mem = i;
        ui->dateTimeEdit_current->setDateTime(QDateTime::fromTime_t(position));
        ui->page_3d->setDataTime(ui->dateTimeEdit_current->dateTime());
    }

    //

    //qDebug()<<C_calculation::check_RL_zone_overview()

    //
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    display_iteration(position);
}
#include <windows.h>

void MainWindow::on_pushButtonplay_pressed()
{
    if (check_stop)
    {
        check_stop = false;
        ui->pushButtonplay->setText("Стоп");
        while (ui->horizontalSlider->value()!=ui->horizontalSlider->maximum() && !check_stop)
        {
            time_current = QDateTime().currentDateTime();
            value = ui->horizontalSlider->value();
            qApp->processEvents();

            display_iteration(value);

            qApp->processEvents();
            ui->page_3d->setDataTime(ui->dateTimeEdit_current->dateTime());
            ui->dateTimeEdit_current->setDateTime(ui->dateTimeEdit_current->dateTime().addMSecs(sh*1000./shbo+time_correction));
            ui->horizontalSlider->setValue(ui->dateTimeEdit_current->dateTime().toTime_t());
            qApp->processEvents();
            time_correction = 0;
            time_lag = (QDateTime().currentDateTime().toMSecsSinceEpoch()-time_current.toMSecsSinceEpoch());
            if (1000/shbo-time_lag>0) Sleep(1000/shbo-time_lag);
            else time_correction = -(1000/shbo-time_lag);
        }
        ui->pushButtonplay->setText("Пуск");
        check_stop = true;
    }
    else
    {
        check_stop = true;
        ui->pushButtonplay->setText("Пуск");
    }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    if (!check_move_step)
    {
        check_move_step = true;
        if (value <= 1800)
            value /= 30;
        else
            value = int((value-1740)*2);
        sh = value;
        QDateTime time = QDateTime (QDateTime::fromString("00:00:00", "hh:mm:ss"));;
        time = time.addSecs(value);
        ui->timeEdit->setDateTime(time);
        check_move_step = false;
    }
}

void MainWindow::on_timeEdit_timeChanged(const QTime &time)
{
    if (!check_move_step)
    {
        check_move_step = true;
        int value = time.hour()*3600+time.minute()*60+time.second();
        if (value <= 60)
            value *= 30;
        else
            value = 1800 + int((value - 60)/2);
        ui->verticalSlider->setValue(value);
        check_move_step = false;
    }
}

void MainWindow::on_commandLinkButton_input_SC_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_input);
    ui->page_input->ui->stackedWidget->setCurrentWidget(ui->page_input->ui->page_SC);
}

void MainWindow::on_commandLinkButton_input_RV_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_input);
    ui->page_input->ui->stackedWidget->setCurrentWidget(ui->page_input->ui->page_region);
}

void MainWindow::on_commandLinkButton_input_GCC_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_input);
    ui->page_input->ui->stackedWidget->setCurrentWidget(ui->page_input->ui->page_ground_complex);
}

void MainWindow::on_commandLinkButtoncount_2_clicked()
{
    ui->groupBox->setEnabled(false);
    ui->stackedWidget->setEnabled(false);
    ui->stackedWidget->setCurrentWidget(ui->page_input);
    ui->commandLinkButton_2d->setEnabled(false);
    ui->commandLinkButton_3d->setEnabled(false);
    ui->commandLinkButton_output->setEnabled(false);
    ui->groupBox_3->setVisible(1);
    ui->groupBox_2->setVisible(0);
    ui->groupBox_4->setEnabled(false);
    ui->progressBar->setValue(0);
    int n = int(pow(1.96*ui->lineEdit_variation->text().toDouble()/ui->lineEdit_error->text().toDouble(),2));
    QVector <int> vector_time;
//    QDateTime current_time = QDateTime().currentDateTime();
    QDateTime normally_distributed_time;

    for (int var = 0; var < n; ++var)
    {
        qApp->processEvents();
        //normally_distributed_time = current_time.addDays(rand()%30+1);
        //normally_distributed_time = normally_distributed_time.addSecs(rand()+rand()+rand()%20866+3);
        normally_distributed_time = QDateTime().currentDateTime().addDays(rand()%30+1).addSecs(rand()+rand()+rand()%20866+3);
        vector_time.push_back(C_calculation::count_time_viewnig(ui->timeEdit->time().msecsSinceStartOfDay()/1000,normally_distributed_time,list_receptions_points,list_region,listKA,list_control_means));//в часах
        ui->progressBar->setValue(int(var*100/n));
    }
    ui->page_output->create_probability_distribution_efficiency(vector_time);
    ui->page_output->create_probability_distribution_gaps(C_calculation::probability_distribution_function(C_calculation::count_variational_series(vector_time)));
    ui->page_output->ui->tabWidget->setTabText(1,"Интегральная функция распределения вероятностей оперативности наблюдения");
    ui->page_output->d_plot_probability_distribution_gaps->setAxisTitle(QwtPlot::xBottom, "Оперативность наблюдения, час");
    ui->progressBar->setValue(100);
    ui->groupBox_3->setVisible(0);
    ui->groupBox_2->setVisible(1);
    ui->groupBox_2->setEnabled(true);
    ui->groupBox_4->setEnabled(true);

    ui->page_3d->setDataTime(time_begin);

    ui->commandLinkButton_output->setEnabled(true);
    ui->groupBox->setEnabled(true);
    ui->stackedWidget->setEnabled(true);
}

void MainWindow::on_lineEdit_variation_textChanged(const QString &arg1)
{
    if (arg1.toInt()>0 && arg1.toInt()<100)
        ui->label_8->setText(QString::number(int(pow(1.96*arg1.toDouble()/ui->lineEdit_error->text().toDouble(),2))));
    else {if (arg1.toInt()>100) ui->lineEdit_variation->setText("99");else ui->lineEdit_variation->setText("1");}
}

void MainWindow::on_lineEdit_error_textChanged(const QString &arg1)
{
    if (arg1.toInt()>0 && arg1.toInt()<100)
        ui->label_8->setText(QString::number(int(pow(1.96*ui->lineEdit_variation->text().toDouble()/arg1.toDouble(),2))));
    else {if (arg1.toInt()>100) ui->lineEdit_error->setText("99");else ui->lineEdit_error->setText("1");}
}

void MainWindow::current_region()
{
    if (ui->page_input->ui->page_region->list_region_usage->size() < 2)
        ui->stackedWidget_button->setCurrentWidget(ui->page_probability_observing);
    else
        ui->stackedWidget_button->setCurrentWidget(ui->page_efficiency_observation);

    if (ui->page_input->ui->page_region->list_region_usage->size() == 0)
        ui->stackedWidget_button->setEnabled(false);
    else
        ui->stackedWidget_button->setEnabled(true);
}
