#include "tower.h"
#include "enemy.h"
#include <QPointF>
#include <QPen>
#include <QTransform>
#include <QLocale>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QTimer>
#include <typeinfo>
#include <iostream>
#include "mainview.h"
#include <QGraphicsPixmapItem>
#include <cmath>
#include "projectile.h"

extern MainView * view;

const qreal atkArea_LineWidth = 0.3;
const Qt::PenStyle atkArea_LineStyle = Qt::DashLine;
const Qt::GlobalColor atkArea_Color = Qt::black;


//Tower::Tower(QWidget *parent) : Square(parent) {

//}

/**
 * @brief Tower basic constructor, build a default tower (CS Student) at the given position.
 * @param row the row of the tower in the game's grid map
 * @param column the column of the tower in the game's grid map
 * @param parent the parent of this item
 */
Tower::Tower(int row, int column, QWidget *parent) : Square(column, row, parent) {
    // set the default tower stats
    range_ = 3;
    damage_ = 5;
    attackInterval_ = 1500;
    damageMultiplier_ = 1.0;
    pierce_ = 0;
    canFire_ = true;

    totalCost_ = 0;

    upgradeLevel_ = 1;
    maxLevel_ = 4;

    description_ = "";

    // has no attack speed buff at creation
    hasAtkSpdBuff_ = false;

    // initialize the targetable enemies, at first the tower can only target normal enemies
    std::fill_n(targetAble_, std::size(targetAble_), false);
    targetAble_[EnemyTypes::CompilerError] = true;

    // initialize the targetable buff list to all false
    std::fill_n(targetAbleBuff_, std::size(targetAbleBuff_), false);

    // set tower graphics
    ogImagePath_ = ":/images/CStudent1.png";
    projectileImagePath_ = ":/images/CStudent_projectile.png";
    towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );

    // create the attack circle
    attack_area_ = new QGraphicsEllipseItem(QRect(QPoint(0,0),
                                                 QSize(this->pixmap().width(),
                                                 this->pixmap().height())));
    attack_area_->setPen(QPen(QBrush(atkArea_Color), atkArea_LineWidth, atkArea_LineStyle));
    view->getGame()->addItem(attack_area_);

    attack_area_->setScale(this->range_);


    // move the attack area to the right possition
    QPointF area_center(0 + attack_area_->rect().width() / 2, 0 + attack_area_->rect().height() / 2);
    area_center *= this->range_;
    QLineF ln(area_center,towerCenter());
    attack_area_->setPos(0+ln.dx(),0+ln.dy());

    // connect the timer to the getTarget function
    attackTimer_ = new QTimer(this);
    connect(attackTimer_,SIGNAL(timeout()),this,SLOT(getTarget()));
    attackTimer_->start(attackInterval_);
}

/**
 * @brief Tower specific constructor, initialize the member of the tower with given parameter.
 * @param row the row of the tower in the game's grid map
 * @param column the column of the tower in the game's grid map
 * @param range the attack range of the tower
 * @param damage the damage of the tower
 * @param attackInterval the attack interval of the tower, in mili-seconds
 * @param parent the parent of this item
 */
Tower::Tower(int row, int column, int range, int damage,
             int attackInterval, QWidget *parent)
    : Square(column, row, parent),range_(range), damage_(damage),
    attackInterval_(attackInterval) {
    totalCost_ = 0;
    canFire_ = true;

    // set the original damage multipier to 1.0
    damageMultiplier_ = 1.0;

    // has no attack speed buff at creation
    hasAtkSpdBuff_ = false;

    // initialize the targetable enemies, at first the tower can only target normal enemies
    std::fill_n(targetAble_, std::size(targetAble_), false);
    targetAble_[EnemyTypes::CompilerError] = true;

    // initialize the targetAble buff by other towers, at the start there is no buff
    std::fill_n(targetAbleBuff_, std::size(targetAbleBuff_), false);

    // create the attack circle
    attack_area_ = new QGraphicsEllipseItem(QRect(QPoint(0,0),
                                                 QSize(this->pixmap().width(),
                                                 this->pixmap().height())));
    attack_area_->setPen(QPen(QBrush(atkArea_Color), atkArea_LineWidth, atkArea_LineStyle));
    view->getGame()->addItem(attack_area_);

    attack_area_->setScale(this->range_);


    // move the attack area to the right possition
    QPointF area_center(0 + attack_area_->rect().width() / 2, 0 + attack_area_->rect().height() / 2);
    area_center *= this->range_;
    QLineF ln(area_center,towerCenter());
    attack_area_->setPos(0+ln.dx(),0+ln.dy());

    // connect the timer to the getTarget function
    attackTimer_ = new QTimer(this);
    connect(attackTimer_,SIGNAL(timeout()),this,SLOT(getTarget()));
    attackTimer_->start(attackInterval_);
}

