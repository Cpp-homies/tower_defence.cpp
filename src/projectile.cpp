#include "projectile.h"
#include <QPixmap>
#include <QTimer>
#include <qmath.h>

/**
 * @brief Projectile contructor
 * @param parent The parent object of the projectile.
 */
Projectile::Projectile(QGraphicsItem *parent)
    : QObject(),QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/images/CStudent_projectile.png"));

    QTimer * move_timer = new QTimer(this);
    connect(move_timer,SIGNAL(timeout()),this,SLOT(move()));
    move_timer->start(10);

    maxRange_ = 200;
    distanceTravelled_ = 0;
}
/**
 * @brief Allow the projectile to continue moving.
 */
void Projectile::move(){
    int STEP_SIZE = 6;
    double theta = rotation(); // degrees

    double dy = STEP_SIZE * qSin(qDegreesToRadians(theta));
    double dx = STEP_SIZE * qCos(qDegreesToRadians(theta));

    setPos(x()+dx, y()+dy);
    setDistanceTravelled(distanceTravelled_+STEP_SIZE);
    if(distanceTravelled_>maxRange_) deleteLater();
}
/**
 * @brief Gets the max range of the projectile.
 * @return The max range of the projectile.
 */
double Projectile::getMaxRange(){
    return maxRange_;
}
/**
 * @brief Gets the distanced travelled so far by the projectile.
 * @return The distance travelled so far by the projectile.
 */
double Projectile::getDistanceTravelled(){
    return distanceTravelled_;
}
/**
 * @brief Sets the max range of the projectile.
 * @param range The number to set as the max range of the projectile.
 */
void Projectile::setMaxRange(double range){
    maxRange_ = range;
}
/**
 * @brief Sets the distance travelled of the projectile.
 * @param distance The number to set as the distance travelled by the projectile so far.
 */
void Projectile::setDistanceTravelled(double distance){
    distanceTravelled_ = distance;
}
