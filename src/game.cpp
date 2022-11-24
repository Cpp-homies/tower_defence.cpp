#include "game.h"
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QGraphicsTextItem>
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

    gameLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    createMap();
    createGameControls();
    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(gameLayout);
    addItem(form);

}

void Game::createMap(){





    mapLayout = new QGraphicsGridLayout();
    mapLayout->setSpacing(0);

    for(int i = 0; i<10; ++i)
    {
        for (int j = 0; j < 10; ++j) {
                Square* tile = new Square(i,j,nullptr);
                QGraphicsProxyWidget* backgroundTile = addWidget(tile);
                mapLayout->addItem(backgroundTile,i,j);

        }
    }


    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(mapLayout);
    gameLayout->addItem(form);

    // add a dummy enemy for testing
    Enemy* enemy = new Enemy();
    addItem(enemy);
}

void Game::createGameControls()
{
    //for testing purposes, all Layouts will auto adjust the size
    controlsLayout = new QGraphicsLinearLayout(Qt::Vertical);
    for(int i = 0; i<5; ++i)
    {
        QLabel* test = new QLabel();
        test->setText(QString("success!!"));
        QGraphicsProxyWidget* player = addWidget(test);
        controlsLayout->addItem(player);
    }

    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(controlsLayout);
    gameLayout->addItem(form);
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

    return mapLayout->itemAt(row,column)->graphicsItem()->scenePos();
}

/**
*
* TODO: delete the square without crashing the game
* TODO: use a table to keep track of where the empty squares, towers, and path are
*       and update the if condition accordingly
*/
bool Game::buildTower(int row, int column) {
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();

    // if there is a tower occupying the square, return false
    if (dynamic_cast<Tower*>(widget)) {
        return false;
    }
    else {
        // create a new tower and add it to the scene
        QGraphicsWidget* tower = this->addWidget(new Tower(row, column, nullptr));

        // remove the current square from the grid
        this->removeItem(item->graphicsItem());
        this->mapLayout->removeItem(item);

        // add a tower to the grid at the given possition
        this->mapLayout->addItem(tower, row, column);

/**
*
* TODO: delete the square without crashing the game
*/
        // delete the current square
//        delete square;
        return true;
    }
}
