/**
 * @file compilererror.h
 * @author Siim Kasela (siim.kasela@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef COMPILERERROR_H
#define COMPILERERROR_H

#include "enemy.h"
/**
 * @brief Class for CompilerError type enemies
 * Inherits from class Enemy
 * 
 */
class CompilerError: public Enemy
{
    Q_OBJECT
public:
    CompilerError(CompilerErrorType subType, QList<QPointF> path,QList<QPoint> matrixPath);
    void die();
    void explodeException();

private:
    /**
     * @brief The name of the enemy
     * 
     */
    CompilerErrorType name_;
};

#endif // COMPILERERROR_H
