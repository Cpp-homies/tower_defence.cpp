/**
 * @file runtimeerror.cpp
 * @author Siim Kasela (siim.kasela@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "runtimeerror.h"
#include <QGraphicsScene>
#include "game.h"
/**
 * @brief Construct a new Runtime Error:: Runtime Error object
 * Checks the subType of the enemy and sets the members accordingly.
 * @param subType 
 * @param path 
 * @param matrixPath 
 */
RuntimeError::RuntimeError(RuntimeErrorType subType, QList<QPointF> path,QList<QPoint> matrixPath): Enemy(EnemyType::MemoryError,path, matrixPath), name_(subType)
{
    switch (subType) {

    case RuntimeErrorType::StackOverflow:
        setPixmap(QPixmap(":/images/Stack_overflow1.png"));
        setTransformOriginPoint(pixmap().height()/2, pixmap().width()/2);
        damage_ = 500;
        health_ = 5000;
        speed_ = 5;
        break;

    case RuntimeErrorType::MemoryStackMinion:
        setPixmap(QPixmap(":/images/Stack_overflow6.png"));
        setTransformOriginPoint(pixmap().height()/2, pixmap().width()/2);
        damage_ = 10;
        health_ = 1000;
        speed_ = 30;
        break;

    default:
        deleteLater();
        break;
    }
    pointValue_=health_;
}
/**
 * @brief Is called when the enemy takes damage from a projectile.
 * Since this a boss enemy it releases faster minions and has a nr of stages.
 * Minions are released after every stage.
 * 
 * @param damage The amount of damage taken by the projectile.
 */
void RuntimeError::takeDamage(int damage)
{
    if(name_==RuntimeErrorType::StackOverflow)
    {
        int stages = 6;
        int increments = pointValue_/stages;

        if(health_/increments > (health_-damage)/increments && (health_-damage)/increments >=1)
        {
            qobject_cast<Game*>(scene())->playClicksound();
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
            matrixPath.remove(0,point_index_-1 < 0 ? 0 : point_index_-1);
            RuntimeError* minion = new RuntimeError(RuntimeErrorType::MemoryStackMinion,path, matrixPath);
            minion->setSpeed(speed_*2>70 ? 70 : speed_*2);
            emit addedEnemy((Enemy*)minion);
        }

    }
    qobject_cast<Game*>(scene())->playHitsound();
    health_-=damage;
    if(health_<=0)
    {
        die();
    }
}

