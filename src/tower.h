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
#include <QTimer>
#include "mainview.h"
#include "enemy.h"

// enum to act as shorthand that denotes enemy types
namespace EnemyTypes{
    enum TYPES{CompilerError=0, MemoryError=1, RuntimeError=2};
}

extern MainView* view;

class Tower : public Square {
public:
    Tower(QWidget *parent = nullptr);
    Tower(int row, int column, QWidget *parent=nullptr);
    Tower(int row, int column, int range, int damage, int attackSpeed, QWidget *parent=nullptr);

    ~Tower();

    QPointF towerCenter();
    double distanceTo(QGraphicsItem * item);
    virtual void fire(QPointF targetPos);
    bool isTargetable(Enemy* enemy);
    void damageBuff(double buffFactor);
    void atkSpeedBuff(double buffFactor);

    void atkSpeedDebuff(double debuffFactor);

    QList<QGraphicsItem*> getItemInRange();
    QList<Tower*> getTowersInRange();
    int getTotalCost();
    void setRange(int range);
    void addCost(int cost);

    virtual bool upgrade() = 0;
    virtual bool isTower();
    void showAttackArea();
    void hideAttackArea();
    virtual void showHideAttackArea();
public slots:
    void getTarget();
protected:
    int range_;
    double damage_;
    double damageMultiplier_;
    double attackInterval_;// in mili-seconds
    QTimer* attackTimer_;
    int pierce_;
    int maxLevel_;
    int upgradeLevel_;
    int totalCost_;
    int rotationAngle_;

    QGraphicsPixmapItem * towerImg;
    QString ogImagePath_;// the original image path in use
    QString projectileImagePath_;

    bool targetAble_[3];// array for checking if an enemy is targetable
private:
    bool upgradeable_;

    bool has_target_;

    QGraphicsEllipseItem* attack_area_;
    QPointF target_pos_;
};



#endif // TOWER_H
