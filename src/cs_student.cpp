#include "cs_student.h"
#include "tower.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPointF>
#include <QObject>
#include <QList>
#include "mainview.h"
#include <QTimer>

// CS tower upgrade costs can be set here
#define LVL2_COST 500
#define LVL4_COST 500
#define LVL3_COST 500

/**
 * @brief CS_Student constructor, create a CS Student tower at the given posion on the grid map.
 * @param row the row of the tower in the game's grid map
 * @param column the column of the tower in the game's grid map
 * @param parent the parent of this item
 */
CS_Student::CS_Student(int row, int column, QWidget *parent) : Tower(row, column, parent) {

}

/**
 * @brief Upgrade this tower if possible.
 * @return true if the upgrade was successful, false otherwise.
 */
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

                // year 2 CStudent can target memory errors
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
        // update the tower's description
        this->updateDescription();

        return true;
    }
}
