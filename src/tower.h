/**
 * @file tower.h
 * @authors Hung Vu (hung.h.vu@aalto.fi), Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TOWER_H
#define TOWER_H

#include "square.h"
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

/**
 * @brief Base class for all towers in the game, inherited from Square.
 */
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
    void targetTableBuff(EnemyTypes::TYPES type);

    void atkSpeedDebuff(double debuffFactor);
    void targetTableDebuff(EnemyTypes::TYPES type);
    bool hasAtkSpdBuff();

    QGraphicsEllipseItem* getAttackArea();
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
    virtual void updateDescription();

    int projectileStepSize_ = 6;// how fast the tower's projectile move
public slots:
    void getTarget();
protected:
    QString type_;//!< the type of the tower
    int range_;//!< the attack range of the tower
    double damage_;//!< the damage of the tower
    double damageMultiplier_;//!< the damage multiplier of the tower, can be use for buff mechanic
    double attackInterval_;//!< the interval between the tower's shot, in mili-seconds
    QTimer* attackTimer_;//!< timer for timing the tower's attack
    int pierce_;//!< how many enemies (after the first one) can the projectile of this tower pierce through
    int maxLevel_;//!< the max upgrade level of the tower
    int upgradeLevel_;// the tower's current upgrade level
    int totalCost_;//!< the tower current total value
    int rotationAngle_;//!< the rotation angle of the tower

    QGraphicsPixmapItem * towerImg;//!< the tower's image in use
    QString ogImagePath_;//!< the original image path in use
    QString projectileImagePath_;//!< the tower's projectile image
    QGraphicsEllipseItem* attack_area_;//!< the physical attack area of the tower, represent by an ellipse item
    QString description_;

    bool canFire_;//!< true if the tower can fire, false otherwise (ex: support towers)
    bool targetAble_[3];//!< array for checking if an enemy is targetable, an enemy is targetable if the coresponding position for that enemy is true
    bool targetAbleBuff_[3];//!< array for checking if this tower has been allowed to target an enemy by a support tower, logic to targetAble but is used for buffs
private:
    bool upgradeable_;//!< check if the tower is upgradeable or not

    bool has_target_;//!< check if the tower has a target or not
    bool hasAtkSpdBuff_;//!< check if the tower has received an attack speed buff from any of the support towers

    QPointF target_pos_;//!< location of the tower's current target
};



#endif // TOWER_H
