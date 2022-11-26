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
#include <QList>
#include <unordered_set>
#include <queue>
#include <list>

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
                Square* tile = new Square(QPoint(i, j),nullptr);
                QGraphicsProxyWidget* backgroundTile = addWidget(tile);
                mapLayout->addItem(backgroundTile,i,j);
        }
    }
    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(mapLayout);
    gameLayout->addItem(form);
}

/**
 * @brief pointToStringKey An auxilary function for getShortestPath to allow the use of hashsets with QPoints.
 * @param pos QPoint position to be turned into a string.
 * @return a string representing the QPoint.
 */
std::string pointToStringKey(QPoint pos) {
    return std::to_string(pos.x()) + ":" + std::to_string(pos.y());
}

/**
 * @brief getShortestPath A function that uses BFS to find the shortest path for the enemies to take.
 * @param start The start of the enemy path.
 * @param end The end of the enemy path.
 * @param bottomRight The top right tile of the map (to determine the boundaries)
 * @return A list containing QPoints for the shortest path to the end.
 */
QList<QPoint> getShortestPath(QPoint start, QPoint end, QPoint topRight) {
    std::unordered_set<std::string> visited = {pointToStringKey(start)};
    std::queue<QList<QPoint>> to_visit;
    QList<QPoint> first;
    first.append(start);
    to_visit.push(first);
    while (!to_visit.empty()) {
        // Check the top element of the queue
        QList<QPoint> top = to_visit.front();
        QPoint to_check = top.last();
        // Get the four direct neighbors of the element
        QPoint above = to_check + QPoint(0, 1);
        QPoint below = to_check + QPoint(0, -1);
        QPoint left = to_check + QPoint(-1, 0);
        QPoint right = to_check + QPoint(1, 0);
        std::list<QPoint> to_filter{above, below, left, right};
        // Filter to make sure they are all path objects, are in bounds, and have not been visited
        std::list<QPoint> neighbors;
        for (auto i : to_filter) {
            if (i.x() >= 0 && i.x() <= topRight.x() && i.y() >= 0 && i.y() <= topRight.y() && !visited.count(pointToStringKey(i))) {
                // NOTE: CHECKING IF THEY ARE PATH OBJECTS HAS NOT BEEN DONE YET
                neighbors.push_back(i);
            }
        }
        // Check if any are end
        for (auto i : neighbors) {
            top.append(i);
            if (i == end) { // If so, return the shortest path
                return top;
            } else { // Else add them to the queue if they have not yet been visited
                to_visit.push(top);
                // Add i to visited locations
                visited.insert(pointToStringKey(i));
            }
        }
        // Pop the element off the queue
        to_visit.pop();
    }
    // If there is no path possible, return an empty list.
    QList<QPoint> path;
    return path;
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


