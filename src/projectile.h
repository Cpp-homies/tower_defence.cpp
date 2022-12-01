#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "enemy.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QList>

class Projectile : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Projectile(int damage, QString imgPath = ":/images/CStudent_projectile.png",
               int pierce = 0, QGraphicsItem * parent=0);
    double getMaxRange();
    double getDistanceTravelled();
    void setMaxRange(double rng);
    void setDistanceTravelled(double dist);

public slots:
    void move();

private:
    double maxRange_;
    double distanceTravelled_;
    int damage_;
    int pierce_;
    int pierceCount_;
    QList<Enemy*> enemiesHit_;

};

#endif // PROJECTILE_H
