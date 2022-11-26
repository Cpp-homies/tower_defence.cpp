#include "square.h"
#include "projectile.h"
#include "tower.h"
#include "game.h"
#include "mainview.h"
#include <QTransform>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>

extern MainView * view;

Square::Square(int x, int y, QWidget *parent)
    : QLabel(parent), x_(x), y_(y)
{
    // set randomized tile picture
    QList<QPixmap> tilePics;
    tilePics << QPixmap(":/images/Tile.png") << rotate(90, QPixmap(":/images/Tile.png"))
             << rotate(180, QPixmap(":/images/Tile.png")) << rotate(270, QPixmap(":/images/Tile.png"));
    int r = rand() % 4;
    setPixmap(tilePics[r]);
}

void Square::mousePressEvent(QMouseEvent* /* unused */){
    // if the build fail
    if (!view->getGame()->buildTower(this->x_, this->y_)) {
        // do something
    }
    else {
        // build successful, schedule to delete the square
        deleteLater();
    }

//    fire(view->getGame()->getSquarePos(2,2));
}

//Fires a projectile at the targetPos
void Square::fire(QPointF targetPos){

    Projectile* projectile = new Projectile(10);
    projectile->setPos(view->getGame()->getSquarePos(x_,y_)); //takes the same coordinates as the tower
    QLineF ln(view->getGame()->getSquarePos(x_,y_),targetPos); //path of the projectile
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


QPixmap Square::rotate(int angle, QPixmap pixmap){
    QTransform rotation;
    rotation.rotate(angle);
    return pixmap.transformed(rotation);
}
