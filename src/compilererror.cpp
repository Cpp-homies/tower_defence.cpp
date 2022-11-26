#include "compilererror.h"

CompilerError::CompilerError(EnemyName subType, QList<QPointF> path, Game& game): Enemy(path,game)
{
    switch (subType) {
        case EnemyName::SyntaxError:
            setPixmap(QPixmap(":/images/syntax_error3.png"));
            setOffset(QPointF(16,16)); //TODO set bosses offset to 0
            setTransformOriginPoint(pixmap().height(), pixmap().width()); //TODO change bosses originpoints with /2
            damage_ = 1;
            health_ = 1;
            speed_ = 10;
            name_ = EnemyName::SyntaxError;
            break;
        case EnemyName::Exception:
            setPixmap(QPixmap(":/images/syntax_error3.png"));
            setOffset(QPointF(16,16)); //TODO set bosses offset to 0
            setTransformOriginPoint(pixmap().height(), pixmap().width()); //TODO change bosses originpoints with /2
            damage_ = 1;
            health_ = 1;
            speed_ = 5;
            name_ = EnemyName::Exception;
            break;
        default:
            break;
    }
}

void CompilerError::die()
{
    game_.changeScore(pointValue_);
    game_.changeCurrency(pointValue_);
    if(name_==EnemyName::Exception)
    {
        explodeException();
    }
    deleteLater();

}

void CompilerError::explodeException()
{
    for (int i = 1; i < 11; ++i)
    {
        QList<QPointF> path(path_);
        path.remove(0,point_index_);
        path.push_front(scenePos());
        CompilerError* enemy = new CompilerError(EnemyName::SyntaxError,path,  game_);
        //TODO enemy.setPixmap();
        enemy->setSpeed(speed_*i);
        enemy->startMove();
        game_.addItem(enemy);
    }
}


