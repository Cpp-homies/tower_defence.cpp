#include "tower.h"
#include <QPointF>
#include <QPen>
#include <QTransform>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

extern Game * game;

Tower::Tower(QWidget *parent /* unused */) {
    // set tower graphics
    QTransform rm;
    rm.rotate(90);
    setPixmap(QPixmap(":/images/CStudent1.png").transformed(rm));

    // get the tower center
//    QPointF tower_center(x() + this->pixmap()->width() / 2, y() + this->pixmap()->width() / 2);

    // set the attack circle
    attack_area = new QGraphicsEllipseItem(QRect(this->pos(),
                                                 QSize(this->pixmap().width(),
                                                 this->pixmap().height())));
    game->scene->addItem(attack_area);
    // draw out the attack area
//    attack_area->setPen(QPen(QBrush(Qt::black), 2, Qt::DashLine));
//    QPainter* p = new QPainter(this);
//    p->drawEllipse(attack_area->rect());
////    p->setPen(QPen(QBrush(Qt::black), 2, Qt::DashLine));
//    QStyleOptionGraphicsItem* o = new QStyleOptionGraphicsItem();
//    attack_area->paint(p, o);

//    delete p;
//    delete o;

    attack_area->setScale(5);
}
