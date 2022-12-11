/**
 * @file projectile.h
 * @authors Siim (siim.kasela@aalto.fi), Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "enemy.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QList>

/**
 * @brief This is the projectile object.
 * 
 */
class Projectile : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Projectile(int damage, QString imgPath = ":/images/CStudent_projectile.png",
               int pierce = 0, int stepSize = 6, int maxLifeTime = 100000, QGraphicsItem * parent=0);
    double getMaxRange();
    double getDistanceTravelled();
    void setMaxRange(double rng);
    void setDistanceTravelled(double dist);
    void setMoveFrequency(int newValue) {moveInterval_=newValue;};

public slots:
    void move();

private:
    /**
     * @brief Maximum distance the projectile can travel.
     * 
     */
    double maxRange_;
    /**
     * @brief Distance the projectile has traveled.
     * 
     */
    double distanceTravelled_;
    /**
     * @brief Damage that the projectile will make to colliding enemies.
     * 
     */
    int damage_;
    /**
     * @brief Number of enemies the projectile can damage.
     * 
     */
    int pierce_;
    /**
     * @brief Number of enemies the projectile has damaged.
     * 
     */
    int pierceCount_;
    /**
     * @brief List of enemies the projectile has damaged.
     * 
     */
    QList<Enemy*> enemiesHit_;
    /**
     * @brief Size of the step the projectile will make when it moves.
     * 
     */
    int stepSize_;
    /**
     * @brief Maximum time the projectile can exist.
     * 
     */
    int maxLifetime_;
    /**
     * @brief Keeps track of the time the projectile has existed.
     * 
     */
    QTimer* lifeTimer_;
    /**
     * @brief Time in milliseconds between every move of the projectile.
     * 
     */
    int moveInterval_ = 10;
};

#endif // PROJECTILE_H
