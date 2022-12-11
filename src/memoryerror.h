/**
 * @file memoryerror.h
 * @author Siim Kasela (siim.kasela@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MEMORYERROR_H
#define MEMORYERROR_H

#include "enemy.h"
/**
 * @brief Class for the MemoryError type enemy.
 * 
 */
class MemoryError :public Enemy
{
    Q_OBJECT
public:

    MemoryError(MemoryErrorType subType, QList<QPointF> path, QList<QPoint> matrixPath);

private:
    /**
     * @brief The name of the error.
     * 
     */
    MemoryErrorType name_;
};

#endif // MEMORYERROR_H
