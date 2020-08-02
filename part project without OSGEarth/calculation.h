#ifndef C_CALCULATION_H
#define C_CALCULATION_H

#include <QVector>
#include "convert.h"
#include "spacecraft.h"
#include "manager.h"
#include "math.h"


class C_calculation
{
public:
    C_calculation();
    static QVector <QVector <double> > probability_distribution_function (QVector<int> intervals_assessment);
    static QVector <double> runge (QVector <double> dn, double dt);
    static double tau_to_tetta (double tau, double a, double e);
    static S_VekUkhod VekUkhod(S_KEO keo0);
    static C_SpaceCraft move (C_SpaceCraft sc, int dt);
    static double line_int (double x1, double x2, double y1, double y2, double x);
    static bool check_OE_zone_overview (QVector <double> coordinate_object/*долгота,широта*/, QVector <double> coordinate_SC/*в ГСК*/,  double angle, QDateTime time, bool consider_sun);
    static bool check_sun (QVector <double> coordinate_object/*долгота,широта*/, QDateTime time);
    static bool check_RL_zone_overview (QVector<double> coordinate_object, QVector<double> gsc0, QVector<double> gsc1, QVector<double> geo0, QVector<double> geo1, QVector<double> geo2, double angle1, double angle2);
    static QVector <double> display_RL_zone (QVector <double> gsc, QVector <double> geo1, QVector<double> geo2, double angle1, double angle2);
    static QVector <double> count_evaluation (QVector <double> time);
    static int count_time_viewing_second (QDateTime time_begin, QVector <QVector <double> > coordinate_IRP/*information reception point - пункт приема информации, геоцентр.*/, QVector <QVector <double> >coordinate_shooting_areas/*районы съемки*/, QVector < C_SpaceCraft > space_crafts, QVector <QVector <double> > coordinate_IBP/*item bookmarks programms - пункт закладки программ, геоцент.*/);
    static int count_time_viewnig (int step, QDateTime normally_distributed_time, QVector <S_ground_object > *coordinate_IRP/*information reception point - пункт приема информации, геоцентр.*/, QVector <S_ground_object> *coordinate_shooting_areas/*районы съемки*/, QVector < C_SpaceCraft> *space_crafts, QVector <S_ground_object> *coordinate_IBP/*item bookmarks programms - пункт закладки программ, геоцент.*/);
    static QVector <QVector <QDateTime> > count_time_viewnig_one_region (int step, QDateTime time_begin, QDateTime time_end, QVector <S_ground_object > *coordinate_IRP/*information reception point - пункт приема информации, геоцентр.*/, S_ground_object coordinate_shooting_areas/*районы съемки*/, QVector < C_SpaceCraft> *space_crafts, QVector <S_ground_object> *coordinate_IBP/*item bookmarks programms - пункт закладки программ, геоцент.*/);
    static double count_draconion_period (S_KEO oeo);
    static double count_effective_period (C_SpaceCraft sc);
    static double count_period (double a);
    static double count_next_U_viewing (double psi, double i, double u);//расчет следующего аргумента широты КА U, при котором КА будет нах-ся на геодез. широте просмотра
    static int count_difference_time_from_U (double u1, double u2, double a, double e, double om);
    static double tetta_to_E (double tetta, double e);
    static double E_to_tau (double E, double e, double a);
    static double count_fizone (double r, double gamma);//расчет зоны обзора по радиусу и углу полураствора
    static double count_longitude_viewing (double i, double u, double psi);
    static double count_KEO_to_radius (double a, double e, double E);
    static QVector <double> count_left_and_right_longitude_viewing_on_latitude (double psi, C_SpaceCraft sc);
    static QDateTime find_min_datetime (QVector <QDateTime> vector_time, QString min_max);
    static double linear_dependence(double x,double x1,double x2,double y1, double y2);
    static int modern_count_time_viewing_second (QDateTime time_begin, QVector <QVector <double> > coordinate_IRP/*information reception point - пункт приема информации, геоцентр.*/, QVector <QVector <double> >coordinate_shooting_areas/*районы съемки*/, QVector < C_SpaceCraft > *space_crafts, QVector <QVector <double> > coordinate_IBP/*item bookmarks programms - пункт закладки программ, геоцент.*/);
    static QVector <int> count_variational_series (QVector <int> interval_assessment, QDateTime time_begin, QDateTime time_end);
    static QVector <int> count_variational_series (QVector <int> interval_assessment);
    static QVector <double> count_effiency_ratio (QVector <QVector <QDateTime> > vector_time, QDateTime time_begin, QDateTime time_end);
private:
    static QVector <double> coef(double x,double y,double z,double vx,double vy,double vz,double dt);
    static double dv(double X,double x,double y,double z);
};

#endif // C_CALCULATION_H
