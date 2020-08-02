#ifndef ASDOBJECT3D_H
#define ASDOBJECT3D_H
#include <QDateTime>
//#include <common/asdtype.h>
#include "scene3d.h"
#include "osg/Node"

class ASDScene3D;

class ASDObject3D
{
public:
    virtual void repaint(QDateTime time, ASDScene3D* scene)=0;
    protected:
    bool m_create_object; //создан ли объект
    osg::ref_ptr<osg::Node> m_adress; //адрес объекта

};

//class ASDPositionObj
//{
//protected:
//    QVector<ASDPosObj> m_coord;
//    QMap<QDateTime,ASDPosObj> m_coord_map;
//    QDateTime m_time_begin;
//    QDateTime m_time_end;

//public:
//    void setPosition(QVector<ASDPosObj> coord)
//    {
//        m_coord = coord;
//        foreach (ASDPosObj pos, m_coord) {
//            m_coord_map.insertMulti(pos.time_real,pos);
//        }
//        m_time_begin = m_coord_map.firstKey();
//        m_time_end = m_coord_map.lastKey();
//    }

//    /*!
//     * \brief возращает положение объекта на заданное время
//     * \param time - время
//     * \param pos - положение
//     * \return true - объект существует в заданный временной интервал, false - объекта не существует
//     */
//    bool getPosition(QDateTime time, ASDPosObj & pos)
//    {
//        QMap<QDateTime,ASDPosObj>::Iterator iter;

//        if((time>m_time_begin)&&(time<m_time_end))
//            for(iter = m_coord_map.begin(); iter != m_coord_map.end();iter++)
//            {
//                if(time<iter.key())
//                {
//                    pos = iter.value();
//                    return true;
//                }
//            }
//    }


//};

#endif // ASDOBJECT3D_H
