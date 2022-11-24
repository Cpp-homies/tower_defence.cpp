#include "square.h"
#include "projectile.h"
#include "tower.h"
#include "game.h"
#include "mainview.h"
#include <QTransform>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>

extern MainView * view;

Square::Square(QWidget *parent)
    : QLabel(parent)
{
    // set randomized tile picture
    tilePics << QPixmap(":/images/Tile.png") << QPixmap(":/images/Tile2.png") << QPixmap(":/images/Tile3.png") << QPixmap(":/images/Tile4.png");
    int r = rand() % 4;
    setPixmap(tilePics[r]);
}

Square::Square(QWidget *parent, int& x, int& y) : QLabel(parent) {
    // set randomized tile picture
    tilePics << QPixmap(":/images/Tile.png") << QPixmap(":/images/Tile2.png") << QPixmap(":/images/Tile3.png") << QPixmap(":/images/Tile4.png");
    int r = rand() % 4;
    setPixmap(tilePics[r]);

    this->x_ = x;
    this->y_ = y;
}

void Square::mousePressEvent(QMouseEvent* /* unused */){
    // if the build fail
    if (view->getGame()->buildTower(this->x_, this->y_)) {
        // do something
    }

    deleteLater();

//    fire(view->getGame()->getSquarePos(2,2));
}

//Fires a projectile at the targetPos
void Square::fire(QPointF targetPos){

    Projectile* projectile = new Projectile();
    projectile->setPos(x(),y()); //takes the same coordinates as the tower
    QLineF ln(pos(),targetPos); //path of the projectile
    int angle = -1 * ln.angle(); //the angle from tower to target

    //set the projectile image to rotate around it's centre and then add it to the scene
    projectile->setTransformOriginPoint(projectile->pixmap().width()/2,projectile->pixmap().height()/2);
    projectile->setRotation(angle);
    view->getGame()->addItem(projectile);

    //rotate the square
    QTransform transform;
    transform.rotate(angle);
    Square::setPixmap(Square::pixmap().transformed(transform));

}
