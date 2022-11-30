#ifndef MEMORYERROR_H
#define MEMORYERROR_H

#include "enemy.h"

class MemoryError :public Enemy
{
    Q_OBJECT
public:

    MemoryError(MemoryErrorType subType, QList<QPointF> path, QList<QPoint> matrixPath);

private:
    MemoryErrorType name_;
};

#endif // MEMORYERROR_H
