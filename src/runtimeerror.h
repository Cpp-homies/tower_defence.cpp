/**
 * @file runtimeerror.h
 * @author Siim Kasela (siim.kasela@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H

#include "enemy.h"
/**
 * @brief Class for the RuntimeError type enemies.
 * 
 */
class RuntimeError: public Enemy
{
    Q_OBJECT
public:

    RuntimeError(RuntimeErrorType subType, QList<QPointF> path,QList<QPoint> matrixPath);
    void takeDamage(int damage);

private:
    /**
     * @brief The subtype of the enemy.
     * 
     */
    RuntimeErrorType name_;
};

#endif // RUNTIMEERROR_H
