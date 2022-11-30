#ifndef COMPILERERROR_H
#define COMPILERERROR_H

#include "enemy.h"

class CompilerError: public Enemy
{
    Q_OBJECT
public:
    CompilerError(CompilerErrorType subType, QList<QPointF> path);
    void die();
    void explodeException();

private:

    CompilerErrorType name_;
};

#endif // COMPILERERROR_H
