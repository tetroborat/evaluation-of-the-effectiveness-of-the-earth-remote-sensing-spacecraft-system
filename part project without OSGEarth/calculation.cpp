#include "calculation.h"
#include <QDebug>
#include "constants.h"
#include "convert.h"
C_calculation::C_calculation()
{
}

QVector<QVector<double> > C_calculation::probability_distribution_function(QVector<int> intervals_assessment)
{
    double T = 0;
    QVector <double> intervals_time;
    for (int var = 0; var < intervals_assessment.size(); ++var) {
        T += intervals_assessment[var];
        intervals_time.push_back(T);
    }
    QVector<QVector<double> > result (2*intervals_assessment.size()+2,QVector<double> ());
    result[0].push_back(0);
    result[0].push_back(0);
    for (int var = 0; var < intervals_time.size(); var+=1)
    {
        result[2*var+1].push_back(intervals_assessment[var]/3600.);
        result[2*var+1].push_back(result[2*var][1]);
        result[2*var+2].push_back(intervals_assessment[var]/3600.);
        result[2*var+2].push_back(intervals_time[var]/T);
    }
    result[result.size()-1].push_back(result[result.size()-2][0]+1);
    result[result.size()-1].push_back(result[result.size()-2][1]);
    return result;
}

QVector<double> C_calculation::runge(QVector <double> dn,double dt)//x,y,z,vx,vy,vz
{
    double x = dn[0];
    double y = dn[1];
    double z = dn[2];
    double vx = dn[3];
    double vy = dn[4];
    double vz = dn[5];
    QVector <double> cf1;
    QVector <double> cf2;
    QVector <double> cf3;
    QVector <double> cf4;
    QVector <double> cf;
    cf1=coef(x,y,z,vx,vy,vz,dt);
    cf2=coef(x+cf1[0]/2,y+cf1[1]/2,z+cf1[2]/2,vx+cf1[3]/2,vy+cf1[4]/2,vz+cf1[5]/2,dt);
    cf3=coef(x+cf2[0]/2,y+cf2[1]/2,z+cf2[2]/2,vx+cf2[3]/2,vy+cf2[4]/2,vz+cf2[5]/2,dt);
    cf4=coef(x+cf3[0],y+cf3[1],z+cf3[2],vx+cf3[3],vy+cf3[4],vz+cf3[5],dt);
    for(int i = 0;i<6;i++)
    {
        cf<<(cf1[i]+2*cf2[i]+2*cf3[i]+cf4[i])/6+dn[i];
    }
    return cf;
}

QVector <double> C_calculation::coef(double x, double y, double z, double vx, double vy, double vz,double dt)
{
    QVector <double> X;
    X<<vx*dt<<vy*dt<<vz*dt<<dv(x,x,y,z)*dt<<dv(y,x,y,z)*dt<<dv(z,x,y,z)*dt;
    return X;
}

double C_calculation::dv(double X, double x, double y, double z)
{
    return -mu*X/pow(x*x+y*y+z*z,1.5);
}

double C_calculation::linear_dependence(double x, double x1, double x2, double y1, double y2)
{
    return (x-x1)/(x2-x1)*(y2-y1)+y1;
}

S_VekUkhod C_calculation::VekUkhod(S_KEO keo0)
{
    S_VekUkhod ukhod;
    ukhod.dOm = 3*pi2*M_PI/mu*cos(keo0.i)/pow(keo0.a*(1-pow(keo0.e,2)),2);
    ukhod.dom = ukhod.dOm/cos(keo0.i)/2*(1-5*pow(cos(keo0.i),2));
    ukhod.dtau = ukhod.dOm/cos(keo0.i)*pow(pow(keo0.a,3)/mu,0.5)*(1+3*keo0.e)*(1-3*pow(sin(keo0.i)*sin(keo0.om),2));
    return ukhod;
}

C_SpaceCraft C_calculation::move(C_SpaceCraft sc, int dt)
{
    double T = count_period(sc.KEO.a);
    sc.KEO.tau = fmod(sc.KEO.tau + dt/T*sc.ukhod.dtau+dt,T);
    sc.KEO.Om = fmod(sc.KEO.Om + dt/T*sc.ukhod.dOm,2*M_PI);
    if(sc.KEO.Om<0) sc.KEO.Om+=2*M_PI;
    sc.KEO.om = fmod(sc.KEO.om + dt/T*sc.ukhod.dom,2*M_PI);
    if(sc.KEO.om<0) sc.KEO.om+=2*M_PI;
    sc.KEO.datetime = sc.KEO.datetime.addSecs(dt);
    sc.ukhod.dtau = sc.ukhod.dOm/cos(sc.KEO.i)*pow(pow(sc.KEO.a,3)/mu,0.5)*(1+3*sc.KEO.e)*(1-3*pow(sin(sc.KEO.i)*sin(sc.KEO.om),2));

    return sc;
}

double C_calculation::line_int(double x1, double x2, double y1, double y2, double x)
{
    return (y2-y1)*(x-x1)/(x2-x1)+y1;
}

bool C_calculation::check_OE_zone_overview(QVector <double> coordinate_object, QVector <double> coordinate_SC,  double angle, QDateTime time, bool consider_sun)
{
    angle /= 180/M_PI;
    QVector <double> DAzYm = C_convert::IscToDAzYm(C_convert::GscToIsc(coordinate_SC,coordinate_object));
    if (consider_sun)
    {
        if (M_PI/2-DAzYm[2] < angle && check_sun(coordinate_object,time)) return true;
        else return false;
    }
    else
        if (M_PI/2-DAzYm[2] < angle) return true;
        else return false;
}

