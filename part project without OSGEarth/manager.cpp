#include "manager.h"
#include <QDebug>
#include "spacecraft.h"
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"
#include "calculation.h"
#include "constants.h"


C_manager::C_manager()
{
    qDebug()<<"C_manager";
    listKA = new QVector < C_SpaceCraft >;
    list_region = new QVector < S_ground_object >;
    list_control_means = new QVector < S_ground_object >;
    list_receptions_points = new QVector < S_ground_object >;

    connect_BD();
}

void C_manager::connect_BD()
{
    //Подключаем базу данных
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./mydatabasepros.db3");
    db.open();


    //Осуществляем запрос
    QSqlQuery query;



    query.exec("SELECT name_SC,nuT,e,i,omegab,omegam,M,type,datatime,angle1,angle2 FROM SC");
    while (query.next())
    {
        C_SpaceCraft sd;
        sd.name = query.value(0).toString();
        moment = QDateTime (QDateTime::fromString(query.value(8).toString(), "yyyy-MM-dd hh:mm:ss"));

        //вводятся элементы в TLE и преобразуются в КЭО
        sd.KEO = {pow(86400/query.value(1).toDouble()*sqrt(mu)/2/M_PI,2./3.),query.value(2).toDouble(),query.value(3).toDouble()/180.*M_PI,query.value(4).toDouble()/180.*M_PI,query.value(5).toDouble()/180.*M_PI,86400/query.value(1).toDouble()/360.*query.value(6).toDouble(),moment};
        sd.type = query.value(7).toString();
        sd.angle1 = query.value(9).toDouble();
        sd.angle2 = query.value(10).toDouble();
        sd.ukhod = C_calculation::VekUkhod(sd.KEO);
        listKA->append(sd);
    }
//    C_SpaceCraft sd;
//    sd.name = "тестер";
//    moment = QDateTime (QDateTime::fromString("2019-10-23 10:00:00", "yyyy-MM-dd hh:mm:ss"));
//    sd.KEO = {7546.87,0.0549843,1.62871,4.88087,5.38117,5587,moment};
//    sd.angle1 = 83;
//    sd.ukhod = C_calculation::VekUkhod(sd.KEO);
//    sd.type = OEN;
//    listKA->append(sd);

    S_ground_object ground_object;

    query.exec("SELECT name_control_mean,longitude,latitude FROM control_means");
    while (query.next())
    {
        ground_object.name = query.value(0).toString();
        ground_object.longitude = query.value(1).toString().toDouble()/180*M_PI;
        ground_object.latitude = query.value(2).toString().toDouble()/180*M_PI;
        list_control_means->append(ground_object);
    }

    query.exec("SELECT name_receptoin_point,longitude,latitude FROM reception_points");
    while (query.next())
    {
        ground_object.name = query.value(0).toString();
        ground_object.longitude = query.value(1).toString().toDouble()/180*M_PI;
        ground_object.latitude = query.value(2).toString().toDouble()/180*M_PI;
        list_receptions_points->append(ground_object);
    }

    query.exec("SELECT name_region,longitude,latitude,type,notice FROM region");
    while (query.next())
    {
        ground_object.name = query.value(0).toString();
        ground_object.longitude = query.value(1).toString().toDouble()/180*M_PI;
        ground_object.latitude = query.value(2).toString().toDouble()/180*M_PI;
        ground_object.type = query.value(3).toString();
        ground_object.notice = query.value(4).toString();
        list_region->append(ground_object);
    }
}
