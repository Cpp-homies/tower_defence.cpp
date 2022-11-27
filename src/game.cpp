#include "game.h"
#include "square.h"
#include "mainview.h"
#include "button.h"
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QGraphicsTextItem>
#include <QToolButton>
#include "square.h"
#include "mainview.h"
#include "enemy.h"
#include "tower.h"

#include <QIcon>
#include <QScrollBar>
#define BUILD_BUTTON_SIZE 80
extern MainView * view;

Game::Game(QObject* parent): QGraphicsScene(parent)
{
    // set starting values of health, currency etc
    health_ = 100;
    currency_ = 100;
    level_ = 1;
    score_ = 0;
    mode_ = Modes::normal;

    // set size 1280x717 (use 717 height because we dont want scroll)
    setSceneRect(0,0,1280,717);

    gameLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    createMap();
    createGameControls();
    QGraphicsWidget *form = new QGraphicsWidget;
    gameLayout->setContentsMargins(0,0,0,0);
    mapLayout->setSpacing(0);
    form->setLayout(gameLayout);
    addItem(form);

}

void Game::createMap(){





    mapLayout = new QGraphicsGridLayout();

    for(int i = 0; i<11; ++i)
    {
        for (int j = 0; j < 11; ++j) {

                Square* tile = new Square(i,j,nullptr);
                QGraphicsProxyWidget* backgroundTile = addWidget(tile);
                mapLayout->addItem(backgroundTile,i,j);

        }
    }

    // set margins to 0
    mapLayout->setContentsMargins(0,0,0,0);

    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(mapLayout);
    gameLayout->addItem(form);

}

void Game::createGameControls()
{
    //for testing purposes, all Layouts will auto adjust the size
    controlsLayout = new QGraphicsLinearLayout(Qt::Vertical);
//    for(int i = 0; i<5; ++i)
//    {
//        QLabel* test = new QLabel();
//        test->setText(QString("success!!"));
//        QGraphicsProxyWidget* player = addWidget(test);
//        controlsLayout->addItem(player);
//    }

    // set margins to 0
    controlsLayout->setContentsMargins(0,0,0,0);

    // main menu button
    Button * menuButton = new Button(QString("Main menu"), 200, 50);
    int lxPos = this->width() - menuButton->boundingRect().width() - 40;
    int lyPos = 40;
    menuButton->setPos(lxPos, lyPos);
    connect(menuButton, SIGNAL(clicked()), this, SLOT(showMenu()));
    menuButton->setZValue(10);
    addItem(menuButton);

    // upgrade button
    Button * upgradeButton = new Button(QString("Upgrade Tower"), 240, 50);
//    upgradeButton->setRect(QRectF(upgradeButton->boundingRect().topLeft(),
//                            QSizeF(upgradeButton->boundingRect().width() + 20, upgradeButton->boundingRect().height())));

    int uxPos = this->width() - upgradeButton->boundingRect().width() - 40;
    int uyPos = this->height() - upgradeButton->boundingRect().height() - 20;
    upgradeButton->setPos(uxPos, uyPos);
    connect(upgradeButton, SIGNAL(clicked()), this, SLOT(enterUpgradeMode()));
    upgradeButton->setZValue(10);
    addItem(upgradeButton);

    // tower build buttons
        // create CSstudent button
    QToolButton* build_CSstudent = new QToolButton();
    // create the icon for the button
    QTransform tr;
    tr.rotate(90);
    build_CSstudent->setIcon(QIcon(QPixmap(":/images/CStudent1.png").transformed(tr)));
    build_CSstudent->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_CSstudent, SIGNAL(clicked()), this, SLOT(enterBuildMode()));

    // add the button to the control layout
    QGraphicsProxyWidget* CSstudentWidget = addWidget(build_CSstudent);
    controlsLayout->addItem(CSstudentWidget);


        // create TA button
    QToolButton* build_TA = new QToolButton();
    // create the icon for the button
    build_TA->setIcon(QIcon(QPixmap(":/images/TA.png").transformed(tr)));
    build_TA->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_TA, SIGNAL(clicked()), this, SLOT(enterBuildMode()));

    // add the button to the control layout
    QGraphicsProxyWidget* TAWidget = addWidget(build_TA);
    controlsLayout->addItem(TAWidget);


        // create Search Engine button
    QToolButton* build_SE = new QToolButton();
    // create the icon for the button
    build_SE->setIcon(QIcon(QPixmap(":/images/Bing.png").transformed(tr)));
    build_SE->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_SE, SIGNAL(clicked()), this, SLOT(enterBuildMode()));

    // add the button to the control layout
    QGraphicsProxyWidget* SEWidget = addWidget(build_SE);
    controlsLayout->addItem(SEWidget);


        // create Language server button
    QToolButton* build_LS = new QToolButton();
    // create the icon for the button
    build_LS->setIcon(QIcon(QPixmap(":/images/Language_server.png").transformed(tr)));
    build_LS->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_LS, SIGNAL(clicked()), this, SLOT(enterBuildMode()));

    // add the button to the control layout
    QGraphicsProxyWidget* LSWidget = addWidget(build_LS);
    controlsLayout->addItem(LSWidget);


        // create Language server button
    QToolButton* build_Valgrind = new QToolButton();
    // create the icon for the button
    build_Valgrind->setIcon(QIcon(QPixmap(":/images/Valgrind.png").transformed(tr)));
    build_Valgrind->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_Valgrind, SIGNAL(clicked()), this, SLOT(enterBuildMode()));

    // add the button to the control layout
    QGraphicsProxyWidget* ValWidget = addWidget(build_Valgrind);
    controlsLayout->addItem(ValWidget);


    // add the control layout to the game layout
    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(controlsLayout);
    gameLayout->addItem(form);
}