/**
 * @brief Get a target for the tower to shoot at.
 * 
 */
void Tower::getTarget() {
    // get all the items in the tower's range
    QList <QGraphicsItem*> items_in_range = attack_area_->collidingItems();

    has_target_ = false;

    // initiallize min_dist with the largest possible distance in range (the max range of the tower)
    double min_dist = this->range_ * this->pixmap().width();
    QPointF min_point = QPointF(0,0);

    // find the closest enemy
    for (int i = 0, n = items_in_range.size(); i < n; i++) {
        Enemy * enemy = dynamic_cast<Enemy *>(items_in_range[i]);
        // if this is an enemy
        if (enemy) {
            // check if the enemy is targetable or not
            if (isTargetable(enemy)) {
                double cur_dist = distanceTo(enemy);
                if (cur_dist < min_dist){
                    min_dist = cur_dist;
                    min_point = enemy->scenePos();
                    has_target_ = true;
                }
            }
        }
    }

    if (has_target_) {
        this->target_pos_ = min_point;
        fire(target_pos_);
    }
}

/**
 * @brief Tower's destructor
 */
Tower::~Tower() {
   // remove the tower image from the scene and delete it
   view->getGame()->removeItem(towerImg);
   delete towerImg;

   // remove the tower attack area from the scene and delete it
   view->getGame()->removeItem(attack_area_);
   delete attack_area_;

   // remove the attackTimer
   attackTimer_->deleteLater();
}

/**
 * @brief Set a new range of the tower, which changes the attack area accordingly.
 * @param range the new range of the tower
 */
void Tower::setRange(int range) {
    this->range_ = range;

    // remove the old attack area
    view->getGame()->removeItem(attack_area_);
    delete attack_area_;

    // create the new attack circle
    attack_area_ = new QGraphicsEllipseItem(QRect(QPoint(0,0),
                                                 QSize(this->pixmap().width(),
                                                 this->pixmap().height())));
    attack_area_->setPen(QPen(QBrush(atkArea_Color), atkArea_LineWidth, atkArea_LineStyle));
    view->getGame()->addItem(attack_area_);

    attack_area_->setScale(this->range_);


    // move the attack area to the right possition
    QPointF area_center(0 + attack_area_->rect().width() / 2, 0 + attack_area_->rect().height() / 2);
    area_center *= this->range_;
    QLineF ln(area_center,towerCenter());
    attack_area_->setPos(0+ln.dx(),0+ln.dy());

}

/**
 * @brief Fire a projectile at the given target position.
 * @param targetPos the position of the target to fire at
 */
