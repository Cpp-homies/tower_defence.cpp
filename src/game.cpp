#include "game.h"
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QPushButton>
#include "square.h"

#include <QIcon>
#include <QScrollBar>


Game::Game()
{
    health_ = 100;
    currency_ = 100;
    level_ = 1;
    score_ = 0;

    createMap();

}

void Game::createMap(){

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

                Square* tile = new Square(nullptr);
                QGraphicsProxyWidget* backgroundTile = scene->addWidget(tile);
                layout->addItem(backgroundTile,i,j);


        }
    }


    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    scene->addItem(form);
    setScene(scene);
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

QPointF Game::getSquarePos(int row, int column){

    return layout->itemAt(row,column)->graphicsItem()->scenePos();
}


