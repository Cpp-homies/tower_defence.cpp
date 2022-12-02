#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H

#include "enemy.h"

class RuntimeError: public Enemy
{
    Q_OBJECT
public:

    RuntimeError(RuntimeErrorType subType, QList<QPointF> path,QList<QPoint> matrixPath);
    void takeDamage(int damage);

private:

    RuntimeErrorType name_;
};

#endif // RUNTIMEERROR_H
