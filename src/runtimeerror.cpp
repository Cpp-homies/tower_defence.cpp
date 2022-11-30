#include "runtimeerror.h"
#include <QGraphicsScene>

RuntimeError::RuntimeError(RuntimeErrorType subType, QList<QPointF> path,QList<QPoint> matrixPath): Enemy(EnemyType::MemoryError,path, matrixPath), name_(subType)
{
    switch (subType) {

    case RuntimeErrorType::StackOverflow:
        setPixmap(QPixmap(":/images/Stack_overflow1.png"));
        setTransformOriginPoint(pixmap().height()/2, pixmap().width()/2);
        damage_ = 50;
        health_ = 100;
        speed_ = 30;
        break;

    case RuntimeErrorType::MemoryStackMinion:
        setPixmap(QPixmap(":/images/Stack_overflow6.png"));
        setTransformOriginPoint(pixmap().height()/2, pixmap().width()/2);
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

        if(health_/increments > (health_-damage)/increments && (health_-damage)/increments >=1)
        {
            int stage = (health_-damage)/increments-1;
            QList<QPixmap> stageImgs;
            stageImgs << QPixmap(":/images/Stack_overflow6.png") << QPixmap(":/images/Stack_overflow5.png")
                      << QPixmap(":/images/Stack_overflow4.png") << QPixmap(":/images/Stack_overflow3.png")
                      << QPixmap(":/images/Stack_overflow2.png") ;
            setPixmap(stageImgs[stage]);
            setTransformOriginPoint(pixmap().height()/2, pixmap().width()/2);
            QList<QPointF> path(path_);
            path.remove(0,point_index_);
            path.push_front(scenePos());
            QList<QPoint> matrixPath(matrixPath_);
            matrixPath.remove(0,point_index_);
            RuntimeError* minion = new RuntimeError(RuntimeErrorType::MemoryStackMinion,path, matrixPath);
            minion->setSpeed(speed_*2);
            emit addedEnemy((Enemy*)minion,1);
        }

    }
    health_-=damage;
    if(health_<=0)
    {
        die();
    }
}