bool C_calculation::check_sun(QVector<double> coordinate_object, QDateTime time)
{
    double d = 367.*time.date().year() - (7.*(time.date().year() + ((time.date().month()+9.)/12.)))/4. + (275.*time.date().month()) / 9. + time.date().day()-730530.;
    double w = 282.9404 + 4.70935 *pow(10,-5) * d;
    double a = 1.000000;
    double e = 0.016709-1.151 * pow(10,-9) * d ;
    double M = fmod(356.0470 + 0.9856002585 * d,360) ;
    if (M<0) M+= 360;
    double L = fmod(w + M,360);
    if (L<0) L+= 360;
    double E = M + (180. / M_PI) * e * sin (M/180.*M_PI) * (1 + e * cos (M/180.*M_PI));
    double x = cos (E/180.*M_PI) - e;
    double y = sin (E/180.*M_PI) * sqrt (1-e*e);
    double r = sqrt (x*x + y*y);
    double v = atan2 (y, x )/M_PI*180.;
    double lon = fmod(v + w,360.);
    x = r * cos(lon/180.*M_PI);
    y = r * sin (lon/180.*M_PI);
    double z = y * sin (23.4406/180*M_PI);
    y = y * cos (23.4406/180.*M_PI);
    r = sqrt (x*x + y*y + z*z);
    double RA = atan2 (y,x)/M_PI*180./15.;
    double Decl = atan2 (z, sqrt (x*x + y*y))/M_PI*180.;
    double SIDTIME = C_convert::startime(time,"second")/3600. + coordinate_object[0]/M_PI*180./15.;// + time.time().hour() + time.time().minute()/60 + time.time().second()/3600;
    double HA = (SIDTIME-RA)*15.;
    x = cos (HA/180.*M_PI) * cos (Decl/180.*M_PI);
    double Y = sin (HA/180.*M_PI) * cos (Decl/180.*M_PI);
    z = sin (Decl/180.*M_PI);
    double X = x * sin (coordinate_object[1]) - z * cos (coordinate_object[1]);
    double Z = x * cos (coordinate_object[1]) + z * sin (coordinate_object[1]);
    double azimuth  = atan2(Y,X)/M_PI*180. + 180;
    double altitude = atan2(Z, sqrt(X*X+Y*Y))/M_PI*180.;
    if (altitude>0) return true;
    else return false;
}

bool C_calculation::check_RL_zone_overview(QVector<double> coordinate_object, QVector<double> gsc0, QVector<double> gsc1, QVector<double> geo0, QVector<double> geo1, QVector<double> geo2, double angle1, double angle2)
{
    angle1 *= M_PI/180;
    angle2 *= M_PI/180;
    for (int var = 0; var < 2; ++var)
    {
        geo0[var] *= M_PI/180;
        geo1[var] *= M_PI/180;
        geo2[var] *= M_PI/180;
    }
    QVector<double> intermediat_result1 = display_RL_zone(gsc0,geo0,geo1,angle1,angle2);
    QVector<double> intermediat_result2 = display_RL_zone(gsc1,geo1,geo2,angle1,angle2);
    if (coordinate_object[1] > linear_dependence(coordinate_object[0],intermediat_result1[2],intermediat_result1[6],intermediat_result1[3],intermediat_result1[7]) &&
        coordinate_object[1] < linear_dependence(coordinate_object[0],intermediat_result2[2],intermediat_result2[6],intermediat_result2[3],intermediat_result2[7]) &&
            ((coordinate_object[0] > linear_dependence(coordinate_object[1],intermediat_result1[7],intermediat_result2[7],intermediat_result1[6],intermediat_result2[6]) &&
              coordinate_object[0] < linear_dependence(coordinate_object[1],intermediat_result1[5],intermediat_result2[5],intermediat_result1[4],intermediat_result2[4]))
             ||
             (coordinate_object[0] > linear_dependence(coordinate_object[1],intermediat_result1[1],intermediat_result2[1],intermediat_result1[0],intermediat_result2[0]) &&
              coordinate_object[0] < linear_dependence(coordinate_object[1],intermediat_result1[3],intermediat_result2[3],intermediat_result1[2],intermediat_result2[2]))))
        return true;
    else return false;
}

QVector<double> C_calculation::display_RL_zone(QVector<double> gsc1, QVector<double> geo1, QVector<double> geo2,double angle1, double angle2)
{
    QVector <double> result;
    double h = sqrt(gsc1[0]*gsc1[0]+gsc1[1]*gsc1[1]+gsc1[2]*gsc1[2])-RZ_cir;
    double anglei = atan2((geo2[1]-geo1[1]),(geo2[0]-geo1[0]));
    double d1 = tan(angle1)*h;
    double d2 = tan(angle2)*h;
    result.push_back(geo1[0]+sin(anglei)*d1/RZ_cir);//x11
    result.push_back(geo1[1]-cos(anglei)*d1/RZ_cir);//y11
    result.push_back(geo1[0]+sin(anglei)*d2/RZ_cir);//x12
    result.push_back(geo1[1]-cos(anglei)*d2/RZ_cir);//y12

    result.push_back(geo1[0]-sin(anglei)*d1/RZ_cir);//x21
    result.push_back(geo1[1]+cos(anglei)*d1/RZ_cir);//y21
    result.push_back(geo1[0]-sin(anglei)*d2/RZ_cir);//x22
    result.push_back(geo1[1]+cos(anglei)*d2/RZ_cir);//y22
    return result;
}


