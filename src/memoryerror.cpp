#include "memoryerror.h"

MemoryError::MemoryError(MemoryErrorType subType, QList<QPointF> path, Game& game): Enemy(EnemyType::MemoryError,path, game), name_(subType)
{
    switch (subType) {
    case MemoryErrorType::InvalidRead:
        setPixmap(QPixmap(":/images/syntax_error3.png")); //TODO change image
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 10;
        health_ = 20;
        speed_ = 15*name_;

        break;

    case MemoryErrorType::InvalidWrite:
        setPixmap(QPixmap(":/images/syntax_error3.png")); //TODO change image
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 10;
        health_ = 20;
        speed_ = 15;
        break;

    case MemoryErrorType::XBytesAreLost:
        setPixmap(QPixmap(":/images/syntax_error3.png")); //TODO change image
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 20;
        health_ = 50;
        speed_ = 5;
        break;

    case MemoryErrorType::MismatchedDeleteFree:
        setPixmap(QPixmap(":/images/syntax_error3.png")); //TODO change image
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


