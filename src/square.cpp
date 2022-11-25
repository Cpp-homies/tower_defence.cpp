#include "square.h"
#include "projectile.h"
#include "game.h"
#include "mainview.h"
#include <QTransform>

extern MainView * view;
/**
 * @brief Square constructor
 * @param x The X-coordinate (column) of the square.
 * @param y The Y-coordinate (row) of the square.
 * @param parent The parent object of the square.
 */
Square::Square(int x, int y, QWidget *parent)
    : QLabel(parent), x_(x), y_(y)
{
    // set randomized tile picture
    tilePics << QPixmap(":/images/Tile.png") << QPixmap(":/images/Tile2.png") << QPixmap(":/images/Tile3.png") << QPixmap(":/images/Tile4.png");
    int r = rand() % 4;
    setPixmap(tilePics[r]);
}
/**
 * @brief Event handler for mouse events.
 */
void Square::mousePressEvent(QMouseEvent* /* unused */){

    setPixmap(QPixmap(":/images/CStudent1.png"));

    fire(view->getGame()->getSquarePos(2,2));
}

/**
 * @brief Fires a projectile at the target position.
 * @param targetPos The target position.
 */
void Square::fire(QPointF targetPos){

    Projectile* projectile = new Projectile();
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