int C_calculation::count_time_viewing_second(QDateTime time_begin, QVector<QVector<double> > coordinate_IRP, QVector<QVector<double> > coordinate_shooting_areas, QVector <C_SpaceCraft> space_crafts, QVector<QVector<double> > coordinate_IBP)
{
    //qDebug()<<modern_count_time_viewing_second(time_begin,coordinate_IRP,coordinate_shooting_areas,space_crafts,coordinate_IBP)/3600.;
    QDateTime time_current = time_begin;
    int step = 10;//шаг при расчете
    int check = coordinate_shooting_areas.size();
    QVector <double> gsc1,geo1;
    QVector <QVector <QVector <double> > > gsc (space_crafts.size(),QVector <QVector <double> > ());
    QVector <QVector <QVector <double> > > geo(space_crafts.size(),QVector <QVector <double> > ());
    QVector<QVector<QDateTime> > result (space_crafts.size(),QVector <QDateTime> ());
    while (check!=0)
    {
        for (int sc = 0; sc < space_crafts.size(); ++sc)
        {
            gsc1 = C_convert::AgscToGsc(C_convert::KepToAgsc(space_crafts[sc].KEO),time_current);
            geo1 = C_convert::GscToGeo(gsc1);
            if (space_crafts.at(sc).type == RLN)
            {
                gsc[sc].push_back(gsc1);
                geo[sc].push_back(geo1);
                if (gsc[sc].size()>3)
                {
                    gsc[sc].remove(0);
                    geo[sc].remove(0);
                }
            }
            if (result[sc].size() == 0)
                for (int ibp = 0; ibp < coordinate_IBP.size(); ++ibp)
                {
                    if (check_OE_zone_overview(coordinate_IBP[ibp],gsc1,83,time_current,false))
                    {result[sc].push_back(time_current);break;}
                }
            else
                if (result[sc].size()%2 == 1)
                    for (int shooting_area = 0; shooting_area < coordinate_shooting_areas.size(); ++shooting_area)
                    {
                        if (space_crafts.at(sc).type == OEN)
                        {if (check_OE_zone_overview(coordinate_shooting_areas[shooting_area],gsc1,space_crafts.at(sc).angle1,time_current,true))
                            {
                                result[sc].push_back(time_current);
                                coordinate_shooting_areas.remove(shooting_area);
                                break;
                            }}                     
                        if (space_crafts.at(sc).type == RLN)
                            if (geo[sc].size() == 3 && check_RL_zone_overview(coordinate_shooting_areas[shooting_area],gsc[sc][0],gsc[sc][1],geo[sc][0],geo[sc][1],geo[sc][2],space_crafts.at(sc).angle1,space_crafts.at(sc).angle2))
                            {
                                result[sc].push_back(time_current);
                                coordinate_shooting_areas.remove(shooting_area);
                                break;
                            }
                    }
                else
                    for (int irp = 0; irp < coordinate_IRP.size(); ++irp)
                        if (check_OE_zone_overview(coordinate_IRP[irp],gsc1,83,time_current,false))
                        {result[sc].push_back(time_current);check --;break;}
            space_crafts[sc] = move(space_crafts[sc],space_crafts.at(sc).KEO.datetime.secsTo(time_current));
        }
        time_current = time_current.addSecs(step);
    }
    return time_begin.secsTo(time_current);
}

QVector<double> C_calculation::count_evaluation(QVector <double> time)
{
    QVector <double> result;
    double mathematical_expectation = 0;
    double dispersion = 0;
    double asymmetry = 0;
    double excess = 0;
    for (int var = 0; var < time.size(); ++var)
        mathematical_expectation += time[var];
    mathematical_expectation /= time.size();
    for (int var = 0; var < time.size(); ++var)
    {
        dispersion += pow(time[var] - mathematical_expectation,2);
        asymmetry += pow(time[var] - mathematical_expectation,3);
        excess += pow(time[var] - mathematical_expectation,4);
    }
    dispersion /= time.size();
    double square_deviation = sqrt(dispersion);
    asymmetry /= time.size() * pow(square_deviation,3);
    excess /= time.size() * pow(square_deviation,4);
    excess -= 3;
    result.push_back(mathematical_expectation);
    result.push_back(square_deviation);
    result.push_back(asymmetry);
    result.push_back(excess);
    return result;
}

