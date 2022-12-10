#include "ta.h"

// TA tower upgrade costs can be set here
#define LVL2_COST 500

/**
*
* TODO : FIX THE BUFF FEATURE, THE GraphicsItem is not a base class of tower
* so it cannot be casted to that
*
*/

//Tower(int x, int y, int range, int damage, int attackSpeed, QWidget *parent=nullptr);
TA::TA(int row, int column, QWidget *parent) : Tower(row, column, 4, 10, 3000, parent) {
    // set TA stats
    atkSpeedBuffFactor_ = 5;// set the BuffFactor extremely high for testing
//    atkSpeedBuffFactor_ = 0.2;
    upgradeLevel_ = 1;
    maxLevel_ = 2;
    buffPulseInterval_ = 2000;

    // set TA graphics
    ogImagePath_ = ":/images/TA.png";
    projectileImagePath_ = ":/images/Ta_projectile.png";
    this->towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    this->towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );

    // get a list of towers in range
    QList<Tower*> towers = getTowersInRange();

    // buff all towers and mark them as buffed
    for (Tower* tower : towers) {
        // buff the tower
        tower->atkSpeedBuff(atkSpeedBuffFactor_);

        // add the buffed tower's location to the list to marked as buffed
        buffedTowers.prepend(tower->getCoords());
    }

    // add the TA tower into the buffedTowers list so it doesn't buff itself
    buffedTowers.prepend(this->getCoords());

    // connect the buffPulse() to the buffPulseInterval
    buffTimer_ = new QTimer(this);
    connect(buffTimer_,SIGNAL(timeout()),this,SLOT(buffPulse()));
    buffTimer_->start(buffPulseInterval_);
}

TA::~TA() {
    if (view->getGame()->getMode() != Modes::exit) {
        // remove the buff from all buffed towers
        for (QPointF point : buffedTowers) {
            QWidget* widget = view->getGame()->getWidgetAt(point.y(), point.x());
            Tower* tower = dynamic_cast<Tower*>(widget);
            if (tower) {
                // debuff the tower
                tower->atkSpeedDebuff(atkSpeedBuffFactor_);
            }
        }
    }

    // schedule to delete the buff timer
    buffTimer_->deleteLater();
}

bool TA::upgrade() {
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
                // increase range
                setRange(6);

                // update tower graphics
                projectileImagePath_ = ":/images/Teacher_projectile.png";
                ogImagePath_ = ":/images/Teacher.png";
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
        }

        return true;
    }
}

// function for periodically check for new towers in range and buff them
void TA::buffPulse() {
    // get all the nearby towers in range
    QList<Tower*> towers = getTowersInRange();

    // loop through all towers in range
    for (Tower* tower : towers) {
        // buff the tower if it hasn't got buff before
        if (!buffedTowers.contains(tower->getCoords())) {
            // buff the tower
            tower->atkSpeedBuff(atkSpeedBuffFactor_);

            // add the buffed tower's location to the list to marked as buffed
            buffedTowers.prepend(tower->getCoords());
        }
    }
}