void Game::createWave(QList<QPoint> path)
{
    Enemy* enemy = new Enemy(1,1,1, convertCoordinates(path), *this);
    addItem(enemy);
}

QList<QPointF> Game::convertCoordinates(QList<QPoint> path)
{
    QList<QPointF> pathF =  QList<QPointF>(path.length());
    int i = 0;
    foreach(QPoint coord,path)
    {
        QPointF coordF = getSquarePos(coord.x(),coord.y());
        pathF[i]= QPointF(coordF);
        i++;
    }
    return pathF;
}



bool Game::isLost() const{
    return health_<=0;
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

Modes::MODES Game::getMode() const {
    return mode_;
}

TowerTypes::TYPES Game::getBuildType() const {
    return buildType_;
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

void Game::setMode(Modes::MODES m) {
    mode_ = m;
}

void Game::advanceLevel () {
    level_++;
}


//just testing scene changing
//can be used for other purposes
void Game::keyPressEvent(QKeyEvent* /* unused */)
{
    QList<QPoint> path;
    path << QPoint(7,0) << QPoint(7,1) << QPoint(6,1)<< QPoint(6,4);
    createWave(path);

}

QPointF Game::getSquarePos(int row, int column){

    return mapLayout->itemAt(row,column)->graphicsItem()->scenePos();
}

void Game::showMenu(){
    view->showMenu();
}

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

        return true;
    }
}

QWidget* Game::getWidgetAt(int row, int column) {
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();
    return widget;
}

bool Game::isTower(int row, int column) {
    return dynamic_cast<Tower*>(getWidgetAt(row, column));
}

void Game::enterUpgradeMode() {
    mode_ = Modes::upgrade;
}

void Game::enterBuildMode() {
    mode_ = Modes::build;
}

bool Game::upgradeTower(int row, int column) {
    QWidget* widget = getWidgetAt(row, column);
    Tower* tower= dynamic_cast<Tower*>(widget);

    // if the square is a tower, upgrade it if possible
    if (tower) {
        return tower->upgrade();// return the state of the upgrade
    }
    else {
        // not a tower
        return false;
    }
}
