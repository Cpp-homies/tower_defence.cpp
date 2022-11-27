#ifndef TOWER_H
#define TOWER_H

#include "square.h"
#include "game.h"
#include "projectile.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPointF>
#include <QObject>
#include <QList>
#include "mainview.h"
#include "enemy.h"

// enum to act as shorthand that denotes enemy types
namespace EnemyTypes{
    enum TYPES{normal=0, memory=1, boss=2};
}

extern MainView* view;

class Tower : public Square {
public:
    Tower(QWidget *parent = nullptr);
    Tower(int x, int y, QWidget *parent);
    Tower(int x, int y, QWidget *parent, int range, int damage, int attackSpeed);

    QPointF towerCenter();
    double distanceTo(QGraphicsItem * item);
    virtual void fire(QPointF targetPos);
    bool isTargetable(Enemy* enemy);

    virtual bool upgrade();
public slots:
    void getTarget();
private:
    int range_ = 5;
    int damage_;
    int attackInterval_;// in mili-seconds
    double damageMultiplier_;

    bool upgradeable_;
    int upgradeLevel_;
    int maxLevel_;

    int totalCost_;
    bool has_target_;
    int rotationAngle_ = 0;

    QGraphicsEllipseItem* attack_area_;
    QPointF target_pos_;
    QString ogImagePath_;// the original imagepath in use
    bool targetAble_[3];// array for checking if an enemy is targetable
};



#endif // TOWER_H
