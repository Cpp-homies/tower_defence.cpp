#include "compilererror.h"

CompilerError::CompilerError(CompilerErrorType subType, QList<QPointF> path, Game& game): Enemy(EnemyType::CompilerError,path,game), name_(subType)
{
    switch (subType) {
        case CompilerErrorType::SyntaxError:
            setPixmap(QPixmap(":/images/syntax_error1.png"));
            setOffset(QPointF(16,16));
            setTransformOriginPoint(pixmap().height(), pixmap().width());
            damage_ = 1;
            health_ = 1;
            speed_ = 10;            
            break;

        case CompilerErrorType::Exception:
            setPixmap(QPixmap(":/images/Exception.png"));
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
    pointValue_ = health_;
}

void CompilerError::die()
{
    game_.changeScore(pointValue_);
    game_.changeCurrency(pointValue_);
    if(name_==CompilerErrorType::Exception)
    {
        explodeException();
    }
    deleteLater();
    game_.enemyDies();
}

void CompilerError::explodeException()
{
    int amount = 10;
    for (int i = 1; i < amount+1; ++i)
    {
        QList<QPointF> path(path_);
        path.remove(0,point_index_);
        path.push_front(scenePos());
        CompilerError* enemy = new CompilerError(CompilerErrorType::SyntaxError,path,  game_);
        enemy->setSpeed(speed_+i*3);
        enemy->startMove();
        game_.addItem(enemy);
    }
}


