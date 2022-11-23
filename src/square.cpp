#include "square.h"
#include "game.h"
#include "tower.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>

extern Game * game;

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
//    setPixmap(QPixmap(":/images/CStudent1.png"));



    // create a new tower and add it to the scene
    QGraphicsWidget* tower = game->scene->addWidget(new Tower(nullptr, x_, y_));

    // remove the current square from the grid
    QGraphicsLayoutItem* square = game->layout->itemAt(x_, y_);
    game->layout->removeItem(square);

    // add a tower to the grid at the given possition
    game->layout->addItem(tower, x_, y_);

    // delete the current square
    deleteLater();
}