int C_calculation::count_time_viewnig(int step, QDateTime normally_distributed_time, QVector<S_ground_object> *coordinate_IRP, QVector<S_ground_object> *coordinate_shooting_areas, QVector<C_SpaceCraft> *space_crafts, QVector<S_ground_object> *coordinate_IBP)
{
    QVector < QVector <double> > vector_coordinate_IRP, vector_coordinate_shooting_areas,vector_coordinate_IBP;
    S_ground_object GO;
    QVector <C_SpaceCraft> scs;
    for (int var = 0; var < coordinate_IRP->size(); ++var) {
        GO = coordinate_IBP->at(var);
        vector_coordinate_IBP.push_back(QVector <double> (2));
        vector_coordinate_IBP[var][0] = GO.longitude;
        vector_coordinate_IBP[var][1] = GO.latitude;
    }
    for (int var = 0; var < coordinate_IRP->size(); ++var) {
        GO = coordinate_IRP->at(var);
        vector_coordinate_IRP.push_back(QVector <double> (2));
        vector_coordinate_IRP[var][0] = GO.longitude;
        vector_coordinate_IRP[var][1] = GO.latitude;
    }
    for (int var = 0; var < coordinate_shooting_areas->size(); ++var) {
        GO = coordinate_shooting_areas->at(var);
        vector_coordinate_shooting_areas.push_back(QVector <double> (2));
        vector_coordinate_shooting_areas[var][0] = GO.longitude;
        vector_coordinate_shooting_areas[var][1] = GO.latitude;
    }
    for (int var = 0; var < space_crafts->size(); ++var) {
        scs.append(space_crafts->operator [](var).copy());
    }

    QDateTime time_current = normally_distributed_time;
    int check = vector_coordinate_shooting_areas.size();
    QVector <double> gsc1,geo1;
    QVector <QVector <QVector <double> > > gsc (scs.size(),QVector <QVector <double> > ());
    QVector <QVector <QVector <double> > > geo(scs.size(),QVector <QVector <double> > ());
    QVector<QVector<QDateTime> > result (scs.size(),QVector <QDateTime> ());
    while (check!=0)
    {
        for (int sc = 0; sc < scs.size(); ++sc)
        {
            gsc1 = C_convert::AgscToGsc(C_convert::KepToAgsc(scs[sc].KEO),time_current);
            geo1 = C_convert::GscToGeo(gsc1);
            if (scs.at(sc).type == RLN)
            {
                gsc[sc].push_back(gsc1);
                geo[sc].push_back(geo1);
                if (gsc[sc].size()>3)
                {
                    gsc[sc].remove(0);
                    geo[sc].remove(0);
                }
            }
            if (result[sc].size() == 0)
                for (int ibp = 0; ibp < vector_coordinate_IBP.size(); ++ibp)
                {
                    if (check_OE_zone_overview(vector_coordinate_IBP[ibp],gsc1,83,time_current,false))
                    {result[sc].push_back(time_current);break;}
                }
            else
                if (result[sc].size()%2 == 1)
                    for (int shooting_area = 0; shooting_area < vector_coordinate_shooting_areas.size(); ++shooting_area)
                    {
                        if (scs.at(sc).type == OEN)
                        {if (check_OE_zone_overview(vector_coordinate_shooting_areas[shooting_area],gsc1,scs.at(sc).angle1,time_current,true))
                            {
                                result[sc].push_back(time_current);
                                vector_coordinate_shooting_areas.remove(shooting_area);
                                break;
                            }}
                        if (scs.at(sc).type == RLN)
                            if (geo[sc].size() == 3 && check_RL_zone_overview(vector_coordinate_shooting_areas[shooting_area],gsc[sc][0],gsc[sc][1],geo[sc][0],geo[sc][1],geo[sc][2],scs.at(sc).angle1,scs.at(sc).angle2))
                            {
                                result[sc].push_back(time_current);
                                vector_coordinate_shooting_areas.remove(shooting_area);
                                break;
                            }
                    }
                else
                    for (int irp = 0; irp < vector_coordinate_IRP.size(); ++irp)
                        if (check_OE_zone_overview(vector_coordinate_IRP[irp],gsc1,83,time_current,false))
                        {result[sc].push_back(time_current);check --;break;}
            scs[sc] = move(scs[sc],scs.at(sc).KEO.datetime.secsTo(time_current));
        }
        time_current = time_current.addSecs(step);
    }
    return normally_distributed_time.secsTo(time_current);
}

