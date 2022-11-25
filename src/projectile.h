#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
/**
 * @brief The Projectile class. Projectiles are the objects shot by towers.
 */
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
    /**
     * @brief The maxiumum range the projectile can travel.
     */
    double maxRange_;
    /**
     * @brief The distance travelled by the projectile.
     */
    double distanceTravelled_;

};

#endif // PROJECTILE_H
