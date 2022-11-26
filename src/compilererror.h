#ifndef COMPILERERROR_H
#define COMPILERERROR_H

#include "enemy.h"

class CompilerError: public Enemy
{
    Q_OBJECT
public:
    CompilerError(EnemyName subType, QList<QPointF> path, Game& game);
    void die();
    void explodeException();
};

#endif // COMPILERERROR_H
