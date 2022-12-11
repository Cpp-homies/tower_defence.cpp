/**
 * @file square.cpp
 * @authors Siim Kasela (siim.kasela@aalto.fi), Saku Kovanen (saku.kovanen@aalto.fi), Harvey Lim (harvey.lim@aalto.fi), Hung Vu (hung.h.vu@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "square.h"
#include "projectile.h"
#include "tower.h"
#include "game.h"
#include "mainview.h"
#include <QTransform>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>

extern MainView * view;

/**
 * @brief Construct a new Square:: Square object
 * 
 * @param x X-coordinate of the square.
 * @param y Y-coordinate of the square.
 * @param parent The parent widget of the square.
 */
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

/**
 * @brief Based on the mode, builds a tower or upgrades the tower on the square. Changes mode back to normal mode.
 * 
 */
void Square::mousePressEvent(QMouseEvent* /* unused */){
    // switch case for handling interactions
    // according to current mode of the game
    switch (view->getGame()->getMode()) {
    case Modes::build:
        // build the tower according to the chosen type
        switch (view->getGame()->getBuildType()) {
        case TowerTypes::CS_Student:
            // if the build fail
            if (!view->getGame()->buildTower(this->y_, this->x_, TowerTypes::CS_Student)) {
                // do something

                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            else {
                // build successful, schedule to delete the square
                deleteLater();
                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            break;
        case TowerTypes::TA:
            // if the build fail
            if (!view->getGame()->buildTower(this->y_, this->x_, TowerTypes::TA)) {
                // do something

                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            else {
                // build successful, schedule to delete the square
                deleteLater();
                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            break;
        case TowerTypes::SearchEngine:
            // if the build fail
            if (!view->getGame()->buildTower(this->y_, this->x_, TowerTypes::SearchEngine)) {
                // do something

                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            else {
                // build successful, schedule to delete the square
                deleteLater();
                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            break;
        case TowerTypes::Comment:
            // if the build fail
            if (!view->getGame()->buildTower(this->y_, this->x_, TowerTypes::Comment)) {
                // do something

                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            else {
                // build successful, schedule to delete the square
                //deleteLater();
                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            break;
        case TowerTypes::LanguageServer:
            // if the build fail
            if (!view->getGame()->buildTower(this->y_, this->x_, TowerTypes::LanguageServer)) {
                // do something

                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            else {
                // build successful, schedule to delete the square
                deleteLater();
                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            break;
        case TowerTypes::Valgrind:
            // if the build fail
            if (!view->getGame()->buildTower(this->y_, this->x_, TowerTypes::Valgrind)) {
                // do something

                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            else {
                // build successful, schedule to delete the square
                deleteLater();
                // reset the mode of the game back to normal
                view->getGame()->setMode(Modes::normal);
            }
            break;
        default:
            break;
        }

        break;
    case Modes::upgrade:
        // if upgrade fail
        if (!view->getGame()->upgradeTower(this->y_, this->x_)) {
            // do something

            // reset the state of the game back to normal
            view->getGame()->setMode(Modes::normal);
        }
        else {
            // upgrade successful, reset the mode of the game back to normal
            view->getGame()->setMode(Modes::normal);
        }
        break;
    case Modes::sell:
        // if sell fail
        if (!view->getGame()->sellTower(this->y_, this->x_)){

        }
        else {
            // sell successfulll schedule to delete the tower
            deleteLater();
        }
        // reset the state of the game back to normal
        view->getGame()->setMode(Modes::normal);
        break;
    case Modes::normal:
        view->getGame()->hideAllAttackAreasExcept();
        if (this->isTower()){
            this->showHideAttackArea();
        }
        break;
    default:
        break;
    }

}

/**
 * @brief Fires a projectile at the targetPos.
 * 
 * @param targetPos Position the projectile is fired at.
 */
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

/**
 * @brief Rotates the square pixmap.
 * 
 * @param angle Angle to be rotated.
 * @param pixmap Pixmap to be rotated.
 * @return QPixmap Rotated pixmap.
 */
QPixmap Square::rotate(int angle, QPixmap pixmap){
    QTransform rotation;
    rotation.rotate(angle);
    return pixmap.transformed(rotation);
}

/**
 * @brief Checks if the square is a tower.
 * 
 * @return true Square is a tower.
 * @return false Square is not a tower.
 */
bool Square::isTower(){
    return false;
}
