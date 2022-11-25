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

#include <QIcon>
#include <QScrollBar>

extern MainView * view;

/**
 * @brief Game object constructor
 * @param parent Parent widget
 */
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
/**
 * @brief Creates a map.
 */
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
}
/**
 * @brief Creates game controls
 */
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


/**
 * @brief Checks if the game is has been lost.
 * @return A boolean representing the loss state of the game.
 */
bool Game::isLost() const{
    return health_>0;
}
/**
 * @brief Gets the remaining number of reputation (health) of the player.
 * @return The number of remaining reputation (health) points that the player has.
 */
int Game::getHealth() const {
    return health_;
}
/**
 * @brief Gets the amount of currency the player has.
 * @return The amount of currency the player has.
 */
int Game::getCurrency() const {
    return currency_;
}
/**
 * @brief Gets the current level of the game.
 * @return The level of the game.
 */
int Game::getLevel() const {
    return level_;
}
/**
 * @brief Gets the current score of the game.
 * @return The current score of the game.
 */
int Game::getScore() const {
    return score_;
}
/**
 * @brief Changes the number of reputation points (health) the player has.
 * @param dHealth The number of reputation points (health) to add or subtract (depending on if the value is positive or negative).
 */
void Game::changeHealth (int dHealth) {
    health_+=dHealth;
}
/**
 * @brief Changes the amount of currency the player holds.
 * @param dCurrency The amount of currency to add or subtract (depending on if the value given is positive or negative) from the player's current currency,
 */
void Game::changeCurrency (int dCurrency) {
    currency_+=dCurrency;
}
/**
 * @brief Changes the score of the game.
 * @param dPoints The amount of points to add or subtract (depending on if the value given is positive or negative) from the current score.
 */
void Game::changeScore (int dPoints) {
    score_+=dPoints;
}
/**
 * @brief Increases the level of the game.
 */
void Game::advanceLevel () {
    level_++;
}

//just testing scene changing
//can be used for other purposes
/**
 * @brief Event handler for keypresses.
 */
void Game::keyPressEvent(QKeyEvent* /* unused */)
{
    view->showLeaderboard();

}
/**
 * @brief Gets the absolute position of a square.
 * @param row The row number of the square in question.
 * @param column The column number of the square in question.
 * @return A QPointF object containing the absolute position of the square.
 */
QPointF Game::getSquarePos(int row, int column){

    return mapLayout->itemAt(row,column)->graphicsItem()->scenePos();
}


