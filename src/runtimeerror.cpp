#include "runtimeerror.h"

RuntimeError::RuntimeError(RuntimeErrorType subType, QList<QPointF> path, Game& game): Enemy(EnemyType::MemoryError,path, game), name_(subType)
{
    switch (subType) {

    case RuntimeErrorType::StackOverflow:
        setPixmap(QPixmap(":/images/syntax_error1.png")); //TODO change image
        setTransformOriginPoint(pixmap().height()/2, pixmap().width()/2);
        damage_ = 50;
        health_ = 100;
        speed_ = 30;
        break;

    case RuntimeErrorType::MemoryStackMinion:
        setPixmap(QPixmap(":/images/syntax_error3.png")); //TODO change image
        setOffset(QPointF(16,16));
        setTransformOriginPoint(pixmap().height(), pixmap().width());
        damage_ = 10;
        health_ = 5;
        speed_ = 5;
        break;

    default:
        deleteLater();
        break;
    }
    pointValue_=health_;
}

void RuntimeError::takeDamage(int damage)
{
    if(name_==RuntimeErrorType::StackOverflow)
    {
        int stages = 6;
        int increments = pointValue_/stages;

        if((health_-1)/increments > (health_-damage)/increments)
        {
            int stage = (health_-damage)/increments+1;
//                setPixmap(QPixmap(":/images/syntax_error1.png")); //TODO change boss pixmap according to stage
//                setTransformOriginPoint(pixmap().height()/2, pixmap().width()/2);
            QList<QPointF> path(path_);
            path.remove(0,point_index_);
            path.push_front(scenePos());
            RuntimeError* minion = new RuntimeError(RuntimeErrorType::MemoryStackMinion,path, game_ );
            minion->setSpeed(speed_*2);
            minion->startMove();
            game_.addItem(minion);
        }

    }
    health_-=damage;
    if(health_<=0)
    {
        die();
    }
}

