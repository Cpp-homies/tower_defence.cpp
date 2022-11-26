#include "tower.h"
#include "enemy.h"
#include <QPointF>
#include <QPen>
#include <QTransform>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include "mainview.h"

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

    // set tower graphics
    QTransform rm;
    rm.rotate(90);
    ogImagePath_ = ":/images/CStudent1.png";
    setPixmap(QPixmap(ogImagePath_).transformed(rm));


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

Tower::Tower(int x, int y, QWidget *parent, int range, int damage, int attackInterval) : Square(x, y, parent),
                                                                    range_(range), damage_(damage), attackInterval_(attackInterval) {
    // set the original damage multipier to 1.0
    damageMultiplier_ = 1.0;

    // set tower graphics
    QTransform rm;
    rm.rotate(90);
    ogImagePath_ = ":/images/CStudent1.png";
    setPixmap(QPixmap(ogImagePath_).transformed(rm));

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
        if (enemy) {
            double cur_dist = distanceTo(enemy);
            if (cur_dist < min_dist){
                min_dist = cur_dist;
                min_point = enemy->scenePos();
                has_target_ = true;
            }
        }
    }

    if (has_target_) {
        this->target_pos_ = min_point;
        fire(target_pos_);
    }
}

//Fires a projectile at the targetPos
void Tower::fire(QPointF targetPos) {

    Projectile* projectile = new Projectile();
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
        Square::setPixmap(QPixmap(ogImagePath_).transformed(transform).scaled(pixmap().size(), Qt::KeepAspectRatioByExpanding));
        rotationAngle_ = angle;// update the rotation angle
    }


}

double Tower::distanceTo(QGraphicsItem * item) {
    QLineF line(pos(),item->pos());
    return line.length();
}

QPointF Tower::towerCenter() {
    QPoint towerPos = view->getGame()->getSquarePos(x_, y_).toPoint();
    QPointF center(towerPos.x() + this->pixmap().width()/2,
                   towerPos.y() + this->pixmap().height()/2);

    return center;
}

bool Tower::upgrade() {
    if (upgradeLevel_ >= maxLevel_) {
        // already max level
        return false;
    }
    else {
        // upgrade the tower according to its level
        upgradeLevel_ += 1;
        QTransform rm;
        rm.rotate(90);

        switch (upgradeLevel_) {
        case 2:
            // increase damage by 20%
            this->damage_ = this->damage_ * 1.2;

            // update tower graphics
            ogImagePath_ = ":/images/CStudent2.png";
            setPixmap(QPixmap(ogImagePath_).transformed(rm));

            break;
        case 3:
/**
TODO : Add other changes to tower characteristic beside damage
*/
            // increase damage by 20%
            this->damage_ = this->damage_ * 1.2;

            // update tower graphics
            ogImagePath_ = ":/images/CStudent3.png";
            setPixmap(QPixmap(ogImagePath_).transformed(rm));

            break;
        case 4:
            // increase damage by 20%
            this->damage_ = this->damage_ * 1.2;

            // update tower graphics
            ogImagePath_ = ":/images/CStudent4.png";
            setPixmap(QPixmap(ogImagePath_).transformed(rm));

            break;
        }

        return true;
    }
}
