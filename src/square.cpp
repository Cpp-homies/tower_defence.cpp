#include "square.h"
#include "projectile.h"
#include "game.h"
#include <QTransform>

extern Game * game;

Square::Square(QWidget *parent)
    : QLabel(parent)
{
    // set randomized tile picture
    tilePics << QPixmap(":/images/Tile.png") << QPixmap(":/images/Tile2.png") << QPixmap(":/images/Tile3.png") << QPixmap(":/images/Tile4.png");
    int r = rand() % 4;
    setPixmap(tilePics[r]);
}

void Square::mousePressEvent(QMouseEvent* /* unused */){

    setPixmap(QPixmap(":/images/CStudent1.png"));

    fire(game->getSquarePos(2,2));
}

void Square::fire(QPointF targetPos){

    Projectile* projectile = new Projectile();
    projectile->setPos(x(),y());
    QLineF ln(pos(),targetPos);
    int angle = -1 * ln.angle();
    QTransform transform;
    transform.rotate(angle);
    Square::setPixmap(Square::pixmap().transformed(transform));
    projectile->setTransformOriginPoint(projectile->pixmap().width()/2,projectile->pixmap().height()/2);
    projectile->setRotation(angle);
    game->scene->addItem(projectile);
}
