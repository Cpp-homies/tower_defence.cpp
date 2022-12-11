/**
 * @file compilererror.cpp
 * @author Siim Kasela (siim.kasela@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "compilererror.h"
#include <QGraphicsScene>
/**
 * @brief Construct a new Compiler Error:: Compiler Error object
 * Checks the subType of the enemy and sets the member values accordingly.
 * 
 * @param subType 
 * @param path 
 * @param matrixPath 
 */
CompilerError::CompilerError(CompilerErrorType subType, QList<QPointF> path,QList<QPoint> matrixPath): Enemy(EnemyType::CompilerError,path, matrixPath), name_(subType)
{
    switch (subType) {
        case CompilerErrorType::SyntaxError:
            setPixmap(QPixmap(":/images/syntax_error1.png"));
            setOffset(QPointF(16,16));
            setTransformOriginPoint(pixmap().height(), pixmap().width());
            damage_ = 1;
            health_ = 1;
            speed_ = 20;
            break;

        case CompilerErrorType::Exception:
            setPixmap(QPixmap(":/images/Exception.png"));
            setOffset(QPointF(16,16));
            setTransformOriginPoint(pixmap().height(), pixmap().width());
            damage_ = 10;
            health_ = 15;
            speed_ = 5;            
            break;

        default:
            deleteLater();
            break;
    }
    pointValue_ = health_;
}
/**
 * @brief After being killed calls the explodeException() function.
 * 
 */
void CompilerError::die()
{
    if(name_==CompilerErrorType::Exception)
    {
        explodeException();
    }
    emit enemyDies(pointValue_);
    deleteLater();

}
/**
 * @brief Spawns the amount of SyntaxError enemies on the location of the enemy and sets different speeds to each of them. 
 * 
 */
void CompilerError::explodeException()
{

    int amount = 3;

    for (int i = 1; i < amount+1; ++i)
    {
        QList<QPointF> path(path_);
        path.remove(0,point_index_);
        path.push_front(scenePos());
        QList<QPoint> matrixPath(matrixPath_);
        matrixPath.remove(0,point_index_-1 < 0 ? 0 : point_index_-1);
        CompilerError* enemy = new CompilerError(CompilerErrorType::SyntaxError,path,matrixPath);
        enemy->setSpeed(speed_+i*3 > 70 ? 70 : speed_+i*3);
        emit addedEnemy((Enemy*)enemy);
    }
}


