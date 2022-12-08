#include "language_server.h"
#include "projectile.h"
#include "cmath"
#include <QPropertyAnimation>

#define LVL2_COST 50
#define LVL3_COST 100

Language_Server::Language_Server(int row, int column, QWidget *parent) : Tower(row, column, 3, 1, 1000, parent) {
    // set Language_Server stats
    upgradeLevel_ = 1;
    maxLevel_ = 3;

    // set high piercing
    pierce_ = 1000;
    // set Language Server graphics
    ogImagePath_ = ":/images/Language_server.png";
    projectileImagePath_ = ":/images/Language_server_projectile.png";
    this->towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    this->towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );
}

void Language_Server::fire(QPointF /* not used */) {
    // create a proxy projectile to use as shooting visual effect
    Projectile* projectile = new Projectile(damage_, projectileImagePath_, pierce_, 0, 200);
    projectile->setPos(this->pos()-QPointF(32,32));
    projectile->setScale((this->range_/2+0.5)/4);
    QTimer animationStage1;
    animationStage1.singleShot(50, [this, projectile](){projectile->setScale((this->range_/2+0.5)/2);});
    QTimer animationStage2;
    animationStage2.singleShot(100, [this, projectile](){projectile->setScale((this->range_/2+0.5)*3/4);});
    QTimer animationStage3;
    animationStage3.singleShot(150, [this, projectile](){projectile->setScale(this->range_/2+0.5);});


    view->getGame()->addItem(projectile);
}

bool Language_Server::upgrade() {
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
                // increase damage by 100%
                damageBuff(1.0);

                // setRange (to make attack area visible after upgrade)
                setRange(3);

                // update tower graphics
                projectileImagePath_ = ":/images/Tree_sitter_projectile.png";
                ogImagePath_ = ":/images/Tree_sitter.png";
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
            if (view->getGame()->getCurrency() >= LVL2_COST) {
                // increase attack speed by 200%
                atkSpeedBuff(2.0);

                // increase range to 4
                setRange(4);

                // can now target memory errors
                targetAble_[EnemyTypes::MemoryError] = true;

                // update tower graphics
                projectileImagePath_ = ":/images/Copilot_projectile.png";
                ogImagePath_ = ":/images/Copilot.png";
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
        }

        return true;
    }
}
