#include "convert.h"
#include <cmath>
#include <QTime>
#include <QDebug>
#include "constants.h"
#include "calculation.h"
#include "mainwindow.h"
#include "constants.h"

double C_convert::r(double p, double e, double tt)
{
    return p/(1+e*cos(tt));
}

double C_convert::vr(double p, double e, double tt)
{
    return sqrt(mu/p)*e*sin(tt);
}

double C_convert::vt(double p, double e, double tt)
{
    return sqrt(mu/p)*(1+e*cos(tt));
}

double C_convert::startime(QDateTime datetime, QString type)
{
    int d=datetime.date().day();
    int m=datetime.date().month();
    int y=datetime.date().year();
    int h=datetime.time().hour()-3;
    double mi=datetime.time().minute()+datetime.time().second()/60.;
    int a = int((14-m)/12);
    int yy = y + 4800 - a;
    int mm = m + 12*a - 3;
    int JDN = d + int((153*mm+2)/5) + 365*yy + int(yy/4) - int(yy/100) + int(yy/400) - 32045;
    double JD = JDN + (h-12)/24. + mi/1440.;
    double T = (JD - 2451545)/36525.;
    double S =((6*3600+41*60+50.54841+8640184.812866*T+0.093104*pow(T,2)-6.2/1000000.*pow(T,3))+h*3600+mi*60);
    if (type == "angle")
        return fmod(S*2*M_PI/86400,M_PI*2);//выводит время в радианах
    else
        return S;//выводит время в секундах
}

C_convert::C_convert()
{

}

QVector<double> C_convert::GscToAgsc(QVector<double> gsc, QDateTime datetime)
{
    QVector < double > result;
    double s=startime(datetime,"angle");
    result.push_back(cos(s)*gsc[0]-sin(s)*gsc[1]);
    result.push_back(cos(s)*gsc[1]+sin(s)*gsc[0]);
    result.push_back(gsc[2]);
    result.push_back(cos(s)*gsc[3]-sin(s)*gsc[4]-result[1]*wz);
    result.push_back(cos(s)*gsc[4]+sin(s)*gsc[3]+result[0]*wz);
    result.push_back(gsc[5]);
    return result;
}

