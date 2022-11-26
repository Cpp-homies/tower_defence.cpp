#include "enemy.h"
#include "mainview.h"

#include <QTimer>

Enemy::Enemy(int health, int damage, int speed, QList<QPointF> path,Game& game, QGraphicsItem * parent):
     QGraphicsPixmapItem(parent), health_(health), damage_(damage), speed_(speed), game_(game), path_(path)
{
    setPixmap(QPixmap(":/images/syntax_error3.png"));
    setOffset(QPointF(16,16));
    setTransformOriginPoint(pixmap().height(), pixmap().width());
    setPos(path_[0]);
    point_index_ = 0;
    dest_ = path_[0];

    QTimer * timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(50);
}

void Enemy::die()
{
    deleteLater();
}

void Enemy::move()
{
    QLineF ln(pos(),dest_);
    if (ln.length() == 0)
    {
        point_index_++;
        // last point reached
        if (point_index_ >= path_.size()){
            game_.changeHealth(-damage_);
            if(game_.isLost()) //qobject_cast<MainView*>(game_.parent())->showLeaderboard();
            return;
        }
        // last point not reached
        dest_ = path_[point_index_];
        ln = QLineF (pos(),dest_);
        setRotation(-1 * ln.angle());


    }

    // move enemy forward at current angle
    int STEP_SIZE = 2;
    double theta = rotation(); // degrees

    double dy = STEP_SIZE * qSin(qDegreesToRadians(theta));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(theta));

    setPos(x()+dx, y()+dy);
}

