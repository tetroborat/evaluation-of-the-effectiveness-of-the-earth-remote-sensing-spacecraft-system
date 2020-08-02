#ifndef C_MANAGER_H
#define C_MANAGER_H
#include <QVector>
#include <spacecraft.h>

struct S_ground_object
{
    double longitude;
    double latitude;
    QString type;
    QString notice;
    QString name;
};

class C_manager
{
public:
    void connect_BD();
    C_manager();
    QVector < C_SpaceCraft > *listKA;
    QVector < S_ground_object > *list_region;
    QVector < S_ground_object > *list_control_means;
    QVector < S_ground_object > *list_receptions_points;
private:
    QDateTime moment;

};



#endif // C_MANAGER_H