QVector<QVector<QDateTime> > C_calculation::count_time_viewnig_one_region(int step, QDateTime time_begin, QDateTime time_end, QVector<S_ground_object> *coordinate_IRP, S_ground_object coordinate_shooting_areas, QVector<C_SpaceCraft> *space_crafts, QVector<S_ground_object> *coordinate_IBP)
{
    QVector < QVector <double> > vector_coordinate_IRP, vector_coordinate_IBP;
    QVector <double> vector_coordinate_shooting_areas;
    vector_coordinate_shooting_areas.push_back(coordinate_shooting_areas.longitude);
    vector_coordinate_shooting_areas.push_back(coordinate_shooting_areas.latitude);
    S_ground_object GO;
    QVector <C_SpaceCraft> scs;
    for (int var = 0; var < coordinate_IBP->size(); ++var) {
        GO = coordinate_IBP->at(var);
        vector_coordinate_IBP.push_back(QVector <double> (2));
        vector_coordinate_IBP[var][0] = GO.longitude;
        vector_coordinate_IBP[var][1] = GO.latitude;
    }
    for (int var = 0; var < coordinate_IRP->size(); ++var) {
        GO = coordinate_IRP->at(var);
        vector_coordinate_IRP.push_back(QVector <double> (2));
        vector_coordinate_IRP[var][0] = GO.longitude;
        vector_coordinate_IRP[var][1] = GO.latitude;
    }
    for (int var = 0; var < space_crafts->size(); ++var) {
        scs.append(space_crafts->operator [](var).copy());
    }

    QDateTime time_current = time_begin;
    QVector <double> gsc1,geo1;
    QVector < QVector <QVector <double> > > gsc (scs.size(),QVector < QVector <double> > ());
    QVector < QVector <QVector <double> > > geo(scs.size(),QVector < QVector <double> > ());
    QVector < QVector < QDateTime > > result (scs.size(),QVector < QDateTime> ());
    while (time_current < time_end)
    {
        for (int sc = 0; sc < scs.size(); ++sc)
        {
            gsc1 = C_convert::AgscToGsc(C_convert::KepToAgsc(scs[sc].KEO),time_current);
            geo1 = C_convert::GscToGeo(gsc1);
            if (scs.at(sc).type == RLN)
            {
                gsc[sc].push_back(gsc1);
                geo[sc].push_back(geo1);
                if (gsc[sc].size()>3)
                {
                    gsc[sc].remove(0);
                    geo[sc].remove(0);
                }
            }
            if (result[sc].size()%3  == 0)
            {
                if (result[sc].size() == 0 || result[sc][result[sc].size()-1].secsTo(time_current) > 1800)
                for (int ibp = 0; ibp < vector_coordinate_IBP.size(); ++ibp)
                {
                    if (check_OE_zone_overview(vector_coordinate_IBP[ibp],gsc1,83,time_current,false))
                    {
                        result[sc].push_back(time_current);
                        qDebug() <<  "PU" <<  "\t" << coordinate_IBP->at(ibp).name <<  time_current.toString("dd.MM.yyyy hh:mm:ss")  <<  "\t" << scs[sc].name;
                        break;
                    }
                }
            }
            else
                if (result[sc].size()%3 == 1)
                    {
                        if (scs.at(sc).type == OEN && check_OE_zone_overview(vector_coordinate_shooting_areas,gsc1,scs.at(sc).angle1,time_current,true))
                        {result[sc].push_back(time_current);
                            qDebug() << "region" <<"\t" << "Сирия"  << time_current.toString("dd.MM.yyyy hh:mm:ss") <<  "\t" << scs[sc].name;}
                        if (scs.at(sc).type == RLN && geo[sc].size() == 3 && check_RL_zone_overview(vector_coordinate_shooting_areas,gsc[sc][0],gsc[sc][1],geo[sc][0],geo[sc][1],geo[sc][2],scs.at(sc).angle1,scs.at(sc).angle2))
                                {result[sc].push_back(time_current);
                            qDebug() << "region" << "\t" <<"Сирия"  << time_current.toString("dd.MM.yyyy hh:mm:ss")  <<  "\t" << scs[sc].name;}
                        if (scs.at(sc).type == RTN && check_OE_zone_overview(vector_coordinate_shooting_areas,gsc1,scs.at(sc).angle1,time_current,false))
                                {result[sc].push_back(time_current);
                        qDebug() <<  "region" << "\t" <<"Сирия"  << time_current.toString("dd.MM.yyyy hh:mm:ss")  <<  "\t" << scs[sc].name;}
                    }
                else
                    for (int irp = 0; irp < vector_coordinate_IRP.size(); ++irp)
                        if (check_OE_zone_overview(vector_coordinate_IRP[irp],gsc1,83,time_current,false) &&
                                ((scs.at(sc).type == OEN && !check_OE_zone_overview(vector_coordinate_shooting_areas,gsc1,scs.at(sc).angle1,time_current,true)) ||
                                 (scs.at(sc).type == RLN && geo[sc].size() == 3 && !check_RL_zone_overview(vector_coordinate_shooting_areas,gsc[sc][0],gsc[sc][1],geo[sc][0],geo[sc][1],geo[sc][2],scs.at(sc).angle1,scs.at(sc).angle2)) ||
                                 (scs.at(sc).type == RTN && !check_OE_zone_overview(vector_coordinate_shooting_areas,gsc1,scs.at(sc).angle1,time_current,false))))
                        {
                            result[sc].push_back(time_current);
                            qDebug() << "PPI" << "\t" << coordinate_IRP->at(irp).name << time_current.toString("dd.MM.yyyy hh:mm:ss")  <<  "\t" << scs[sc].name;
                            break;
                        }
            scs[sc] = move(scs[sc],scs.at(sc).KEO.datetime.secsTo(time_current));
        }
        time_current = time_current.addSecs(step);
    }
    return result;
}

double C_calculation::count_draconion_period(S_KEO oeo)
{
    return count_period(oeo.a)*(1+1.5*c20*pow(RZ_ell/oeo.a,2)*(3-2.5*pow(sin(oeo.i),2)-oeo.e*cos(oeo.om)*(1-5*pow(sin(oeo.i),2))));
}

double C_calculation::count_effective_period(C_SpaceCraft sc)
{
    return 2*M_PI/(wz-sc.ukhod.dOm/count_draconion_period(sc.KEO));
}

double C_calculation::count_period(double a)
{
    return 2*M_PI*sqrt(pow(a,3)/mu);
}

double C_calculation::count_next_U_viewing (double psi, double i, double u)
{
    if (i > 0)//прямые орбиты
    {
        if (psi > 0)//просмотр северного полушария
        {
            if (u < asin(sin(psi)/sin(i)) || u >= M_PI-asin(sin(psi)/sin(i))-0.001)
                return asin(sin(psi)/sin(i));
            else
                return M_PI-asin(sin(psi)/sin(i));
        }
        else//просмотр южного полушария
        {
            if (u < M_PI-asin(sin(psi)/sin(i)) || u >= 2*M_PI+asin(sin(psi)/sin(i))-0.001)
                return M_PI-asin(sin(psi)/sin(i));
            else
                return 2*M_PI+asin(sin(psi)/sin(i));
        }
    }
    else//обратные орбиты
    {
        if (psi > 0)//просмотр северного полушария
        {
            if (u < -asin(sin(psi)/sin(i)) || u >= M_PI+asin(sin(psi)/sin(i))-0.001)
                return -asin(sin(psi)/sin(i));
            else
                return M_PI+asin(sin(psi)/sin(i));
        }
        else//просмотр южного полушария
        {
            if (u < M_PI+asin(sin(psi)/sin(i)) || u >= 2*M_PI-asin(sin(psi)/sin(i))-0.001)
                return M_PI+asin(sin(psi)/sin(i));
            else
                return 2*M_PI-asin(sin(psi)/sin(i));
        }
    }
}

