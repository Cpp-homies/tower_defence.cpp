/**
 * @file memoryerror.cpp
 * @author Siim Kasela (siim.kasela@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "memoryerror.h"
/**
 * @brief Construct a new Memory Error:: Memory Error object
 * Checks the subtype of the error and set the members accordingly.
 * @param subType 
 * @param path 
 * @param matrixPath 
 */
MemoryError::MemoryError(MemoryErrorType subType, QList<QPointF> path,QList<QPoint> matrixPath): Enemy(EnemyType::MemoryError,path, matrixPath), name_(subType)
{
    switch (subType) {
    case MemoryErrorType::InvalidRead:
        setPixmap(QPixmap(":/images/Invalid_read.png"));
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 10;
        health_ = 20;
        speed_ = 40;

        break;

    case MemoryErrorType::InvalidWrite:
        setPixmap(QPixmap(":/images/Invalid_write.png"));
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 5;
        health_ = 10;
        speed_ = 70;
        break;

    case MemoryErrorType::XBytesAreLost:
        setPixmap(QPixmap(":/images/bytes_lost.png"));
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 20;
        health_ = 200;
        speed_ = 5;
        break;

    case MemoryErrorType::MismatchedDeleteFree:
        setPixmap(QPixmap(":/images/Mismatched.png"));
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 20;
        health_ = 200;
        speed_ = 40;
        break;

    default:
        deleteLater();
        break;
    }
    pointValue_=health_;
}


