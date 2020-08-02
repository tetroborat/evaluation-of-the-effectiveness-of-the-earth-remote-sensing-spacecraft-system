#ifndef SCENE3D_H
#define SCENE3D_H
#include "object3d.h"
#include "gui3d/osgviewerQt.h"

#include <QWidget>

class ASDObject3D;

class ASDScene3D : public ViewerWidget
{

public:
    ASDScene3D(QWidget *parent = 0);
    ~ASDScene3D();
    void addObject3D(ASDObject3D* object);
    void removeObject3D(ASDObject3D* object);
    void repaintObjects3D(QDateTime time);

private:
     QList<ASDObject3D*> m_objects;

};

#endif // SCENE3D_H
