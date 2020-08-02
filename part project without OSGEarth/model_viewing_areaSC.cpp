#include "model_viewing_areaSC.h"
#include "gui3d/osgviewerQt.h"

model_viewing_areaSC::model_viewing_areaSC()
{

    qDebug()<<"C_calculation";
}

model_viewing_areaSC::model_viewing_areaSC(QString name, QVector<double> gsc_SC, QVector<double> geo_SC,  double gamma, QColor color)
{
    m_matrix_transform = ViewerWidget::addConus(name, gsc_SC,geo_SC,gamma, color);
}

model_viewing_areaSC::model_viewing_areaSC(QString name, QVector<double> gsc_SC, QVector<double> geo_SC, double angle1, double angle2, QColor color)
{
    m_matrix_transform = ViewerWidget::addTriangle(name, gsc_SC,geo_SC,angle1,angle2,color);
}

model_viewing_areaSC::model_viewing_areaSC(QString name, QVector<double> gsc_SC, QVector<double> geo_SC, QString type)
{
    m_matrix_transform = ViewerWidget::addImage(name, gsc_SC, geo_SC, type);
}


