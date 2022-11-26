#include "projectile.h"
#include <QPixmap>
#include <QTimer>
#include <qmath.h>


Projectile::Projectile(QGraphicsItem *parent)
    : QObject(),QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/images/CStudent_projectile.png"));
    setTransformOriginPoint(pixmap().width()/2,pixmap().height()/2);
    QTimer * move_timer = new QTimer(this);
    connect(move_timer,SIGNAL(timeout()),this,SLOT(move()));
    move_timer->start(10);

    maxRange_ = 200;
    distanceTravelled_ = 0;
}

void Projectile::move(){
    int STEP_SIZE = 6;
    double theta = rotation(); // degrees

    double dy = STEP_SIZE * qSin(qDegreesToRadians(theta));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(theta));

    setPos(x()+dx, y()+dy);
    setDistanceTravelled(distanceTravelled_+STEP_SIZE);
    if(distanceTravelled_>maxRange_) deleteLater();
}

double Projectile::getMaxRange(){
    return maxRange_;
}

double Projectile::getDistanceTravelled(){
    return distanceTravelled_;
}

void Projectile::setMaxRange(double range){
    maxRange_ = range;
}

void Projectile::setDistanceTravelled(double distance){
    distanceTravelled_ = distance;
}
