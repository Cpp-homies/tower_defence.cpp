#include "game.h"
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QPushButton>
#include "square.h"
#include "mainview.h"
#include "tower.h"

#include <QIcon>
#include <QScrollBar>

extern MainView * view;

Game::Game(QObject* parent): QGraphicsScene(parent)
{
    health_ = 100;
    currency_ = 100;
    level_ = 1;
    score_ = 0;

    createMap();

}

void Game::createMap(){





    layout = new QGraphicsGridLayout();
    layout->setSpacing(0);

    for(int i = 0; i<10; ++i)
    {
        for (int j = 0; j < 10; ++j) {

<<<<<<< HEAD
                Square* tile = new Square(nullptr, i, j);
                QGraphicsProxyWidget* backgroundTile = scene->addWidget(tile);
=======
                Square* tile = new Square(nullptr);
                QGraphicsProxyWidget* backgroundTile = addWidget(tile);
>>>>>>> master
                layout->addItem(backgroundTile,i,j);


        }
    }


    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    addItem(form);
}

bool Game::isLost() const{
    return health_>0;
}

int Game::getHealth() const {
    return health_;
}

int Game::getCurrency() const {
    return currency_;
}

int Game::getLevel() const {
    return level_;
}

int Game::getScore() const {
    return score_;
}

void Game::changeHealth (int dHealth) {
    health_+=dHealth;
}

void Game::changeCurrency (int dCurrency) {
    currency_+=dCurrency;
}

void Game::changeScore (int dPoints) {
    score_+=dPoints;
}

void Game::advanceLevel () {
    level_++;
}

//just testing scene changing
//can be used for other purposes
void Game::keyPressEvent(QKeyEvent* /* unused */)
{
    view->showLeaderboard();

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
