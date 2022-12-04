#include "cs_student.h"
#include "tower.h"
#include "projectile.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPointF>
#include <QObject>
#include <QList>
#include "mainview.h"
#include "enemy.h"
#include <QTimer>

// CS tower upgrade costs can be set here
#define LVL2_COST 10
#define LVL3_COST 15
#define LVL4_COST 25

CS_Student::CS_Student(int row, int column, QWidget *parent) : Tower(row, column, parent) {

}

bool CS_Student::upgrade() {
    if (upgradeLevel_ >= maxLevel_) {
        // already max level
        return false;
    }
    else {
        // upgrade the tower according to its level
        upgradeLevel_ += 1;

        switch (upgradeLevel_) {
        case 2:
            // if the player has enough money for the upgrade
            // upgrade the tower
            if (view->getGame()->getCurrency() >= LVL2_COST) {
                // increase damage by 20%
                this->damage_ = this->damage_ * 1.2;
                targetAble_[EnemyTypes::MemoryError] = true;

                // increase range to 4
                setRange(4);

                // increase attack speed by 20% (attackInterval should be at 800 now)
                atkSpeedBuff(0.2);

                // update tower graphics
                projectileImagePath_ = ":/images/CStudent2_projectile.png";
                ogImagePath_ = ":/images/CStudent2.png";
                towerImg->setPixmap(QPixmap(ogImagePath_));

                // deduct the cost of the tower from player's money
                view->getGame()->changeCurrency(-LVL2_COST);

                // add the cost of the upgrade to tower's total cost
                addCost(LVL2_COST);
            }
            else {
                // upgrade failed
                return false;
            }

            break;
        case 3:
            // if the player has enough money for the upgrade
            // upgrade the tower
            if (view->getGame()->getCurrency() >= LVL3_COST) {
                // increase damage by 50%
                this->damage_ = this->damage_ * 1.5;

                // can now target bosses
                targetAble_[EnemyTypes::CompilerError] = true;

                // increase range to 5
               setRange(5);

                // increase attack speed by 30% (attackInterval should be at around 615 now)
                atkSpeedBuff(0.3);

                // update tower graphics
                projectileImagePath_ = ":/images/CStudent3_projectile.png";
                ogImagePath_ = ":/images/CStudent3.png";
                towerImg->setPixmap(QPixmap(ogImagePath_));

                // deduct the cost of the tower from player's money
                view->getGame()->changeCurrency(-LVL3_COST);

                // add the cost of the upgrade to tower's total cost
                addCost(LVL3_COST);
            }
            else {
                // upgrade failed
                return false;
            }

            break;
        case 4:
            if (view->getGame()->getCurrency() >= LVL4_COST) {
                // increase damage by 50%
                this->damage_ = this->damage_ * 1.5;

                // increase range to 7
                setRange(7);

                // increase attack speed by 100% (attackInterval should be at around 307 now)
                atkSpeedBuff(1);

                //increase pierce
                pierce_ = 2;

                // update tower graphics
                projectileImagePath_ = ":/images/CStudent4_projectile.png";
                ogImagePath_ = ":/images/CStudent4.png";
                towerImg->setPixmap(QPixmap(ogImagePath_));

                // deduct the cost of the tower from player's money
                view->getGame()->changeCurrency(-LVL4_COST);

                // add the cost of the upgrade to tower's total cost
                addCost(LVL4_COST);
            }
            else {
                // upgrade failed
                return false;
            }


            break;
        }

        return true;
    }
}
