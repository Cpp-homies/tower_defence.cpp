#include "ta.h"

// TA tower upgrade costs can be set here
#define LVL2_COST 500
// Stats can be set here
#define RANGE 4
#define DAMAGE 2
#define ATTACK_INTERVAL 1500 // ms
#define PIERCE  3
#define SPEED_BUFF 1.05
#define DAMAGE_BUFF 1.1

//Tower(int x, int y, int range, int damage, int attackSpeed, QWidget *parent=nullptr);
TA::TA(int row, int column, QWidget *parent)
    : Tower(row, column, RANGE, DAMAGE, ATTACK_INTERVAL, parent) {
    // set TA stats
    atkSpeedBuffFactor_ = SPEED_BUFF;
    damageBuffFactor_ = 1.5;
    upgradeLevel_ = 1;
    maxLevel_ = 2;
    buffPulseInterval_ = 2000;
    pierce_ = PIERCE;

    // TA can target memory errors
    targetAble_[EnemyTypes::MemoryError] = true;

    // set TA graphics
    ogImagePath_ = ":/images/TA.png";
    projectileImagePath_ = ":/images/Ta_projectile.png";
    this->towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    this->towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );

    // get a list of towers in range
    QList<Tower*> towers = getTowersInRange();

    // buff all towers and mark them as buffed
    for (Tower* tower : towers) {
        // buff the tower if it has not been buffed yet
        if (!tower->hasAtkSpdBuff()) {
            // buff the tower
            tower->atkSpeedBuff(atkSpeedBuffFactor_);

            // add the buffed tower's location to the list to marked as buffed
            buffedTowers.prepend(tower->getCoords());
        }
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

                // increase projectile traveldistance
                maxRangeMultiplier_ = 3;

                // increase damage
                damage_ = 5;

                // increase pierce
                pierce_ = 10;

                // increase buffs
                damageBuffFactor_ = 1.5;
                atkSpeedBuffFactor_ = 1.1;

                // Teacher can target runtime errors
                targetAble_[EnemyTypes::RuntimeError] = true;

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
        if (!tower->hasAtkSpdBuff() && !buffedTowers.contains(tower->getCoords())) {
            // buff the tower
            tower->atkSpeedBuff(atkSpeedBuffFactor_);

            // add the buffed tower's location to the list to marked as buffed
            buffedTowers.prepend(tower->getCoords());
        }
    }
}

//Fires a projectile at the targetPos
void TA::fire(QPointF targetPos) {

    Projectile* projectile = new Projectile(damage_, projectileImagePath_, pierce_, projectileStepSize_);
    projectile->setPos(view->getGame()->getSquarePos(y_,x_)); //takes the same coordinates as the tower
    QLineF ln(view->getGame()->getSquarePos(y_,x_),targetPos); //path of the projectile
    int angle = -1 * ln.angle(); //the angle from tower to target
    int maxTowerRange = ceil(this->attack_area_->boundingRect().width() * range_ /2);
    projectile->setMaxRange(maxTowerRange * maxRangeMultiplier_);// set max range of the projectile to the range of the tower

    //set the projectile image to rotate around it's centre and then add it to the scene
    projectile->setTransformOriginPoint(projectile->pixmap().width()/2,projectile->pixmap().height()/2);
    projectile->setRotation(angle);
    view->getGame()->addItem(projectile);

    // if the angle towards the enemy changes,
    // undo the previous rotation and update to the new rotation angle
    if (this->rotationAngle_ != angle) {
        // the original image rotate by the new angle
        QTransform transform;
        transform.rotate(angle);
        towerImg->setTransformOriginPoint(QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2));
        //towerImg->setTransform(transform);
        towerImg->setRotation(angle);
        //Square::setPixmap(QPixmap(ogImagePath_).transformed(transform).scaled(pixmap().size(), Qt::KeepAspectRatioByExpanding));
        rotationAngle_ = angle;// update the rotation angle
    }


}