int C_calculation::count_difference_time_from_U (double u1, double u2, double a, double e, double om)
{
    if (tetta_to_E(u2 - om, e) > tetta_to_E(u1 - om, e))
        return int( E_to_tau(tetta_to_E(u2 - om, e),e,a)- E_to_tau(tetta_to_E(u1 - om, e),e,a));
    else
        return int( E_to_tau(tetta_to_E(u2 - om, e)+M_PI*2,e,a)- E_to_tau(tetta_to_E(u1 - om, e),e,a));
}

double C_calculation::tetta_to_E(double tetta, double e)
{
    double E = 2*atan(sqrt((1-e)/(1+e))*tan(tetta/2));
        if (tetta < M_PI)
        {
            if (E < 0)
                E += M_PI;
        }
        else
            if (E < 0)
                E += 2*M_PI;
            else
                E += M_PI;
    return E;
}

double C_calculation::E_to_tau(double E, double e, double a)
{
    return C_calculation::count_period(a)/2/M_PI*(E-e*sin(E));
}

double C_calculation::count_fizone(double r, double gamma)
{
    if (asin(RZ_cir/r) < gamma)
        return acos(RZ_cir/r);
    return asin(r/RZ_cir*sin(gamma))-gamma;
}

double C_calculation::count_longitude_viewing(double i, double u, double psi)
{
    if (cos(u)/cos(psi) > 0)
        return fmod(2*M_PI+asin(cos(i)*sin(u)/cos(psi)),2*M_PI);
    else
        return M_PI - asin(cos(i)*sin(u)/cos(psi));
}

double C_calculation::count_KEO_to_radius(double a, double e, double E)
{
    return a*(1-e*cos(E));
}

QVector<double> C_calculation::count_left_and_right_longitude_viewing_on_latitude(double psi, C_SpaceCraft sc)
{
    QVector <double> result;
    double u = sc.KEO.om+tau_to_tetta(sc.KEO.tau,sc.KEO.a,sc.KEO.e);
    double fizone = count_fizone(count_KEO_to_radius( sc.KEO.a,sc.KEO.e,tetta_to_E(tau_to_tetta(sc.KEO.tau,sc.KEO.a,sc.KEO.e),sc.KEO.e)),sc.angle1/180*M_PI);
    double longitude_viewing = count_longitude_viewing( sc.KEO.i, u, psi);
    double delta_longitude_viewing = acos((cos(fizone)-pow(sin(psi),2))/pow(cos(psi),2));
    if (sc.KEO.i > 0 && sc.KEO.i < M_PI/2)
    {result.push_back(M_PI*2 + longitude_viewing - delta_longitude_viewing - u*count_draconion_period(sc.KEO)/count_effective_period(sc));
    result.push_back(M_PI*2 + longitude_viewing + delta_longitude_viewing - u*count_draconion_period(sc.KEO)/count_effective_period(sc));}
    else
    {result.push_back(M_PI*2 + longitude_viewing - delta_longitude_viewing + u*count_draconion_period(sc.KEO)/count_effective_period(sc));
    result.push_back(M_PI*2 + longitude_viewing + delta_longitude_viewing + u*count_draconion_period(sc.KEO)/count_effective_period(sc));}
    return result;
}

QDateTime C_calculation::find_min_datetime(QVector<QDateTime> vector_time, QString min_max)
{
    QDateTime result = vector_time[0];
    if (min_max == "min")
    {
        for (int var = 0; var < vector_time.size(); ++var)
            if (result.secsTo(vector_time[var]) < 0)
                result = vector_time[var];
    }
    else
    {
        for (int var = 0; var < vector_time.size(); ++var)
            if (result.secsTo(vector_time[var]) > 0)
                result = vector_time[var];
    }
    return result;
}

