#include "spacecraft.h"

C_SpaceCraft::C_SpaceCraft()
{
    main_curve = new QwtPlotCurve();
}

C_SpaceCraft C_SpaceCraft::copy()
{
    C_SpaceCraft copy;
    copy.angle1 = angle1;
    copy.angle2 = angle2;
    copy.geo = geo;
    copy.gsc = gsc;
    copy.KEO = KEO;
    copy.main_curve = main_curve;
    copy.name = name;
    copy.type = type;
    copy.ukhod = ukhod;
    return copy;
}