void Tower::fire(QPointF targetPos) {

    Projectile* projectile = new Projectile(damage_, projectileImagePath_, pierce_, projectileStepSize_);
    projectile->setPos(view->getGame()->getSquarePos(y_,x_)); //takes the same coordinates as the tower
    QLineF ln(view->getGame()->getSquarePos(y_,x_),targetPos); //path of the projectile
    int angle = -1 * ln.angle(); //the angle from tower to target
    int maxTowerRange = ceil(this->attack_area_->boundingRect().width() * range_ /2);
    projectile->setMaxRange(maxTowerRange);// set max range of the projectile to the range of the tower

    //set the projectile image to rotate around it's centre and then add it to the scene
    projectile->setTransformOriginPoint(projectile->pixmap().width()/2,projectile->pixmap().height()/2);
    projectile->setRotation(angle);
    view->getGame()->addItem(projectile);

    //rotate the square
//    QTransform transform;
//    transform.rotate(angle);
//    Square::setPixmap(Square::pixmap().transformed(transform));

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

/**
 * @brief Check if the tower can target the given enemy.
 * @param enemy pointer to an Enemy class object
 * @return true if the enemy can be target by this tower, false otherwise
 */
bool Tower::isTargetable(Enemy* enemy) {
    // check the enemy type
    int type = static_cast<int>(enemy->getType()) - 1;
    switch (type) {
        // check if current tower can target given type
        case EnemyTypes::RuntimeError:
        {
            return targetAble_[EnemyTypes::RuntimeError] || targetAbleBuff_[EnemyTypes::RuntimeError];
            break;
        }
        case EnemyTypes::MemoryError:
        {
            return targetAble_[EnemyTypes::MemoryError] || targetAbleBuff_[EnemyTypes::MemoryError];
            break;
        }
        case EnemyTypes::CompilerError:
        {
            return targetAble_[EnemyTypes::CompilerError] || targetAbleBuff_[EnemyTypes::CompilerError];
            break;
        }
        default:
            break;
        }
    return false;
}

/**
 * @brief Calculate the distance from this tower to the given item on the screen
 * @param item a QGraphicsItem on the screen
 * @return the distance from this tower to the given item as a double
 */
double Tower::distanceTo(QGraphicsItem * item) {
    QLineF line(pos(),item->pos());
    return line.length();
}

/**
 * @brief Returns the tower's attack area
 * @return a QGraphicsEllipseItem that represents the tower's current attack area
 */
QGraphicsEllipseItem* Tower::getAttackArea() {
    return attack_area_;
}

/**
 * @brief Get a list of items that are currently in the tower's range.
 * @return a QList of QGraphicsItem* that contains every item that is in the tower's range
 */
QList<QGraphicsItem*> Tower::getItemInRange() {
    return attack_area_->collidingItems();
}

/**
 * @brief Get a list of towers that are currently in this tower's range
 * @return a QList of Towers* that contains every tower that is in this tower's range
 */
QList<Tower*> Tower::getTowersInRange() {
    QList<QGraphicsItem*> items_in_range = getItemInRange();
    QList<Tower*> towers_in_range;

    // go through all items and add every tower to the list
    for (QGraphicsItem* item : items_in_range) {
        QGraphicsProxyWidget* proxyWidget = dynamic_cast<QGraphicsProxyWidget*>(item);

        // if there is a proxy widget
        if (proxyWidget) {
            QWidget* widget = proxyWidget->widget();
            Tower * tower = dynamic_cast<Tower *>(widget);
            // if this is a tower
            if (tower) {
                towers_in_range.prepend(tower);
            }
        }
    }
    return towers_in_range;
}

/**
 * @brief Return the position of the tower's center on the screen.
 * @return the tower's center as a QPointF item
 */
QPointF Tower::towerCenter() {
    QPoint towerPos = view->getGame()->getSquarePos(y_, x_).toPoint();
    QPointF center(towerPos.x() + this->pixmap().width()/2,
                   towerPos.y() + this->pixmap().height()/2);

    return center;
}

/**
 * @brief Calculate the current total value of the tower.
 * @return the total value of the tower as an int
 */
int Tower::getTotalCost() {
    return totalCost_;
}

/**
 * @brief Increase the tower's damage by the given buff factor.
 * @param buffFactor the buff factor. A buff factor of 0.2 will coresponds to an increase of 20%.
 */
void Tower::damageBuff(double buffFactor) {
    damage_ *= (1 + buffFactor);
}

/**
 * @brief Increase the tower's attack speed by the given buff factor.
 * @param buffFactor the buff factor. A buff factor of 0.2 will coresponds to an increase of 20%.
 */
void Tower::atkSpeedBuff(double buffFactor) {
    if (canFire_) {
        // reduces the attack interval by buffFactor
        // which increases the speed by buffFactor
        attackInterval_ /= (1 + buffFactor);

        // raise the hasAtkSpeedBuff flag
        hasAtkSpdBuff_ = true;

        // schedule to delete the old attackTimer
        attackTimer_->deleteLater();

        // connect a new timer to the getTarget function
        attackTimer_ = new QTimer(this);
        connect(attackTimer_,SIGNAL(timeout()),this,SLOT(getTarget()));
        attackTimer_->start(fmax(attackInterval_, 500));
    }
}

/**
 * @brief Decrease the tower's attack speed by the given buff factor.
 * @param buffFactor the buff factor. A buff factor of 0.2 will coresponds to a decrease of 20%.
 */
void Tower::atkSpeedDebuff(double debuffFactor) {
    if (canFire_) {
        attackInterval_ *= (1 + debuffFactor);

        // lower the hasAtkSpeedBuff flag
        hasAtkSpdBuff_ = false;

        // schedule to delete the old attackTimer
        attackTimer_->deleteLater();

        // connect a new timer to the getTarget function
        attackTimer_ = new QTimer(this);
        connect(attackTimer_,SIGNAL(timeout()),this,SLOT(getTarget()));
        attackTimer_->start(fmax(attackInterval_, 500));
    }
}

/**
 * @brief Check if the tower has any attack speed buff or not.
 * @return true if the tower has attack speed buff, false otherwise
 */
bool Tower::hasAtkSpdBuff() {
    return hasAtkSpdBuff_;
}


/**
 * @brief Allow the tower to target the given enemy type.
 * @param type an enemy type
 */
void Tower::targetTableBuff(EnemyTypes::TYPES type) {
    targetAbleBuff_[type] = true;
}

/**
 * @brief Remove an existing targetable buff from the tower. This will not affect the tower's own targetable ability.
 * In other words if the tower was able to target an enemy type without any buff, it will still be able to do so.
 * @param type an enemy type
 */
void Tower::targetTableDebuff(EnemyTypes::TYPES type) {
    targetAbleBuff_[type] = false;
}

/**
 * @brief Show the tower's attack area on the scene.
 */
void Tower::showAttackArea()
{

    attack_area_->setPen(QPen(QBrush(atkArea_Color),
                              atkArea_LineWidth, atkArea_LineStyle));
}

/**
 * @brief Hide the tower's attack area on the scene.
 */
void Tower::hideAttackArea()
{
    attack_area_->setPen(QPen(QBrush(Qt::transparent),
                              atkArea_LineWidth, atkArea_LineStyle));
}

/**
 * @brief Show the tower's attack area if it is currently hidden, and hide the attack area if it is currently displayed.
 */
void Tower::showHideAttackArea()
{
    if (attack_area_->pen().color()==atkArea_Color){
        hideAttackArea();
    } else {
        showAttackArea();
    }
}

/**
 * @brief Update the tower's text description base on the tower's current stats.
 */
void Tower::updateDescription() {
    // create new Tooltip description for this tower
    description_ = QString("<p><b>-CS Student level %1-</b><br><br>"
                           "<b>Damage: </b>%2<br>"
                           "<b>Attack interval: </b>%3s<br>"
                           "<b>Range: </b>%4<br>"
                           "<b>Pierce: </b>%5<br>"
                           "<b>Total value: </b>%6<br>"
                           "<b>Can target: </b>").arg(QString::number(upgradeLevel_),
                                                            QString::number(damage_),
                                                            QString::number(attackInterval_ / 1000, 'f', 2),
                                                            QString::number(range_),
                                                            QString::number(pierce_),
                                                            QString::number(totalCost_));

    // list out the enemy types that this tower can target at the moment
    for (int i = 0, n = 3; i < n; ++i) {
        QString str = "";
        // check the enemy type
        switch (i) {
        // check if current tower can target given type
        // if yes, append it to the tower's description
        case EnemyTypes::RuntimeError:
        {
            if(targetAble_[EnemyTypes::RuntimeError] || targetAbleBuff_[EnemyTypes::RuntimeError]) {
                // add comma at the start of every element except the first one
                if (i != 0) {
                    str.append(", ");
                }
                str.append("Runtime Errors");
            }
            break;
        }
        case EnemyTypes::MemoryError:
        {
            if(targetAble_[EnemyTypes::MemoryError] || targetAbleBuff_[EnemyTypes::MemoryError]) {
                // add comma at the start of every element except the first one
                if (i != 0) {
                    str.append(", ");
                }
                str.append("Memory Errors");
            }
            break;
        }
        case EnemyTypes::CompilerError:
        {
            if(targetAble_[EnemyTypes::CompilerError] || targetAbleBuff_[EnemyTypes::CompilerError]) {
                // add comma at the start of every element except the first one
                if (i != 0) {
                    str.append(", ");
                }
                str.append("Compiler Errors");
            }
            break;
        }
        default:
            break;
        }

        description_.append(str);
    }

    // close the description
    description_.append("</p>");

    this->setToolTip(description_);
}

/**
 * @brief Check if this is a tower or not.
 * @return true
 */
bool Tower::isTower(){
    return true;
}

/**
 * @brief Add to the tower's total cost.
 * @param cost the cost to be added.
 */
void Tower::addCost(int cost) {
    totalCost_ += cost;
}
