/**
 * @file projectile.cpp
 * @authors Siim (siim.kasela@aalto.fi), Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "projectile.h"
#include "enemy.h"
#include <QPixmap>
#include <QTimer>
#include <qmath.h>

/**
 * @brief Construct a new Projectile:: Projectile object. Initializes timers to cause moving.
 * 
 * @param damage Damage that the projectile causes on enemies it hits
 * @param imgPath Path to the image file
 * @param pierce Number of enemies the projectile can damage
 * @param stepSize Size of the step the projectile will make when it moves
 * @param maxLifeTime Maximum time the projectile can exist
 * @param parent The parent of the projectile.
 */
Projectile::Projectile(int damage, QString imgPath,
                       int pierce, int stepSize,
                       int maxLifeTime, QGraphicsItem *parent)
    : QObject(),QGraphicsPixmapItem(parent), damage_(damage),
    pierce_(pierce), stepSize_(stepSize), maxLifetime_(maxLifeTime)
{
    setPixmap(QPixmap(imgPath));
    setTransformOriginPoint(pixmap().width()/2,pixmap().height()/2);
    QTimer * move_timer = new QTimer(this);
    connect(move_timer,SIGNAL(timeout()),this,SLOT(move()));
    move_timer->start(moveInterval_);
    if (maxLifetime_ < 100000){
        lifeTimer_ = new QTimer(this);
        lifeTimer_->singleShot(maxLifetime_, [this](){this->deleteLater();});
    }

    maxRange_ = 200;
    distanceTravelled_ = 0;
    pierceCount_ = 0;
}

/**
 * @brief Moves the projectiles forward the stepSize amount. Deletes the projectile when it has traveled far enough. Damages colliding enemies.
 * 
 */
void Projectile::move(){
    double theta = rotation(); // degrees

    double dy = stepSize_ * qSin(qDegreesToRadians(theta));
    double dx = stepSize_ * qCos(qDegreesToRadians(theta));

    setPos(x()+dx, y()+dy);
    for (auto item : collidingItems())
    {
        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if(enemy)
        {
            if (!enemiesHit_.contains(enemy)){
                enemy->takeDamage(damage_);
                enemiesHit_.append(enemy);
                pierceCount_++;
            }
            if (pierceCount_ >= pierce_){
                if (maxLifetime_ >= 100000){
                    deleteLater();
                }
            }
            // update distance travel here as well
            setDistanceTravelled(distanceTravelled_+stepSize_);
            if(distanceTravelled_>maxRange_) deleteLater();
            return;
        }

    }


    setDistanceTravelled(distanceTravelled_+stepSize_);
    if(distanceTravelled_>maxRange_) deleteLater();
}

/**
 * @brief Returns the maxRange of the projectile.
 * 
 * @return the maxRange of the projectile.
 */
double Projectile::getMaxRange(){
    return maxRange_;
}

/**
 * @brief Returns the distance which the projectile has traveled.
 * 
 * @return the distance which the projectile has traveled.
 */
double Projectile::getDistanceTravelled(){
    return distanceTravelled_;
}

/**
 * @brief Sets the maxRange_ variable.
 * 
 * @param range New value for the range.
 */
void Projectile::setMaxRange(double range){
    maxRange_ = range;
}

/**
 * @brief Sets the variable distanceTravelled_.
 * 
 * @param distance New value for the distance.
 */
void Projectile::setDistanceTravelled(double distance){
    distanceTravelled_ = distance;
}
