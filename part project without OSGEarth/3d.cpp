#include "3d.h"
#include "ui_3d.h"

C_3d::C_3d(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_3d)
{
    ui->setupUi(this);

}

void C_3d::addModelViewingAreaSC(QString name, QVector<double> gsc_SC, QVector<double> geo_SC, QString type, double angle1, double angle2)
{
    ui->widget->addModelViewingAreaSC_scene3d(name, gsc_SC,geo_SC,type,angle1,angle2);
}

void C_3d::addImage(QString name, QVector<double> gsc_SC, QVector<double> geo_SC, QString type)
{
    ui->widget->addImage(name, gsc_SC,geo_SC,type);
}

void C_3d::addObject(QVector<double> geo_SC,QString name,QString icon)
{
    ui->widget->addEarthObj(geo_SC[1],geo_SC[0],name,icon);
}


void C_3d::setDataTime(QDateTime time)
{
    ui->widget->setDateTime(time);
}

void C_3d::moveConus_OEN(QVector<double> gsc_SC, QVector<double> geo_SC, int i)
{
    ui->widget->repaint_OEN(i,gsc_SC,geo_SC);
}

void C_3d::moveConus_RTN(QVector<double> gsc_SC, QVector<double> geo1_SC, QVector<double> geo2_SC, int i)
{
    ui->widget->repaint_RTN(i,gsc_SC,geo1_SC,geo2_SC);
}

C_3d::~C_3d()
{
    delete ui;
}
