#ifndef C_CONVERT_H
#define C_CONVERT_H
#include <QVector>
#include "spacecraft.h"


class C_convert
{
private:
    static double r(double p,double e,double tt);
    static double vr(double p,double e,double tt);
    static double vt(double p,double e,double tt);
public:
    static double startime (QDateTime datetime, QString type);
    C_convert();
    static QVector <double> GscToAgsc (QVector <double> gsc, QDateTime datetime);
    static QVector <double> KepToAgsc(S_KEO keo);
    static QVector <double> AgscToGsc (QVector <double> agsc, QDateTime datetime);
    static S_KEO AgscToKep (QVector<double> agsc, QDateTime datetime);
    static QVector <double> GscToGeo (QVector<double> gsc);
    static QVector <double> KepToGeo (QVector<double> kep);
    static QVector <double> GscToIsc (QVector<double> gsc, QVector<double> geo);
    static QVector <double> IscToDAzYm (QVector<double> isc);
    static QVector <double> AgscToOsc (QVector<double> agsc, S_KEO keo);
    static QVector <double> GeoToGsc (QVector<double> geo);
};

#endif // C_CONVERT_H
