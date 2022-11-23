#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

class Projectile : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Projectile(QGraphicsItem * parent=0);
    double getMaxRange();
    double getDistanceTravelled();
    void setMaxRange(double rng);
    void setDistanceTravelled(double dist);

public slots:
    void move();

private:
    double maxRange_;
    double distanceTravelled_;

};

#endif // PROJECTILE_H
