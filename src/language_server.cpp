#include "language_server.h"
#include "projectile.h"
#include "cmath"

Language_Server::Language_Server(int row, int column, QWidget *parent) : Tower(row, column, 5, 1, 2000, parent) {
    // set high piercing
    pierce_ = 3;
    // set Language Server graphics
    ogImagePath_ = ":/images/Language_server.png";
    projectileImagePath_ = ":/images/Language_server_projectile.png";
    this->towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    this->towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );
}

void Language_Server::fire(QPointF targetPos) {
    // create a proxy projectile to use as shooting visual effect
    Projectile* projectile = new Projectile(damage_, projectileImagePath_, pierce_);
    projectile->setPos(view->getGame()->getSquarePos(y_,x_)); //takes the same coordinates as the tower
//    projectile->setScale(ceil(this->range_ / 2));

    QLineF ln(view->getGame()->getSquarePos(y_,x_),targetPos); //path of the projectile
    int angle = -1 * ln.angle(); //the angle from tower to target
    int maxTowerRange = ceil(this->getAttackArea()->boundingRect().width() * (double)range_ / 2.0);
    projectile->setMaxRange(maxTowerRange);// set max range of the projectile to the range of the tower

    //set the projectile image to rotate around it's centre and then add it to the scene
    projectile->setTransformOriginPoint(projectile->pixmap().width()/2,projectile->pixmap().height()/2);
    projectile->setRotation(angle);
    view->getGame()->addItem(projectile);
}
