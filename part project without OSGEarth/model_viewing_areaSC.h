#ifndef MODEL_VIEWING_AREASC_H
#define MODEL_VIEWING_AREASC_H
//#include "object3d.h"
#include <osg/MatrixTransform>
#include "gui3d/osgviewerQt.h"



class model_viewing_areaSC
{
public:
    model_viewing_areaSC();
    model_viewing_areaSC(QString name, QVector<double> gsc_SC, QVector<double> geo_SC, double gamma, QColor color);
    model_viewing_areaSC(QString name, QVector<double> gsc_SC, QVector<double> geo_SC, double angle1, double angle2, QColor color);
    model_viewing_areaSC(QString name, QVector<double> gsc_SC, QVector<double> geo_SC, QString type);

    /*osg::ref_ptr<osg::PositionAttitudeTransform>*/
    osg::ref_ptr<osg::MatrixTransform> m_matrix_transform;
private:

};

#endif // MODEL_VIEWING_AREASC_H