QVector<double> C_convert::KepToAgsc(S_KEO keo)
{
    double tt = C_calculation::tau_to_tetta(keo.tau,keo.a,keo.e);
    double p = keo.a*(1-pow(keo.e,2));
    double r = C_convert::r(p,keo.e,tt);
    double vr = C_convert::vr(p,keo.e,tt);
    double vt = C_convert::vt(p,keo.e,tt);
    QVector < double > result;
    /*x*/result.push_back(r*(cos(keo.Om)*cos(keo.om+tt)-sin(keo.Om)*sin(keo.om+tt)*cos(keo.i)));
    /*y*/result.push_back(r*(sin(keo.Om)*cos(keo.om+tt)+cos(keo.Om)*sin(keo.om+tt)*cos(keo.i)));
    /*z*/result.push_back(r*sin(keo.om+tt)*sin(keo.i));
    /*vx*/result.push_back(vr*(cos(keo.Om)*cos(keo.om+tt)-sin(keo.Om)*sin(keo.om+tt)*cos(keo.i))-vt*(cos(keo.Om)*sin(keo.om+tt)+sin(keo.Om)*cos(keo.om+tt)*cos(keo.i)));
    /*vy*/result.push_back(vr*(sin(keo.Om)*cos(keo.om+tt)+cos(keo.Om)*sin(keo.om+tt)*cos(keo.i))-vt*(sin(keo.Om)*sin(keo.om+tt)-cos(keo.Om)*cos(keo.om+tt)*cos(keo.i)));
    /*vz*/result.push_back(vr*sin(keo.om+tt)*sin(keo.i)+vt*cos(keo.om+tt)*sin(keo.i));
    return result;
}
QVector<double> C_convert::AgscToGsc(QVector<double> agsc, QDateTime datetime)/*x0,y1,z2,vx3,vy4,vz5*/
{
    QVector < double > result;
    double s=startime(datetime,"angle");
    result.push_back(cos(s)*agsc[0]+sin(s)*agsc[1]);
    result.push_back(cos(s)*agsc[1]-sin(s)*agsc[0]);
    result.push_back(agsc[2]);
    result.push_back(cos(s)*agsc[3]+sin(s)*agsc[4]+result[1]*wz);
    result.push_back(cos(s)*agsc[4]-sin(s)*agsc[3]-result[0]*wz);
    result.push_back(agsc[5]);
    return result;
}
S_KEO C_convert::AgscToKep(QVector<double> agsc, QDateTime datetime)/*x0,y1,z2,vx3,vy4,vz5*/
{
    S_KEO oeo;
    double c1 = agsc[1]*agsc[5]-agsc[2]*agsc[4];
    double c2 = agsc[2]*agsc[3]-agsc[0]*agsc[5];
    double c3 = agsc[0]*agsc[4]-agsc[1]*agsc[3];
    double c = sqrt((c1*c1+c2*c2+c3*c3));
    double l1 = -mu*agsc[0]/pow(agsc[0]*agsc[0]+agsc[1]*agsc[1]+agsc[2]*agsc[2],0.5)+c3*agsc[4]-c2*agsc[5];
    double l2 = -mu*agsc[1]/pow(agsc[0]*agsc[0]+agsc[1]*agsc[1]+agsc[2]*agsc[2],0.5)+c1*agsc[5]-c3*agsc[3];
    double l3 = -mu*agsc[2]/pow(agsc[0]*agsc[0]+agsc[1]*agsc[1]+agsc[2]*agsc[2],0.5)+c2*agsc[3]-c1*agsc[4];
    double l = sqrt((l1*l1+l2*l2+l3*l3));
    double r = sqrt(agsc[0]*agsc[0]+agsc[1]*agsc[1]+agsc[2]*agsc[2]);
    oeo.i = acos(c3/c);
    if (-c2/c/sin(oeo.i)>0)
        oeo.Om = fmod(asin(c1/c/sin(oeo.i))+2*M_PI,2*M_PI);
    else
        oeo.Om = M_PI-asin(c1/c/sin(oeo.i));
    if ((c1*l2-c2*l1)/l/c/sin(oeo.i)>0)
        oeo.om = fmod(asin(l3/l/sin(oeo.i))+2*M_PI,2*M_PI);
    else
        oeo.om = M_PI-asin(l3/l/sin(oeo.i));
    oeo.a = (c*c/mu)/(1-oeo.e*oeo.e);
    oeo.e = l/mu;
    double tetta;
    if (c*c-mu*r>0)
        tetta = fmod(asin((c*(agsc[0]*agsc[3]+agsc[1]*agsc[4]+agsc[2]*agsc[5]))/(l*r))+2*M_PI,2*M_PI);
    else
        tetta = M_PI-asin((c*(agsc[0]*agsc[3]+agsc[1]*agsc[4]+agsc[2]*agsc[5]))/(l*r));
    oeo.tau = C_calculation::E_to_tau(C_calculation::tetta_to_E(tetta,oeo.e),oeo.e,oeo.a);
    oeo.datetime = datetime;
    return oeo;
}
QVector<double> C_convert::GscToIsc(QVector<double> gsc,QVector<double> geo)
{
    QVector < double > result;
    result.push_back(-cos(geo[0])*sin(geo[1])*gsc[0]-sin(geo[0])*sin(geo[1])*gsc[1]+cos(geo[1])*gsc[2]);
    result.push_back(cos(geo[0])*cos(geo[1])*gsc[0]+sin(geo[0])*cos(geo[1])*gsc[1]+sin(geo[1])*gsc[2]-RZ_cir);//-pow(Rze,2)*(1-alzg)/sqrt(pow(Rze*(1-alzg)*cos(geo[1]),2)+pow(Rze*sin(geo[1]),2)));
    result.push_back(-sin(geo[0])*gsc[0]+cos(geo[0])*gsc[1]);
    result.push_back(-cos(geo[0])*sin(geo[1])*gsc[3]-sin(geo[0])*sin(geo[1])*gsc[4]+cos(geo[1])*gsc[5]);
    result.push_back(cos(geo[0])*cos(geo[1])*gsc[3]+sin(geo[0])*cos(geo[1])*gsc[4]+sin(geo[1])*gsc[5]);//-pow(Rze,2)*(1-alzg)/sqrt(pow(Rze*(1-alzg)*cos(geo[1]),2)+pow(Rze*sin(geo[1]),2)));
    result.push_back(-sin(geo[0])*gsc[3]+cos(geo[0])*gsc[4]);
    return result;
}

QVector<double> C_convert::IscToDAzYm(QVector<double> isc)
{
    QVector <double> result;
    result.push_back(sqrt(isc[0]*isc[0]+isc[1]*isc[1]+isc[2]*isc[2]));
    if (isc[2]>0) result.push_back(fmod(atan2(isc[2],isc[0]),2*M_PI));
    else result.push_back(fmod(2*M_PI+atan2(isc[2],isc[0]),2*M_PI));
    result.push_back(asin(isc[1]/result[0]));
    return result;
}

QVector<double> C_convert::AgscToOsc(QVector<double> agsc, S_KEO keo)
{

}

QVector<double> C_convert::GeoToGsc(QVector<double> geo)
{
    QVector <double> gsc;
    gsc.push_back(6371*cos(geo[1])*cos(geo[0]));//6293.5
    gsc.push_back(6371 *cos(geo[1])*sin(geo[0]));
    gsc.push_back(6371*sin(geo[1]));
    return gsc;
}
QVector<double> C_convert::GscToGeo(QVector<double> gsc)
{
    QVector <double> result;
    /*долгота*/
    if (gsc[1]>=0)
        result.push_back(acos(gsc[0]/pow(gsc[0]*gsc[0]+gsc[1]*gsc[1],0.5))/M_PI*180.);
    else
        result.push_back(360.-acos(gsc[0]/pow(gsc[0]*gsc[0]+gsc[1]*gsc[1],0.5))/M_PI*180.);
    /*широта*/
    result.push_back(asin(gsc[2]/pow((gsc[0]*gsc[0]+gsc[1]*gsc[1]+gsc[2]*gsc[2]),0.5))/M_PI*180.);
    return result;
}

QVector<double> C_convert::KepToGeo(QVector<double> kep)/*p0,e1,tt2,Om3,om4,i5*/
{
    QVector <double> result;
    /*широта*/result.push_back(asin(sin(kep[5])*sin(kep[2]+kep[4])));
    /*долгота*/result.push_back(kep[3]-wz*QTime().currentTime().second()+asin(tan(kep[0])/tan(kep[5])));
}
