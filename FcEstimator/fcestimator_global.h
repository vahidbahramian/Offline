#ifndef FCESTIMATOR_GLOBAL_H
#define FCESTIMATOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FCESTIMATOR_LIBRARY)
#  define FCESTIMATORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FCESTIMATORSHARED_EXPORT Q_DECL_IMPORT
#endif
extern "C" FCESTIMATORSHARED_EXPORT double FC_Estimate(double x_Ruler,double y_Ruler);

#endif // FCESTIMATOR_GLOBAL_H
