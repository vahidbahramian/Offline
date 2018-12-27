#ifndef FCESTIMATOR_H
#define FCESTIMATOR_H

#include "fcestimator_global.h"

class FCESTIMATORSHARED_EXPORT FcEstimator
{

public:
    FcEstimator();

};
extern "C" FCESTIMATORSHARED_EXPORT double FC_Estimate(double x_Ruler,double y_Ruler);

#endif // FCESTIMATOR_H
