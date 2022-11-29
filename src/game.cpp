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
#include "tower.h"
#include "compilererror.h"
#include "memoryerror.h"
#include "runtimeerror.h"
#include "enemy.h"
#include "tower.h"
#include "cs_student.h"
#include "ta.h"

#include <QIcon>
#include <QScrollBar>
#define BUILD_BUTTON_SIZE 100
extern MainView * view;

Game::Game(QObject* parent): QGraphicsScene(parent)
{
    // set starting values of health, currency etc
    health_ = 10;
    currency_ = 100;
    level_ = 1;
    score_ = 0;
    enemyCount_ = 0;
    wavesCount_ = 0;
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
    controlsLayout = new QGraphicsGridLayout();

    // set spacings
    controlsLayout->setContentsMargins(130,130,120,120);
    controlsLayout->setSpacing(60);

    // Font for displaying round, health and wealth
    QFont statsFont("Roboto", 32);

        // display round number
    // Create the roundnumber indicator
    QGraphicsTextItem * roundIndicator = new QGraphicsTextItem(QString("round"));
    // set font
    QFont roundIndicatorFont("Roboto", 32, 900);
    roundIndicator->setDefaultTextColor(Qt::white);
    roundIndicator->setFont(roundIndicatorFont);
    // set position
    int RIxPos = 744;
    int RIyPos = 40;
    roundIndicator->setPos(RIxPos,RIyPos);
    // add indicator to the scene
    addItem(roundIndicator);
    // Create the round number
    QGraphicsTextItem * roundNumber = new QGraphicsTextItem(QString::number(level_));
    // set font
    roundNumber->setDefaultTextColor(Qt::white);
    roundNumber->setFont(statsFont);
    // set position
    int RNxPos = 860;
    int RNyPos = 40;
    roundNumber->setPos(RNxPos,RNyPos);
    // add to the scene
    addItem(roundNumber);

        // display health
    // Create the health indicator
    QGraphicsRectItem * healthIndicator = new QGraphicsRectItem();
    healthIndicator->setPen(QPen(Qt::transparent));
    healthIndicator->setRect(0,0,64,64);
    QBrush healthIndicatorB;
    healthIndicatorB.setTexture(QPixmap(":/images/hp_icon.png"));
    healthIndicator->setBrush(healthIndicatorB);
    // set position
    int HIxPos = 920;
    int HIyPos = 40;
    // QGraphicsProxyWidget* healthIndicatorWidget = addWidget(healthIndicator);
    healthIndicator->setPos(HIxPos,HIyPos);
    // add indicator to the scene
    addItem(healthIndicator);
    // Create the round number
    QGraphicsTextItem * healthNumber = new QGraphicsTextItem(QString::number(health_));
    // set font
    healthNumber->setDefaultTextColor(Qt::white);
    healthNumber->setFont(statsFont);
    // set position
    int HNxPos = HIxPos + 64;
    int HNyPos = 40;
    healthNumber->setPos(HNxPos,HNyPos);
    // add to the scene
    addItem(healthNumber);

        // display wealth
    // Create the wealth indicator
    QGraphicsRectItem * wealthIndicator = new QGraphicsRectItem();
    wealthIndicator->setPen(QPen(Qt::transparent));
    wealthIndicator->setRect(0,0,64,64);
    QBrush wealthIndicatorB;
    wealthIndicatorB.setTexture(QPixmap(":/images/Currency.png"));
    wealthIndicator->setBrush(wealthIndicatorB);
    // set position
    int WIxPos = 1064;
    int WIyPos = 40;
    wealthIndicator->setPos(WIxPos,WIyPos);
    // add indicator to the scene
    addItem(wealthIndicator);
    // Create the round number
    QGraphicsTextItem * wealthNumber = new QGraphicsTextItem(QString::number(currency_));
    // set font
    wealthNumber->setDefaultTextColor(Qt::white);
    wealthNumber->setFont(statsFont);
    // set position
    int WNxPos = WIxPos + 64;
    int WNyPos = 40;
    wealthNumber->setPos(WNxPos,WNyPos);
    // add to the scene
    addItem(wealthNumber);


    // main menu button
    Button * menuButton = new Button(QString("Main menu"), 200, 50);
    int lxPos = this->width() - menuButton->boundingRect().width() - 40;
    int lyPos = this->height() - menuButton->boundingRect().height() - 40;
    menuButton->setPos(lxPos, lyPos);
    connect(menuButton, SIGNAL(clicked()), this, SLOT(showMenu()));
    menuButton->setZValue(10);
    addItem(menuButton);

    // upgrade button
    Button * upgradeButton = new Button(QString("Upgrade Tower"), 240, 50);
//    upgradeButton->setRect(QRectF(upgradeButton->boundingRect().topLeft(),
//                            QSizeF(upgradeButton->boundingRect().width() + 20, upgradeButton->boundingRect().height())));

    int uxPos = this->width() - upgradeButton->boundingRect().width() - menuButton->boundingRect().width() - 80;
    int uyPos = this->height() - upgradeButton->boundingRect().height() - 40;
    upgradeButton->setPos(uxPos, uyPos);
    connect(upgradeButton, SIGNAL(clicked()), this, SLOT(enterUpgradeMode()));
    upgradeButton->setZValue(10);
    addItem(upgradeButton);

    // tower build buttons
        // create CSstudent button
    QToolButton* build_CSstudent = new QToolButton();
    // create the icon for the button
    build_CSstudent->setIcon(QIcon(QPixmap(":/images/CStudent1.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_CSstudent->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_CSstudent, SIGNAL(clicked()), this, SLOT(enterBuildCS()));

    // add the button to the control layout
    QGraphicsProxyWidget* CSstudentWidget = addWidget(build_CSstudent);
    controlsLayout->addItem(CSstudentWidget, 1, 1);


        // create TA button
    QToolButton* build_TA = new QToolButton();
    // create the icon for the button
    build_TA->setIcon(QIcon(QPixmap(":/images/TA.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_TA->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_TA, SIGNAL(clicked()), this, SLOT(enterBuildTA()));

    // add the button to the control layout
    QGraphicsProxyWidget* TAWidget = addWidget(build_TA);
    controlsLayout->addItem(TAWidget, 1, 2);


        // create Search Engine button
    QToolButton* build_SE = new QToolButton();
    // create the icon for the button
    build_SE->setIcon(QIcon(QPixmap(":/images/Bing.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_SE->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_SE, SIGNAL(clicked()), this, SLOT(enterBuildSE()));

    // add the button to the control layout
    QGraphicsProxyWidget* SEWidget = addWidget(build_SE);
    controlsLayout->addItem(SEWidget, 2, 1);


        // create Language server button
    QToolButton* build_LS = new QToolButton();
    // create the icon for the button
    build_LS->setIcon(QIcon(QPixmap(":/images/Language_server.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_LS->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_LS, SIGNAL(clicked()), this, SLOT(enterBuildLS()));

    // add the button to the control layout
    QGraphicsProxyWidget* LSWidget = addWidget(build_LS);
    controlsLayout->addItem(LSWidget, 2, 2);


        // create Valgrind button
    QToolButton* build_Valgrind = new QToolButton();
    // create the icon for the button
    build_Valgrind->setIcon(QIcon(QPixmap(":/images/Valgrind.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_Valgrind->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_Valgrind, SIGNAL(clicked()), this, SLOT(enterBuildVal()));

    // add the button to the control layout
    QGraphicsProxyWidget* ValWidget = addWidget(build_Valgrind);
    controlsLayout->addItem(ValWidget, 3, 1);


        // create Comment button
    QToolButton* build_Comment = new QToolButton();
    // create the icon for the button
    build_Comment->setIcon(QIcon(QPixmap(":/images/Comment.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_Comment->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));

    // connect the button with the enterBuildMode function
    connect(build_Comment, SIGNAL(clicked()), this, SLOT(enterBuildCom()));

    // add the button to the control layout
    QGraphicsProxyWidget* ComWidget = addWidget(build_Comment);
    controlsLayout->addItem(ComWidget, 3, 2);


    // add the control layout to the game layout
    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(controlsLayout);
    gameLayout->addItem(form);
}

void Game::createWave(QList<QPoint> path)
{
    //can create an enemy with these 3 lines
    CompilerError* enemy = new CompilerError(CompilerErrorType::Exception, convertCoordinates(path), *this);
    addItem(enemy);
    enemy->startMove();
}

//converting grid matrix coordinates to scene coordinates for the enemie path
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

int Game::getEnemyCount() const
{
    return enemyCount_;
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

void Game::enemyDies()
{
    if(--enemyCount_==0)
    {
        if(level_<20)
        {
            advanceLevel();
            emit waveWon();

        } else emit gameWon();
    }
}

//just testing scene changing
//can be used for other purposes
void Game::keyPressEvent(QKeyEvent* /* unused */)
{
    QList<QPoint> path;
    path << QPoint(7,0) << QPoint(7,1) << QPoint(8,1)<< QPoint(8,5);
    createWave(path);

}

QPointF Game::getSquarePos(int row, int column){

    return mapLayout->itemAt(row,column)->graphicsItem()->scenePos();
}

void Game::showMenu(){
    view->showMenu();
    this->clear();
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

bool Game::buildTower(int row, int column, TowerTypes::TYPES type) {
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();

    // if there is a tower occupying the square, return false
    if (dynamic_cast<Tower*>(widget)) {
        return false;
    }
    else {
        // the square is available for build
        // build tower according to the passed in type
        switch (type) {
        case TowerTypes::CS_Student:
        {
            // create a new tower and add it to the scene
            QGraphicsWidget* tower = this->addWidget(new CS_Student(row, column, nullptr));

            // remove the current square from the grid
            this->removeItem(item->graphicsItem());
            this->mapLayout->removeItem(item);

            // add a tower to the grid at the given possition
            this->mapLayout->addItem(tower, row, column);
            break;
        }
        case TowerTypes::TA:
        {
            // create a new tower and add it to the scene
            QGraphicsWidget* tower = this->addWidget(new TA(row, column, nullptr));

            // remove the current square from the grid
            this->removeItem(item->graphicsItem());
            this->mapLayout->removeItem(item);

            // add a tower to the grid at the given possition
            this->mapLayout->addItem(tower, row, column);
            break;
        }
        default:
            break;
        }
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

void Game::enterBuildCS() {
    mode_ = Modes::build;
    buildType_ = TowerTypes::CS_Student;
}

void Game::enterBuildTA() {
    mode_ = Modes::build;
    buildType_ = TowerTypes::TA;
}

void Game::enterBuildSE() {
    mode_ = Modes::build;
    buildType_ = TowerTypes::SearchEngine;
}

void Game::enterBuildLS() {
    mode_ = Modes::build;
    buildType_ = TowerTypes::LanguageServer;
}

void Game::enterBuildVal() {
    mode_ = Modes::build;
    buildType_ = TowerTypes::Valgrind;
}

void Game::enterBuildCom() {
    mode_ = Modes::build;
    buildType_ = TowerTypes::Comment;
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
