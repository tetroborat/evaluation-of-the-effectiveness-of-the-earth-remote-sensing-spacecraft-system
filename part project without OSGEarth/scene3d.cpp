#include "scene3d.h"

ASDScene3D::ASDScene3D(QWidget *parent) :
    ViewerWidget(parent)
{
    QDateTime date = QDateTime();
    setDateTime(date);
    createEarth(m_root_agsk);
    createSky();
}

ASDScene3D::~ASDScene3D()
{
}


void ASDScene3D::addObject3D(ASDObject3D *object)
{
    m_objects.append(object);
}

void ASDScene3D::removeObject3D(ASDObject3D *object)
{
     m_objects.removeOne(object);
}

void ASDScene3D::repaintObjects3D(QDateTime time)
{
    foreach (ASDObject3D* object, m_objects) {
       object->repaint(time, this);
    }
}
