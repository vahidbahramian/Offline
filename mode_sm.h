#ifndef MODE_SM_H
#define MODE_SM_H
#include <QThread>
#include <QString>
#include <QWaitCondition>
#include <QMutex>
#include "QTimer"
#include <vector>
#include "configuration.h"
#include "processor.h"

class Mode_SM : public QThread
{
    Q_OBJECT
public:
    Mode_SM(QObject *parent);
};

#endif // MODE_SM_H
