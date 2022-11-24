#include "tower.h"
#include <QPointF>
#include <QPen>
#include <QTransform>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include "mainview.h"

extern MainView * view;

Tower::Tower(QWidget *parent) {

}

Tower::Tower(QWidget *parent, int& x, int& y) {
    // set Tower's coordinates in the gridMap
    this->x_ = x;
    this->y_ = y;

    // set tower graphics
    QTransform rm;
    rm.rotate(90);
    setPixmap(QPixmap(":/images/CStudent1.png").transformed(rm));

    // create the attack circle
    attack_area = new QGraphicsEllipseItem(QRect(QPoint(0,0),
                                                 QSize(this->pixmap().width(),
                                                 this->pixmap().height())));
    attack_area->setPen(QPen(QBrush(Qt::black), 0.5, Qt::DashLine));
    view->getGame()->addItem(attack_area);

    attack_area->setScale(range);


    // move the attack area to the right possition
    QPointF area_center(0 + attack_area->rect().width() / 2, 0 + attack_area->rect().height() / 2);
    area_center *= range;
    QLineF ln(area_center,towerCenter());
    attack_area->setPos(0+ln.dx(),0+ln.dy());

    // connect the timer to the getTarget function
    QTimer * timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(getTarget()));
    timer->start(1000);
}

void Tower::getTarget() {
    // get all the items in the tower's range
    QList <QGraphicsItem*> items_in_range = attack_area->collidingItems();

    has_target = false;

    // initiallize min_dist with the largest possible distance in range (the max range of the tower)
    double min_dist = this->range * this->pixmap().width();
    QPointF min_point = QPointF(0,0);

    // find the closest enemy
    for (int i = 0, n = items_in_range.size(); i < n; i++) {
        Enemy * enemy = dynamic_cast<Enemy *>(items_in_range[i]);
        if (enemy) {
            double cur_dist = distanceTo(enemy);
            if (cur_dist < min_dist){
                min_dist = cur_dist;
                min_point = items_in_range[i]->pos();
                has_target = true;
            }
        }
    }

    if (has_target) {
        this->target_pos = min_point;
        fire(target_pos);
    }
}

//Fires a projectile at the targetPos
void Tower::fire(QPointF targetPos) {

    Projectile* projectile = new Projectile();
    projectile->setPos(x(),y()); //takes the same coordinates as the tower
    QLineF ln(pos(),targetPos); //path of the projectile
    int angle = -1 * ln.angle(); //the angle from tower to target

    //set the projectile image to rotate around it's centre and then add it to the scene
    projectile->setTransformOriginPoint(projectile->pixmap().width()/2,projectile->pixmap().height()/2);
    projectile->setRotation(angle);
    view->getGame()->addItem(projectile);

    //rotate the square
//    QTransform transform;
//    transform.rotate(angle);
//    Square::setPixmap(Square::pixmap().transformed(transform));

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
