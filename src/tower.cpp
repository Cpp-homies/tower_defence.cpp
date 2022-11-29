#include "tower.h"
#include "enemy.h"
#include <QPointF>
#include <QPen>
#include <QTransform>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include <typeinfo>
#include <iostream>
#include "compilererror.h"
#include "memoryerror.h"
#include "runtimeerror.h"
#include "mainview.h"
#include <QGraphicsPixmapItem>

extern MainView * view;

const qreal atkArea_LineWidth = 0.3;
const Qt::PenStyle atkArea_LineStyle = Qt::DashLine;
const Qt::GlobalColor atkArea_Color = Qt::black;


//Tower::Tower(QWidget *parent) : Square(parent) {

//}

Tower::Tower(int x, int y, QWidget *parent) : Square(x, y, parent) {
    // set the default tower stats
    range_ = 5;
    damage_ = 10;
    attackInterval_ = 1000;
    damageMultiplier_ = 1.0;

    upgradeLevel_ = 1;
    maxLevel_ = 4;

    // initialize the targetable enemies at first the tower can only target normal enemies
    std::fill_n(targetAble_, std::size(targetAble_), false);
    targetAble_[EnemyTypes::normal] = true;

    // set tower graphics
    ogImagePath_ = ":/images/CStudent1.png";
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
    QTimer * timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(getTarget()));
    timer->start(attackInterval_);
}

// constructor that set specific stats, used in subclasses of tower
Tower::Tower(int x, int y, int range, int damage, int attackInterval, QWidget *parent) : Square(x, y, parent),
                                                                    range_(range), damage_(damage), attackInterval_(attackInterval) {
    // set the original damage multipier to 1.0
    damageMultiplier_ = 1.0;

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
    QTimer * timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(getTarget()));
    timer->start(attackInterval_);
}

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

//Fires a projectile at the targetPos
void Tower::fire(QPointF targetPos) {

    Projectile* projectile = new Projectile(damage_);
    projectile->setPos(view->getGame()->getSquarePos(x_,y_)); //takes the same coordinates as the tower
    QLineF ln(view->getGame()->getSquarePos(x_,y_),targetPos); //path of the projectile
    int angle = -1 * ln.angle(); //the angle from tower to target

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

bool Tower::isTargetable(Enemy* enemy) {

    // iterate through the targetable enemy types
    for (int i = 0; i < 3; i++) {
        // if the current enemy type is targetable
        if (targetAble_[i]) {
            // try casting the input enemy to the current type
            switch (i) {
            case EnemyTypes::boss:
            {
                // insert casting here after the merge with enemy implementation
                break;
            }
            case EnemyTypes::memory:
            {
                // insert casting here after the merge with enemy implementation
                break;
            }
            case EnemyTypes::normal:
            {
                // insert casting here after the merge with enemy implementation
                break;
            }
            default:
                break;
            }
        }
    }
    return true;
}

double Tower::distanceTo(QGraphicsItem * item) {
    QLineF line(pos(),item->pos());
    return line.length();
}

QList<QGraphicsItem*> Tower::getItemInRange() {
    return attack_area_->collidingItems();
}

QPointF Tower::towerCenter() {
    QPoint towerPos = view->getGame()->getSquarePos(x_, y_).toPoint();
    QPointF center(towerPos.x() + this->pixmap().width()/2,
                   towerPos.y() + this->pixmap().height()/2);

    return center;
}

void Tower::damageBuff(double buffFactor) {
    damageMultiplier_ *= buffFactor;
}

bool Tower::upgrade() {
    if (upgradeLevel_ >= maxLevel_) {
        // already max level
        return false;
    }
    else {
        // upgrade the tower according to its level
        upgradeLevel_ += 1;

        switch (upgradeLevel_) {
        case 2:
            // increase damage by 20%
            this->damage_ = this->damage_ * 1.2;
            targetAble_[EnemyTypes::memory] = true;

            // update tower graphics
            ogImagePath_ = ":/images/CStudent2.png";
            towerImg->setPixmap(QPixmap(ogImagePath_));

            break;
        case 3:
/**
TODO : Add other changes to tower characteristic beside damage
*/
            // increase damage by 20%
            this->damage_ = this->damage_ * 1.2;

            // update tower graphics
            ogImagePath_ = ":/images/CStudent3.png";
            towerImg->setPixmap(QPixmap(ogImagePath_));

            break;
        case 4:
            // increase damage by 20%
            this->damage_ = this->damage_ * 1.2;

            // update tower graphics
            ogImagePath_ = ":/images/CStudent4.png";
            towerImg->setPixmap(QPixmap(ogImagePath_));

            break;
        }

        return true;
    }
}