int C_calculation::modern_count_time_viewing_second(QDateTime time_begin, QVector<QVector<double> > coordinate_IRP, QVector<QVector<double> > coordinate_shooting_areas, QVector<C_SpaceCraft> *space_crafts, QVector<QVector<double> > coordinate_IBP)
{
    for (int var = 0; var < coordinate_shooting_areas.size(); ++var)
        if (coordinate_shooting_areas[var][0] < 0)
            coordinate_shooting_areas[var][0] += M_PI*2;
    for (int var = 0; var < coordinate_IBP.size(); ++var)
        if (coordinate_IBP[var][0] < 0)
            coordinate_IBP[var][0] += M_PI*2;
    for (int var = 0; var < coordinate_IRP.size(); ++var)
        if (coordinate_IRP[var][0] < 0)
            coordinate_IRP[var][0] += M_PI*2;
    QVector <double> left_and_right;
    double u1, u2;
    QVector <QDateTime> time_load (space_crafts->size(),time_begin.addYears(1)), time_viewing (coordinate_shooting_areas.size(),time_begin.addYears(1)), time_reset (coordinate_shooting_areas.size(),time_begin.addYears(2));
    C_SpaceCraft sc0, sc_before_load;
    for (int sc = 0; sc < space_crafts->size(); ++sc)
    {
        sc0 = space_crafts->operator [](sc).copy();
        sc0.angle1 = 83;
        sc_before_load = move(sc0,sc0.KEO.datetime.secsTo(time_begin));
        for (int ibp = 0; ibp < coordinate_IBP.size(); ++ibp)
        {
            sc0 = sc_before_load;
            u1 = sc0.KEO.om+tau_to_tetta(sc0.KEO.tau,sc0.KEO.a,sc0.KEO.e);
            while (time_load[sc].secsTo(sc0.KEO.datetime) < 0)
            {
                left_and_right = count_left_and_right_longitude_viewing_on_latitude(coordinate_IBP[ibp][1],sc0);
                if (M_PI*2 + coordinate_IBP[ibp][0] > left_and_right[0] && M_PI*2 + coordinate_IBP[ibp][0] < left_and_right[1] )
                {
                    time_load[sc] = sc0.KEO.datetime;
                    break;
                }
                u2 = count_next_U_viewing(coordinate_IBP[ibp][1],sc0.KEO.i,u1);
                if (u2 < u1)
                {
                    sc0 = move(sc0,count_difference_time_from_U(u1, 2*M_PI, sc0.KEO.a, sc0.KEO.e, sc0.KEO.om));
                    u1 = 0;
                    u2 = count_next_U_viewing(coordinate_IBP[ibp][1],sc0.KEO.i,u1);
                }
                sc0 = move(sc0,count_difference_time_from_U(u1, u2, sc0.KEO.a, sc0.KEO.e, sc0.KEO.om));
                u1 = u2;
            }
        }
        sc_before_load = move(sc_before_load,time_begin.secsTo(time_load[sc]));
        sc_before_load.angle1 = space_crafts->at(sc).angle1;
        for (int region = 0; region < coordinate_shooting_areas.size(); ++region)
        {
            sc0 = sc_before_load;
            u1 = sc0.KEO.om+tau_to_tetta(sc0.KEO.tau,sc0.KEO.a,sc0.KEO.e);
            while (time_reset[region].secsTo(sc0.KEO.datetime) < 0)
            {
                left_and_right = count_left_and_right_longitude_viewing_on_latitude(coordinate_shooting_areas[region][1],sc0);
                if (M_PI*2 + coordinate_shooting_areas[region][0] > left_and_right[0] && M_PI*2 + coordinate_shooting_areas[region][0] < left_and_right[1])
                {
                    if (sc0.type == RLN)
                    {
                        time_viewing[region] = sc0.KEO.datetime;
                        break;
                    }
                    else
                        if (check_sun(coordinate_shooting_areas[region],sc0.KEO.datetime))
                        {
                            time_viewing[region] = sc0.KEO.datetime;
                            break;
                        }
                }
                u2 = count_next_U_viewing(coordinate_shooting_areas[region][1],sc0.KEO.i,u1);
                if (u2 < u1)
                {
                    sc0 = move(sc0,count_difference_time_from_U(u1, 2*M_PI, sc0.KEO.a, sc0.KEO.e, sc0.KEO.om));
                    u1 = 0;
                    u2 = count_next_U_viewing(coordinate_shooting_areas[region][1],sc0.KEO.i,u1);
                }
                sc0 = move(sc0,count_difference_time_from_U(u1, u2, sc0.KEO.a, sc0.KEO.e, sc0.KEO.om));
                u1 = u2;
            }
            if (time_viewing[region] == sc0.KEO.datetime)
            {
                sc0.angle1 = 83;
                sc_before_load = sc0;
                for (int irp = 0; irp < coordinate_IRP.size(); ++irp)
                {
                    sc0 = sc_before_load;
                    u1 = sc0.KEO.om+tau_to_tetta(sc0.KEO.tau,sc0.KEO.a,sc0.KEO.e);
                    while (time_reset[region].secsTo(sc0.KEO.datetime) < 0)
                    {
                        left_and_right = count_left_and_right_longitude_viewing_on_latitude(coordinate_IRP[irp][1],sc0);
                        if (M_PI*2 + coordinate_IRP[irp][0] > left_and_right[0] && M_PI*2 + coordinate_IRP[irp][0] < left_and_right[1])
                        {
                            time_reset[region] = sc0.KEO.datetime;
                            break;
                        }
                        u2 = count_next_U_viewing(coordinate_IRP[irp][1],sc0.KEO.i,u1);
                        if (u2 < u1)
                        {
                            sc0 = move(sc0,count_difference_time_from_U(u1, 2*M_PI, sc0.KEO.a, sc0.KEO.e, sc0.KEO.om));
                            u1 = 0;
                            u2 = count_next_U_viewing(coordinate_IRP[irp][1],sc0.KEO.i,u1);
                        }
                        sc0 = move(sc0,count_difference_time_from_U(u1, u2, sc0.KEO.a, sc0.KEO.e, sc0.KEO.om));
                        u1 = u2;
                    }
                }
            }
        }
    }
    return time_begin.secsTo(find_min_datetime(time_reset,"max"));
}

QVector<int> C_calculation::count_variational_series(QVector<int> interval_assessment, QDateTime time_begin, QDateTime time_end)
{
    QVector <int> intermediat_intervals;
    double number;
    interval_assessment.push_back(time_begin.toTime_t());
    interval_assessment.push_back(time_end.toTime_t());
    for (int var = 0; var < interval_assessment.size(); ++var)
    {
        number = std::distance(interval_assessment.begin(), std::min_element(interval_assessment.begin(),interval_assessment.end()));
        intermediat_intervals.push_back(interval_assessment[number]);
        interval_assessment[number] = interval_assessment[std::distance(interval_assessment.begin(), std::max_element(interval_assessment.begin(),interval_assessment.end()))];
    }

    interval_assessment.clear();

    for (int var = 0; var < intermediat_intervals.size()-1; ++var)
        interval_assessment.push_back(intermediat_intervals[var+1]-intermediat_intervals[var]);

    intermediat_intervals.clear();

    for (int var = 0; var < interval_assessment.size(); ++var)
    {
        number = std::distance(interval_assessment.begin(), std::min_element(interval_assessment.begin(),interval_assessment.end()));
        intermediat_intervals.push_back(interval_assessment[number]);
        interval_assessment[number] = interval_assessment[std::distance(interval_assessment.begin(), std::max_element(interval_assessment.begin(),interval_assessment.end()))] + 1;
    }

    return intermediat_intervals;
}

