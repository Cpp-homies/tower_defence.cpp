#include "memoryerror.h"

MemoryError::MemoryError(MemoryErrorType subType, QList<QPointF> path,QList<QPoint> matrixPath): Enemy(EnemyType::MemoryError,path, matrixPath), name_(subType)
{
    switch (subType) {
    case MemoryErrorType::InvalidRead:
        setPixmap(QPixmap(":/images/Invalid_read.png"));
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 10;
        health_ = 20;
        speed_ = 15*name_;

        break;

    case MemoryErrorType::InvalidWrite:
        setPixmap(QPixmap(":/images/Invalid_write.png"));
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 10;
        health_ = 20;
        speed_ = 15;
        break;

    case MemoryErrorType::XBytesAreLost:
        setPixmap(QPixmap(":/images/bytes_lost.png"));
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 20;
        health_ = 50;
        speed_ = 5;
        break;

    case MemoryErrorType::MismatchedDeleteFree:
        setPixmap(QPixmap(":/images/Mismatched.png"));
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 20;
        health_ = 30;
        speed_ = 20;
        break;

    default:
        deleteLater();
        break;
    }
    pointValue_=health_;
}


