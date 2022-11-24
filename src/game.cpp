#include "game.h"
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QPushButton>
#include "square.h"
#include "tower.h"

#include <QIcon>
#include <QScrollBar>


Game::Game()
{
    scene = new QGraphicsScene(this);
    scene ->setSceneRect(0,0,1000,1000);

    setFixedSize(1000,1000);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout = new QGraphicsGridLayout();
    layout->setSpacing(0);

    for(int i = 0; i<10; ++i)
    {
        for (int j = 0; j < 10; ++j) {

                Square* tile = new Square(nullptr, i, j);
                QGraphicsProxyWidget* backgroundTile = scene->addWidget(tile);
                layout->addItem(backgroundTile,i,j);


        }
    }


    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    scene->addItem(form);
    setScene(scene);
}

QPointF Game::getSquarePos(int row, int column){

    return layout->itemAt(row,column)->graphicsItem()->scenePos();
}

/**
*
* TODO: delete the square without crashing the game
* TODO: use a table to keep track of where the empty squares, towers, and path are
*       and update the if condition accordingly
*/
bool Game::buildTower(int row, int column) {
    QGraphicsLayoutItem* square = this->layout->itemAt(row, column);

    // if there is a tower occupying the square, return false
    if (dynamic_cast<Tower*>(square->graphicsItem())) {
        return false;
    }
    else {
        // create a new tower and add it to the scene
        QGraphicsWidget* tower = this->scene->addWidget(new Tower(nullptr, row, column));

        // remove the current square from the grid
        this->scene->removeItem(square->graphicsItem());
        this->layout->removeItem(square);

        // add a tower to the grid at the given possition
        this->layout->addItem(tower, row, column);

/**
*
* TODO: delete the square without crashing the game
*/
        // delete the current square
//        delete square;
        return true;
    }
}