QVector<int> C_calculation::count_variational_series(QVector<int> interval_assessment)
{
    QVector <int> intermediat_intervals;
    double number;
    for (int var = 0; var < interval_assessment.size(); ++var)
    {
        number = std::distance(interval_assessment.begin(), std::min_element(interval_assessment.begin(),interval_assessment.end()));
        intermediat_intervals.push_back(interval_assessment[number]);
        interval_assessment[number] = interval_assessment[std::distance(interval_assessment.begin(), std::max_element(interval_assessment.begin(),interval_assessment.end()))];
    }//выстраиваю по порядку

    return intermediat_intervals;
}

QVector<double> C_calculation::count_effiency_ratio(QVector<QVector<QDateTime> > vector_time, QDateTime time_begin, QDateTime time_end)
{
    QVector<QVector<QDateTime> > comfortable_vector_time,copy;
    for (int sc = 0; sc < vector_time.size(); ++sc)
        for (int cycle = 0; cycle < vector_time[sc].size() && vector_time[sc].size() >= cycle+3 ; cycle += 3)
        {
            QVector<QDateTime> vector_cycle;
            vector_cycle.push_back(vector_time[sc][cycle]);
            vector_cycle.push_back(vector_time[sc][cycle+1]);
            vector_cycle.push_back(vector_time[sc][cycle+2]);
            comfortable_vector_time.push_back(vector_cycle);
        }
    copy = comfortable_vector_time;
    double frequency_service_per_day = float(comfortable_vector_time.size())/float((time_begin.daysTo(time_end)));//частота обслуживания в сутки
    double frequency_information_update = 0;//периодичность обновления информации
    double efficiency_information_delivery;//оперативность доведения информации    
    for (int var = 0; var < comfortable_vector_time.size(); ++var)         
    {
        frequency_information_update += comfortable_vector_time[var][0].secsTo(comfortable_vector_time[var][2]);
        efficiency_information_delivery += comfortable_vector_time[var][1].secsTo(comfortable_vector_time[var][2]);
    }
    frequency_information_update /= comfortable_vector_time.size();
    efficiency_information_delivery /= comfortable_vector_time.size();
    
    QVector<QDateTime> ranked_service_vector;
    QDateTime help;        
    int help1;
    while (comfortable_vector_time.size() > 0)
    {
        help = comfortable_vector_time[0][1];        
        for (int var = 0; var < comfortable_vector_time.size(); ++var) 
        {
            if (comfortable_vector_time[var][1] <= help)
            {
                help = comfortable_vector_time[var][1];
                help1 = var;
            }
        }
        comfortable_vector_time.remove(help1);
        ranked_service_vector.push_back(help);
    }
    double average_time_gap_services = 0;//среднее время разрыва в обслуживании
    double largest_gap_service = ranked_service_vector[0].secsTo(ranked_service_vector[1]);//наибольший разрыв в обслуживании
    for (int var = 0; var < ranked_service_vector.size()-1; ++var)
    {
        average_time_gap_services += ranked_service_vector[var].secsTo(ranked_service_vector[var+1]);
        if (largest_gap_service < ranked_service_vector[var].secsTo(ranked_service_vector[var+1]))
            largest_gap_service = ranked_service_vector[var].secsTo(ranked_service_vector[var+1]);
    }
    average_time_gap_services /= ranked_service_vector.size() - 1;
    ranked_service_vector.clear();
    comfortable_vector_time = copy;
    while (comfortable_vector_time.size() > 0)
    {
        help = comfortable_vector_time[0][2];
        for (int var = 0; var < comfortable_vector_time.size(); ++var)
        {
            if (comfortable_vector_time[var][2] <= help)
            {
                help = comfortable_vector_time[var][2];
                help1 = var;
            }
        }
        comfortable_vector_time.remove(help1);
        ranked_service_vector.push_back(help);
    }
    double average_time_gap_reset = 0;//среднее время разрыва сброса
    for (int var = 0; var < ranked_service_vector.size()-1; ++var)
        average_time_gap_reset += ranked_service_vector[var].secsTo(ranked_service_vector[var+1]);
    average_time_gap_reset /= ranked_service_vector.size() - 1;

    QVector<double> result;
    result.push_back(frequency_information_update);//периодичность обновления информации
    result.push_back(frequency_service_per_day);//частота обслуживания в сутки
    result.push_back(average_time_gap_reset);//среднее время разрыва сброса
    result.push_back(average_time_gap_services);//среднее время разрыва в обслуживании
    result.push_back(largest_gap_service);//наибольший разрыв в обслуживании
    result.push_back(efficiency_information_delivery);//оперативность доведения информации
    return result;
}


double C_calculation::tau_to_tetta(double tau, double a, double e)
{
    if (tau < 0)
        tau += count_period(a);
    tau/=sqrt(a*a*a/mu);
    double E = tau;//в первом приближении E = M, где M = E - e*sinE - средняя аномалия
    while (E-e*sin(E)-tau>0.00000001 or E-e*sin(E)-tau<-0.00000001)
    {
        E=tau+e*sin(E);//fmod(tau+e*sin(E),2*M_PI);
    }
    double tetta = 2*atan(tan(E/2)*sqrt((1+e)/(1-e)));
    if (E < M_PI)
    {
        if (tetta < 0)
            tetta += M_PI;
    }
    else
        if (tetta < 0)
            tetta += 2*M_PI;
        else
            tetta += M_PI;
    return fmod (tetta, M_PI*2);
}
