#include "tower.h"
#include <QPointF>
#include <QPen>
#include <QTransform>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

extern Game * game;

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

    // set the attack circle
    attack_area = new QGraphicsEllipseItem(QRect(QPoint(0,0),
                                                 QSize(this->pixmap().width(),
                                                 this->pixmap().height())));
    attack_area->setPen(QPen(QBrush(Qt::black), 0.5, Qt::DashLine));
    game->scene->addItem(attack_area);

    attack_area->setScale(range);


    // move the attack area to the right possition
    QPointF area_center(0 + attack_area->rect().width() / 2, 0 + attack_area->rect().height() / 2);
    area_center *= range;
//    area_center = this->mapToGlobal(area_center);
    QLineF ln(area_center,towerCenter());
    attack_area->setPos(0+ln.dx(),0+ln.dy());


}

QPointF Tower::towerCenter() {
    QPoint towerPos = game->getSquarePos(x_, y_).toPoint();
    QPointF center(towerPos.x() + this->pixmap().width()/2,
                   towerPos.y() + this->pixmap().height()/2);

    return center;
}
