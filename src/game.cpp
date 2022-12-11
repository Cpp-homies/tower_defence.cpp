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
#include "compilererror.h"
#include "memoryerror.h"
#include "runtimeerror.h"
#include "tower.h"
#include "cs_student.h"
#include "search_engine.h"
#include "language_server.h"
#include "ta.h"
#include "valgrind.h"
#include "path.h"
#include "comment.h"

#include <QQueue>
#include <QSet>
#include <QPoint>
#include <QDataStream>
#include <QFile>
#include <QIcon>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QStandardPaths>
#include <QRandomGenerator>


#define BUILD_BUTTON_SIZE 95

// prices of towers can be set here
#define CS_COST 50
#define TA_COST 100
#define SE_COST 250
#define COM_COST 20
#define LS_COST 120
#define VAL_COST 200

// penalty for selling the tower (will be deducted from the tower's total value)
#define SELL_PENALTY 0.3

// the descriptions (tooltips) of buttons can be set here
QString CS_DESCRIPTION("<p><b>CS student:</b><br>"
                       "A student on his quest to complete his CS degree. In his first year he had a hard time identifying memory errors."
                       " He has somewhat mediocre ability at first, but as he advances through his degree,"
                       " he soon become a valuable member of the debugging force.<br>"
                       "<b>Shortcut:</b> C</p>");

QString TA_DESCRIPTION("<p><b>Teaching Assistant:</b><br>"
                       "A Teaching Assistant who is rather experienced at his field."
                       " With his hints, other members within his reach will have higher commit rate."
                       " After having enough work experience, he can be promoted to a Teacher, with higher patches per commit"
                       " and better reach. Teacher also have the ability to spot a major error type that TA cannot.<br>"
                       "<b>Shortcut:</b> T</p>");

QString SE_DESCRIPTION("<p><b>Search Engine:</b><br>"
                       "A crucial tool for any programmer. It was recently found in a study that Search Engine can increase"
                       " a programmer's commit rate up to 10%. Not all Search Engine are equal however,"
                       " some are just worse than others.<br>"
                       "By using a good Search Engine (that start with a G), even an amatuer programmer could solve memory errors.<br>"
                       "<b>Shortcut:</b> E or B</p>");

QString COM_DESCRIPTION("<p><b>Comment:</b><br>"
                        "\"Out of sight, out of mind\", a programmer can choose to comment a block of error away and deal with it later."
                        " But everything has a deadline and the errors need to be dealt with enventually, though."
                        " The errors block will be uncommented after a while and continue its way to wreak havoc on the project.<br>"
                        "<b>Shortcut:</b> O or \\</p>");

QString LS_DESCRIPTION("<p><b>Language Server:</b><br>"
                       "\"The Language Server Protocol (LSP) is an open, JSON-RPC-based protocol for...\". Yeah whatever, it can catch"
                       " some bugs for you, very cool. Language Server can be more effective than humans at handling minor bugs,"
                       " but some special errors can only be manually dealt with. Unless you have a"
                       " really good Language Server, of course.<br>"
                       "<b>Shortcut:</b> L</p>");

QString VAL_DESCRIPTION("<p><b>Valgrind:</b><br>"
                        "A programmer's favorite tool for dealing with memory problems. Valgrind can search far and wide for bugs,"
                        " some might even say that nothing can escape its scope. It does take a bit long for Valgrind to detect"
                        " something, but when it does, that bug is almost as good as dead.<br>"
                        "<b>Shortcut:</b> V</p>");

Game::Game(QObject* parent,int gamemode)
    : QGraphicsScene(parent), gamemode_(gamemode), hitsound1_(this), hitsound2_(this), hitsound3_(this), hitsound4_(this)
{
    // set starting values of health, currency etc
    if (gamemode_ == 0){        // sandbox
        health_ = 100000;
        currency_ = 100000;
    } else if (gamemode == 1) { // easy
        health_ = 100;
        currency_ = 200;
    } else if (gamemode == 2){  // hard
        health_ = 1;
        currency_ = 100;
        initialIncomeMultiplier_ = 1.0;
    } else {                    // sandbox
        gamemode_ = 0;
        health_ = 100000;
        currency_ = 100000;
    }
    level_ = 0;
    score_ = 0;
    enemyCount_ = 0;
    wavesEnemyCount_ = 0;
    spawnedThisWave_ = QAtomicInteger(0);
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
    //connects error signal with a message box
    connect(this,SIGNAL(error(QString)),this, SLOT(showError(QString)));
    connect(this,SIGNAL(wallAction()),this,SLOT(updatePaths()));
    connect(this,SIGNAL(gameLost()),this,SLOT(stopEnemies()));
    if (gamemode == 2){     // update Leaderboard if gamemode is hard
        connect(this,SIGNAL(gameWon()),this,SLOT(updateLeaderboard()));
        connect(this,SIGNAL(gameLost()),this,SLOT(updateLeaderboard()));
    }
    hitsound1_.setSource(QUrl::fromLocalFile(":/sounds/Hitsound1.wav"));
    hitsound2_.setSource(QUrl::fromLocalFile(":/sounds/Hitsound2.wav"));
    hitsound3_.setSource(QUrl::fromLocalFile(":/sounds/Hitsound3.wav"));
    hitsound4_.setSource(QUrl::fromLocalFile(":/sounds/Hitsound4.wav"));
    clicksound_.setSource(QUrl::fromLocalFile(":/sounds/Mouseclick.wav"));
    hitsound1_.setVolume(0.1f);
    hitsound2_.setVolume(0.1f);
    hitsound3_.setVolume(0.1f);
    hitsound4_.setVolume(0.1f);
    clicksound_.setVolume(1.0f);
}
/**
 * @brief Reads the map file and uses the information to construct the map's tile and path objects at the appropriate
 * locations as denoted by the file data.
 */
void Game::createMap(){
    mapLayout = new QGraphicsGridLayout();

    QFile map(":/files/test.tdmap");
    if (map.exists() && map.open(QIODevice::ReadOnly)) {
        QTextStream data(&map);
        int width;
        int height;

        data >> width >> height;

        QList<QList<QString>> matrix;
        for (int j = 0; j < height; ++j) {
            QList<QString> row;
            for (int i = 0; i < width; ++i) {
                QString value;
                data >> value;
                row.push_back(value);
            }
            matrix.push_back(row);
        }
        map.close();
        map_ = matrix;

        // About the map data:
        // 1: Normal square, on which towers can be built.
        // 2: Path tile, on which comment towers can be built. (These are only intermediate paths, that is, neither start nor end points of the path.)
        // A: Starting point for enemies, on which no towers of any kind can be built.
        // B: Ending point for enemies, on which no towers of any kind can be built.

        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                QString value = matrix[j][i];
                if (value == "1") { // Tile for towers
                    Square* tile = new Square(i,j,nullptr);
                    QGraphicsProxyWidget* backgroundTile = addWidget(tile);
                    mapLayout->addItem(backgroundTile,j,i);
                } else if (value == "2" || value == "A" || value == "B") { // Normal Path
                    Path* tile;
                    bool up = false, right = false, down = false, left = false;
                    int neighbors = 0; // Number of neighbors
                    // Checking for adjacent paths
                    auto check = [](QString a) -> bool {return a == "2" || a == "A" || a == "B";};
                    if (j > 0 && check(matrix[j-1][i])) {
                        up = true;
                        neighbors++;
                    }
                    if (j < height - 1 && check(matrix[j+1][i])) {
                        down = true;
                        neighbors++;
                    }
                    if (i > 0 && check(matrix[j][i-1])) {
                        left = true;
                        neighbors++;
                    }
                    if (i < width - 1 && check(matrix[j][i+1])) {
                        right = true;
                        neighbors++;
                    }
                    // Select correct configuration
                    if (value == "2") {
                        // X-split case
                        if (neighbors == 4) {
                            tile = new Path(i, j, XSplit, 0, nullptr);
                        } else if (neighbors == 3) { // T-split cases
                            if (up == false) {
                                tile = new Path(i, j, TSplit, 0, nullptr);
                            } else if (right == false) {
                                tile = new Path(i, j, TSplit, 90, nullptr);
                            } else if (down == false) {
                                tile = new Path(i, j, TSplit, 180, nullptr);
                            } else {
                                tile = new Path(i, j, TSplit, 270, nullptr);
                            }
                        } else if (neighbors == 2) {
                            // Straight Cases
                            if (left == true && right == true) {
                                tile = new Path(i, j, Straight, 90, nullptr);
                            } else if (up == true && down == true) {
                                tile = new Path(i, j, Straight, 0, nullptr);
                            } else if (up == true && right == true) { // Turn cases
                                tile = new Path(i, j, Turn, 0, nullptr);
                            } else if (right == true && down == true) {
                                tile = new Path(i, j, Turn, 90, nullptr);
                            } else if (down == true && left == true) {
                                tile = new Path(i, j, Turn, 180, nullptr);
                            } else {
                                tile = new Path(i, j, Turn, 270, nullptr);
                            }
                        } else {
                            if (left == true || right == true) {
                                tile = new Path(i, j, Straight, 90, nullptr);
                            } else {
                                tile = new Path(i, j, Straight, 0, nullptr);
                            }
                        }

                    } else if (value == "A") {
                        start_ = QPoint(j, i);
                        if (left) {
                            tile = new Path(i, j, Start, 0, nullptr);
                        } else if (up) {
                            tile = new Path(i, j, Start, 90, nullptr);
                        } else if (right) {
                            tile = new Path(i, j, Start, 180, nullptr);
                        } else {
                            tile = new Path(i, j, Start, 270, nullptr);
                        }
                    } else {
                        end_ = QPoint(i, j);
                        if (left) {
                            tile = new Path(i, j, End, 0, nullptr);
                        } else if (up) {
                            tile = new Path(i, j, End, 90, nullptr);
                        } else if (right) {
                            tile = new Path(i, j, End, 180, nullptr);
                        } else {
                            tile = new Path(i, j, End, 270, nullptr);
                        }
                    }
                    QGraphicsProxyWidget* pathTile = addWidget(tile);
                    mapLayout->addItem(pathTile, j, i);
                }
            }
        }
    }
    // set margins to 0
    mapLayout->setContentsMargins(0,0,0,0);

    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(mapLayout);
    gameLayout->addItem(form);
    shortest_path_ = getShortestPath(start_);
}

/**
 * @brief Gets the shortest list of path coordinates which the enemies can take to get from an arbitrary path coordinate to the ending point.
 * Uses the following priority:
 * - If there is at least one unobstructed path between the given start point and the ending point, take the shortest path.
 * - If all paths are obstructed, take the shortest path to the ending point disregarding any obstructions.
 * @param start The path coordinate to start from.
 * @return A list of path coordinates, representing the shortest path enemies can take to get to the ending point.
 */
QList<QPoint> Game::getShortestPath(QPoint start) {
    QList<QPoint> path = BFS(start, false);
    if (!path.empty()) {
        isBlocked_ = false;
        return path;
    }
    isBlocked_ = true;
    return BFS(start, isBlocked_);
}

/**
 * @brief Breadth-first search algorithm, assuming one can only move either vertically or horizontally.
 * @param start The path coordinate to start from.
 * @param blocked Whether to search in obstructed or unobstructed mode.
 * That is:
 * - Obstructed mode: All paths to the end are assumed to be obstructed, so search for the shortest route while assuming there are no obstructions.
 * - Unobstructed mode: It is assumed that there is at least one path to the ending point. Find the shortest route.
 * @return A list of path coordinates, representing the shortest path enemies can take to get to the ending point.
 */
QList<QPoint> Game::BFS(QPoint start, bool blocked) {
    QQueue<QList<QPoint>> to_visit;
    QList<QPoint> initial;
    initial.push_back(start);
    to_visit.enqueue(initial);
    QSet<QPoint> visited;

    int total_rows = map_.size();
    int total_cols = map_[0].size();

    while (!to_visit.empty()) {
        auto top = to_visit.dequeue();
        QPoint cursor = top.last();
        visited.insert(cursor);
        QList<QPoint> all_neighbors;
        all_neighbors.append(cursor + QPoint(0, 1));
        all_neighbors.append(cursor + QPoint(0, -1));
        all_neighbors.append(cursor + QPoint(1, 0));
        all_neighbors.append(cursor + QPoint(-1, 0));
        QList<QPoint> neighbors;
        // Get in-bound neighbors that have not been visited yet
        for (auto i : all_neighbors) {
            if (i.x() < total_cols && i.x() >= 0 && i.y() < total_rows && i.y() >= 0 && (blocked || !isComment(i.x(), i.y())) && isPath(i.x(), i.y()) && !visited.contains(i)) {
                neighbors.append(i);
            }
        }
        // Queue new elements
        for (auto i : neighbors) {
            QList<QPoint> new_elem;
            new_elem.append(i);
            if (i == end_) {
                return top + new_elem;
            }
            to_visit.append(top + new_elem);
        }
    }
    return QList<QPoint>();
}

/**
 * @brief Plays a random keyboard noise (hitsound) from the four available hitsounds.
 */
void Game::playHitsound() {
    int choice = QRandomGenerator::global()->bounded(0,4);
    switch (choice){
        case 0:
            hitsound1_.play();
        break;
        case 1:
            hitsound2_.play();
        break;
        case 2:
            hitsound3_.play();
        break;
        case 3:
            hitsound4_.play();
        break;
    }
}

/**
 * @brief Plays a mouseclick sound.
 */
void Game::playClicksound() {
    clicksound_.play();
}

/**
 * @brief Creates the game controls and hooks them up to their actions. Places all the controls into the gameLayout.
 * 
 */
void Game::createGameControls()
{
    //for testing purposes, all Layouts will auto adjust the size
    controlsLayout = new QGraphicsGridLayout();

    // set spacings
    controlsLayout->setContentsMargins(80,230,0,0);
    controlsLayout->setSpacing(50);

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
    roundDisplay = new QGraphicsTextItem(QString::number(level_));
    // set font
    roundDisplay->setDefaultTextColor(Qt::white);
    roundDisplay->setFont(statsFont);
    // set position
    int RNxPos = RIxPos + roundIndicator->boundingRect().width();
    int RNyPos = RIyPos;
    roundDisplay->setPos(RNxPos,RNyPos);
    // add to the scene
    addItem(roundDisplay);

        // display score
    // Create the roundnumber indicator
    QGraphicsTextItem * scoreIndicator = new QGraphicsTextItem(QString("score"));
    // set font
    scoreIndicator->setDefaultTextColor(Qt::white);
    scoreIndicator->setFont(roundIndicatorFont);
    // set position
    int SIxPos = 1000;
    int SIyPos = 40;
    scoreIndicator->setPos(SIxPos,SIyPos);
    // add indicator to the scene
    addItem(scoreIndicator);
    // Create the round number
    scoreDisplay = new QGraphicsTextItem(QString::number(score_));
    // set font
    scoreDisplay->setDefaultTextColor(Qt::white);
    scoreDisplay->setFont(statsFont);
    // set position
    int SNxPos = SIxPos + scoreIndicator->boundingRect().width();
    int SNyPos = SIyPos;
    scoreDisplay->setPos(SNxPos,SNyPos);
    // add to the scene
    addItem(scoreDisplay);

        // display health
    // Create the health indicator
    QGraphicsRectItem * healthIndicator = new QGraphicsRectItem();
    healthIndicator->setPen(QPen(Qt::transparent));
    healthIndicator->setRect(0,0,64,64);
    QBrush healthIndicatorB;
    healthIndicatorB.setTexture(QPixmap(":/images/hp_icon.png"));
    healthIndicator->setBrush(healthIndicatorB);
    // set position
    int HIxPos = 744;
    int HIyPos = 120;
    // QGraphicsProxyWidget* healthIndicatorWidget = addWidget(healthIndicator);
    healthIndicator->setPos(HIxPos,HIyPos);
    // add indicator to the scene
    addItem(healthIndicator);
    // Create the round number
    healthDisplay = new QGraphicsTextItem(QString::number(health_));
    // set font
    healthDisplay->setDefaultTextColor(Qt::white);
    healthDisplay->setFont(statsFont);
    // set position
    int HNxPos = HIxPos + 64;
    int HNyPos = HIyPos;
    healthDisplay->setPos(HNxPos,HNyPos);
    // add to the scene
    addItem(healthDisplay);

        // display wealth
    // Create the wealth indicator
    QGraphicsRectItem * wealthIndicator = new QGraphicsRectItem();
    wealthIndicator->setPen(QPen(Qt::transparent));
    wealthIndicator->setRect(0,0,64,64);
    QBrush wealthIndicatorB;
    wealthIndicatorB.setTexture(QPixmap(":/images/Currency.png"));
    wealthIndicator->setBrush(wealthIndicatorB);
    // set position
    int WIxPos = 1000;
    int WIyPos = HIyPos;
    wealthIndicator->setPos(WIxPos,WIyPos);
    // add indicator to the scene
    addItem(wealthIndicator);
    // Create the round number
    wealthDisplay = new QGraphicsTextItem(QString::number(currency_));
    // set font
    wealthDisplay->setDefaultTextColor(Qt::white);
    wealthDisplay->setFont(statsFont);
    // set position
    int WNxPos = WIxPos + 64;
    int WNyPos = WIyPos;
    wealthDisplay->setPos(WNxPos,WNyPos);
    // add to the scene
    addItem(wealthDisplay);


    // main menu button
    Button * menuButton = new Button(QString("Main menu"), 240, 50, Qt::red);
    int lxPos = this->width() - menuButton->boundingRect().width() - 35;
    int lyPos = this->height() - menuButton->boundingRect().height() - 35;
    menuButton->setPos(lxPos, lyPos);
    connect(menuButton, SIGNAL(clicked()), this, SLOT(showMenu()));
    menuButton->setZValue(10);
    addItem(menuButton);

    // upgrade button
    upgradeButton = new Button(QString("Upgrade Tower"), 240, 50);

    int uxPos = this->width() - upgradeButton->boundingRect().width() - menuButton->boundingRect().width() - 50;
    int uyPos = lyPos;
    upgradeButton->setPos(uxPos, uyPos);
    connect(upgradeButton, SIGNAL(clicked()), this, SLOT(enterUpgradeMode()));
    upgradeButton->setZValue(10);
    addItem(upgradeButton);
    // upgrade price
    QLabel * upgradeCostLabel = new QLabel("Upgrade cost: " + QString::number(500));
    upgradeCostLabel->setStyleSheet(buildButtonStylesheet);
    // set font
    QFont font("Roboto", 14);
    upgradeCostLabel->setAlignment(Qt::AlignCenter);
    upgradeCostLabel->setFont(font);
    // add the Label to the layout
    QGraphicsProxyWidget* upgradeCostWidget = new QGraphicsProxyWidget();
    upgradeCostWidget->setWidget(upgradeCostLabel);
    upgradeCostWidget->setMaximumHeight(18);
    upgradeCostWidget->setMinimumWidth(upgradeButton->boundingRect().width());
    upgradeCostWidget->setPos(uxPos,uyPos + upgradeButton->boundingRect().height());
    upgradeCostWidget->setZValue(11);
    addItem(upgradeCostWidget);

    // sell tower button
    sellButton = new Button(QString("Sell Tower"), 240, 50);

    int sxPos = uxPos;
    int syPos = this->height() - sellButton->boundingRect().height() - 100;
    sellButton->setPos(sxPos, syPos);
    connect(sellButton, SIGNAL(clicked()), this, SLOT(enterSellMode()));
    sellButton->setZValue(10);
    addItem(sellButton);

    // next round button
    nextRoundButton = new Button(QString("Start game"), 240, 50);

    int nxPos = lxPos;
    int nyPos = syPos;
    nextRoundButton->setPos(nxPos, nyPos);
    connect(nextRoundButton, SIGNAL(clicked()), this, SLOT(startGame()));
    nextRoundButton->setZValue(10);
    addItem(nextRoundButton);


    // tower build buttons
    buildButtonStylesheet = "background-color: white; border: 1px solid white; text-align: center";

        // create CSstudent button
    build_CSstudent = new QToolButton();
    // set the tooltip description for the button
    build_CSstudent->setToolTip(CS_DESCRIPTION);
    // create the icon for the button
    build_CSstudent->setIcon(QIcon(QPixmap(":/images/CStudent1.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_CSstudent->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_CSstudent->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_CSstudent, SIGNAL(clicked()), this, SLOT(enterBuildCS()));

    QGraphicsLinearLayout* cstudentButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    cstudentButtonAndPrice->setSpacing(0);

        // add the name, button and price to the control layout
    addLabelTo(cstudentButtonAndPrice, "CStudent", buildButtonStylesheet);
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* cstWidget = addWidget(build_CSstudent);
    cstudentButtonAndPrice->addItem(cstWidget);
    // add CostVisual to the linearlayout ButtonAndPrice
    addLabelTo(cstudentButtonAndPrice, QString::number(CS_COST), buildButtonStylesheet);
    controlsLayout->addItem(cstudentButtonAndPrice, 1, 1);


        // create TA button
    build_TA = new QToolButton();
    // set the tooltip description for the button
    build_TA->setToolTip(TA_DESCRIPTION);
    // create the icon for the button
    build_TA->setIcon(QIcon(QPixmap(":/images/TA.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_TA->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_TA->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_TA, SIGNAL(clicked()), this, SLOT(enterBuildTA()));

    // Add price visuals
    QGraphicsLinearLayout* teachersassistantButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    teachersassistantButtonAndPrice->setSpacing(0);

    // add the name, button and price to the control layout
    addLabelTo(teachersassistantButtonAndPrice, "TA", buildButtonStylesheet);
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* teaWidget = addWidget(build_TA);
    teachersassistantButtonAndPrice->addItem(teaWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    addLabelTo(teachersassistantButtonAndPrice, QString::number(TA_COST), buildButtonStylesheet);
    controlsLayout->addItem(teachersassistantButtonAndPrice, 1, 2);


        // create Search Engine button
    build_SE = new QToolButton();
    // set the tooltip description for the button
    build_SE->setToolTip(SE_DESCRIPTION);
    // create the icon for the button
    build_SE->setIcon(QIcon(QPixmap(":/images/Bing.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_SE->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_SE->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_SE, SIGNAL(clicked()), this, SLOT(enterBuildSE()));

    // Add price visuals
    QGraphicsLinearLayout* searchengineButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    searchengineButtonAndPrice->setSpacing(0);

    // add the name, button and price to the control layout
    addLabelTo(searchengineButtonAndPrice, "Search", buildButtonStylesheet);
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* seaWidget = addWidget(build_SE);
    searchengineButtonAndPrice->addItem(seaWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    addLabelTo(searchengineButtonAndPrice, QString::number(SE_COST), buildButtonStylesheet);
    controlsLayout->addItem(searchengineButtonAndPrice, 1, 3);


        // create Language server button
    build_LS = new QToolButton();
    // set the tooltip description for the button
    build_LS->setToolTip(LS_DESCRIPTION);
    // create the icon for the button
    build_LS->setIcon(QIcon(QPixmap(":/images/Language_server.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_LS->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_LS->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_LS, SIGNAL(clicked()), this, SLOT(enterBuildLS()));

    // Add price visuals
    QGraphicsLinearLayout* languageserverButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    languageserverButtonAndPrice->setSpacing(0);

    // add the name, button and price to the control layout
    addLabelTo(languageserverButtonAndPrice, "LServer", buildButtonStylesheet);
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* lanWidget = addWidget(build_LS);
    languageserverButtonAndPrice->addItem(lanWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    addLabelTo(languageserverButtonAndPrice, QString::number(LS_COST), buildButtonStylesheet);
    controlsLayout->addItem(languageserverButtonAndPrice, 2, 1);


        // create Valgrind button
    build_Valgrind = new QToolButton();
    // set the tooltip description for the button
    build_Valgrind->setToolTip(VAL_DESCRIPTION);
    // create the icon for the button
    build_Valgrind->setIcon(QIcon(QPixmap(":/images/Valgrind.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_Valgrind->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_Valgrind->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_Valgrind, SIGNAL(clicked()), this, SLOT(enterBuildVal()));

    // Add price visuals
    QGraphicsLinearLayout* valgrindButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    valgrindButtonAndPrice->setSpacing(0);

    // add the name, button and price to the control layout
    addLabelTo(valgrindButtonAndPrice, "Valgrind", buildButtonStylesheet);
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* valWidget = addWidget(build_Valgrind);
    valgrindButtonAndPrice->addItem(valWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    addLabelTo(valgrindButtonAndPrice, QString::number(VAL_COST), buildButtonStylesheet);
    controlsLayout->addItem(valgrindButtonAndPrice, 2, 2);



        // create Comment button
    build_Comment = new QToolButton();
    // set the tooltip description for the button
    build_Comment->setToolTip(COM_DESCRIPTION);
    // create the icon for the button
    build_Comment->setIcon(QIcon(QPixmap(":/images/Comment.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_Comment->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_Comment->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_Comment, SIGNAL(clicked()), this, SLOT(enterBuildCom()));

    // Add price visuals
    QGraphicsLinearLayout* commentButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    commentButtonAndPrice->setSpacing(0);

    // add the name, button and price to the control layout
    addLabelTo(commentButtonAndPrice, "Comment", buildButtonStylesheet);
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* comWidget = addWidget(build_Comment);
    commentButtonAndPrice->addItem(comWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    addLabelTo(commentButtonAndPrice, QString::number(COM_COST), buildButtonStylesheet);
    controlsLayout->addItem(commentButtonAndPrice, 2, 3);


    // add the control layout to the game layout
    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(controlsLayout);
    gameLayout->addItem(form);
}

/**
 * @brief Creates a wave of enemies for one level according to the description in wave.txt.
 * The intervals between enemies and types of enemies is implemented using QTimer timers.
 * 
 */
void Game::createWave()
{

    //to mark a start of wave creation
    bool flag = true;
    // a buffer variable to hold a timer
    QPointer<QTimer> timerBuffer;
    //for every entry in one line from wave.txt
    for (QString typeOfEnemy: waves_[level_])
    {
        QTextStream stream(&typeOfEnemy);
        int amount=0;
        int type=0;
        int delay=0;
        stream>>amount>>type>>delay;
        if (stream.status()==QTextStream::Ok && delay>=0)
        {
            //timer for this enemy type
            QTimer* timer = new QTimer(this);
            //timer to initiate the next enemy type after the current ones have spawned
            QTimer* nextEnemiesTimer = new QTimer(this);
            nextEnemiesTimer->setSingleShot(true);
            nextEnemiesTimer->setInterval((amount+1)*delay);
            QTimer* addEnemyAmountTimer = new QTimer(this);
            addEnemyAmountTimer->setSingleShot(true);
            addEnemyAmountTimer->setInterval(amount*delay);
            addEnemyAmountTimer->callOnTimeout([this, amount](){this->addSpawnedEnemies(amount);});
            //after every timeout spawn an enemy
            timer->callOnTimeout([this, type](){this->spawnEnemy(type);});
            timer->setInterval(delay);
            //stop current timer when all enemies are spawned
            connect(nextEnemiesTimer, SIGNAL(timeout()), timer, SLOT(stop()));

            if(flag)
            {
                flag = false;
                //start both immediately since there are no enemies
                timer->start();
                nextEnemiesTimer->start((amount+1)*delay);
                addEnemyAmountTimer->start();
            } else
            {
                //wait till the last enemy type has stopped spawning
                connect(timerBuffer, SIGNAL(timeout()),timer, SLOT(start()));
                connect(timerBuffer, SIGNAL(timeout()),nextEnemiesTimer, SLOT(start()) );
                connect(timerBuffer, SIGNAL(timeout()),addEnemyAmountTimer, SLOT(start()));
            }

            //save a timer to the buffer to connect it during the next loop
            timerBuffer = nextEnemiesTimer;
            //how many enemies will be created in this loop
            wavesEnemyCount_+=amount;
        }

    }
    advanceLevel();







}

/**
 * @brief Is called to spawn an enemy of the specified type. (Enum is located in enemy.h)
 * Calls the slot addEnemy which will create the enemy and adds it to the scene.
 * 
 * @param type One of the values of the enum EnemyType
 */
void Game::spawnEnemy(int type)
{
    QList<QPoint> shortestPath = getShortestPath(start_);
    QList<QPointF> convertedPath = convertCoordinates(shortestPath);
    if(type==1 || type==2)
    {
        CompilerError* enemy = new CompilerError(static_cast<CompilerErrorType>(type), convertedPath, shortestPath);
        addEnemy((Enemy*)enemy);

    }
    else if(type==3 || type==4 || type==5 || type==6)
    {
        MemoryError* enemy = new MemoryError(static_cast<MemoryErrorType>(type), convertedPath, shortestPath);
        addEnemy((Enemy*)enemy);

    }
    else if(type==7)
    {
        RuntimeError* enemy = new RuntimeError(static_cast<RuntimeErrorType>(type), convertedPath, shortestPath);
        addEnemy((Enemy*)enemy);

    }

}

/**
 * @brief Is called when the game is lost or won.
 * Reads the data from the leaderboard file (or creates it), compares the current score to the leaderboard 
 * and asks the name of the player if it is in the top10. Updates the leaderboard accordingly.
 * Uses QInputDialog for communicating with the player.
 * 
 */
void Game::updateLeaderboard()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"_leaderboard.dat");

    //read the leaderboard file or create it
    if(!file.open(QIODevice::ReadWrite))
    {

        emit error("Could not open leaderboard file: "+file.errorString());
        return;
    }
    QDataStream stream(&file);
    //if empty
    if(stream.atEnd())
    {
        bool ok;
        QString text = QInputDialog::getText(nullptr, tr("QInputDialog::getText()"),
                                             tr("You got a TOP10 score, enter name:"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !text.isEmpty())
        {
            stream<<text<<score_;

        }

    }
    //if not empty
    else
    {
        QList<QPair<QString,int>> fullLeaderboard;
        while(!stream.atEnd())
        {
            QString name;
            int score;
            stream>>name>>score;
            fullLeaderboard<<QPair<QString,int>(name,score);
        }
        if(fullLeaderboard.length()>=10)
        {
            auto min = std::min_element(fullLeaderboard.begin(),fullLeaderboard.end(),[](QPair<QString,int> a,QPair<QString,int> b)
            {
                    return a.second < b.second;
            });
            if(score_ <= min->second)
            {
                QMessageBox::information(qobject_cast<QWidget*>(this), tr("Weak sauce"),
                             "You didn't get a TOP10 score, try again.");
                file.close();
                return;
            }
            else
            {
                bool ok;
                QString text = QInputDialog::getText(nullptr, tr("Yay!"),
                                                     tr("You got a TOP10 score, enter name:"), QLineEdit::Normal,
                                                     QDir::home().dirName(), &ok);


                *min = QPair<QString,int>(text,score_);


            }
        }
        else
        {
            bool ok;
            QString text = QInputDialog::getText(nullptr, tr("Yay!"),
                                                 tr("You got a TOP10 score, enter name:"), QLineEdit::Normal,
                                                 QDir::home().dirName(), &ok);
            if (ok && !text.isEmpty()) fullLeaderboard<<QPair<QString,int>(text,score_);

        }
        std::sort(fullLeaderboard.begin(),fullLeaderboard.end(),[](QPair<QString,int> a,QPair<QString,int> b)
        {
            return a.second>b.second;
        });
        file.seek(0);
        foreach (auto score, fullLeaderboard) {
            stream<<score.first<<score.second;
        }

    }
    file.close();
    MainView* view_ = qobject_cast<MainView*>(this->parent());

    view_->showLeaderboard();
    mode_ = Modes::exit;
    this->deleteLater();
}

/**
 * @brief Displays an error message in a QMessageBox
 * 
 * @param message The message in QString
 */
void Game::showError(QString message)
{
    QMessageBox::warning(qobject_cast<QWidget*>(this), tr("Error"),
                 message);

}

/**
 * @brief Converts grid matrix coordinates to scene coordinates for the enemy path.
 * 
 * @param path The grid representation of the enemy path.
 * @return QList<QPointF> 
 */

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

/**
 * @brief Reads the wave.txt file which specifies the levels of the games.
 * README.txt can be found in /src/files.
 * Adds waves to the QList<QStringList> waves_ member which will be used for waves' creation and
 * the finalLevel_ member.
 * 
 * 
 */
void Game::readWaveFile()
{
    QFile file(":/files/waves.txt");
    if(!file.exists())
    {
        emit error("wave.txt not found");
        return;

    }
    if(!file.open(QIODevice::ReadOnly))
    {
        emit error(file.errorString());
        return;
    }
    QTextStream stream(&file);
    if(stream.atEnd())
    {

        emit error("wave.txt is empty");
        return;
    }
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList wave = line.split(';', Qt::SkipEmptyParts);
        waves_<<wave;
    }
    file.close();
    finalLevel_=waves_.length();
}


/**
 * @brief Performs a check if the game is lost.
 * 
 * @return true 
 * @return false 
 */
bool Game::isLost() const{
    return health_<=0;
}

/**
 * @brief Performs a check if the game is won.
 * 
 * @return true 
 * @return false 
 */
bool Game::isWon() const
{
    return  health_>0 && level_==finalLevel_ ;
}

/**
 * @brief Performs a check if the level is won.
 * 
 * @return true 
 * @return false 
 */
bool Game::isWaveWon()
{

    if(spawnedThisWave_ == wavesEnemyCount_ && enemyCount_ == 0)
    {
        spawnedThisWave_ = QAtomicInteger(0);
        wavesEnemyCount_ = 0;
        return true;
    } else return false;

}

/**
 * @brief Returns the health of the player.
 * 
 * @return int 
 */
int Game::getHealth() const {
    return health_;
}

/**
 * @brief Returns the amount of currency the player has.
 * 
 * @return int 
 */
int Game::getCurrency() const {
    return currency_;
}
/**
 * @brief Returns the amount of currently active enemies.
 * 
 * @return int 
 */
int Game::getEnemyCount() const
{
    return enemyCount_;
}
/**
 * @brief Returns the currently active level.
 * 
 * @return int 
 */
int Game::getLevel() const {
    return level_;
}
/**
 * @brief Returns the current score.
 * 
 * @return int 
 */
int Game::getScore() const {
    return score_;
}

Modes::MODES Game::getMode() const {
    return mode_;
}

TowerTypes::TYPES Game::getBuildType() const {
    return buildType_;
}

/**
 * @brief Is called when the player takes damage from an enemy. 
 * 
 * @param dHealth The amount of damage dealt by an enemy.
 */
void Game::takeDamage (int dHealth) {
    health_-=dHealth;
    healthDisplay->setPlainText(QString::number(health_));
    Enemy* enemy = qobject_cast<Enemy*>(sender());
    activeEnemies_.removeOne(enemy);
    activeEnemies_.squeeze();
    updateEnemyCount();
    enemy->deleteLater();
    if(isLost())
    {
        emit gameLost();
        disconnect(this,SIGNAL(gameLost()),nullptr,nullptr);
        return;
    }
    if(isWaveWon())
    {
        if(isWon())
        {
           emit gameWon();
           disconnect(this, SIGNAL(gameWon()),nullptr,nullptr);
        } else   createWave();

    }
}

/**
 * @brief Changes the currency amount.
 * 
 * @param dCurrency The amount to be added to currency.
 */
void Game::changeCurrency (int dCurrency) {
    currency_+=dCurrency;
    wealthDisplay->setPlainText(QString::number(currency_));
}

/**
 * @brief Changes the score.
 * 
 * @param dPoints The amount of points to be added.
 */
void Game::changeScore (int dPoints) {
    score_+=dPoints;
    scoreDisplay->setPlainText(QString::number(score_));
}

/**
 * @brief Set the game's mode to the given mode.
 * @param m an enum that represents a certain game mode
 */
void Game::setMode(Modes::MODES m) {
    mode_ = m;
}
/**
 * @brief Advances the level by 1.
 * 
 */
void Game::advanceLevel () {
    level_++;
    roundDisplay->setPlainText(QString::number(level_));
    incomeMultiplier_ = initialIncomeMultiplier_ * pow(0.90, level_); // at level 20 ~0.12 * the initialMultiplier
}

/**
 * @brief Is called when an enemy dies.
 * Updates the activeEnemies_ list and performs level and game ending checks.
 * 
 * @param value The value that will be added the score and currency.
 */
void Game::enemyDies(int value)
{
    changeScore(value);
    changeCurrency( (int)ceil(value * incomeMultiplier_) );
    Enemy* enemy = qobject_cast<Enemy*>(sender());
    activeEnemies_.removeOne(enemy);
    activeEnemies_.squeeze();
    updateEnemyCount();
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(500);
    timer->callOnTimeout([this](){
        if(this->isWaveWon())
        {
            if(isWon())
            {
                emit this->gameWon();
                disconnect(this, SIGNAL(gameWon()),nullptr,nullptr);
            } else this->createWave() ;
        }
    });
    timer->start();

}

/**
 * @brief Adds an enemy to the game.
 * Connects all signals and slots relevant to the enemy. 
 * 
 * @param enemy The enemy to be added.
 */
void Game::addEnemy(Enemy* enemy)
{
    addItem(enemy);

    connect(enemy,SIGNAL(enemyDies(int)),this,SLOT(enemyDies(int)));
    connect(enemy,SIGNAL(addedEnemy(Enemy*)),this,SLOT(addEnemy(Enemy*)));
    connect(enemy,SIGNAL(dealsDamage(int)),this,SLOT(takeDamage(int)));
    connect(enemy,SIGNAL(enemyDies(int)),this,SLOT(updateEnemyCount()));
    connect(enemy,SIGNAL(addedEnemy(Enemy*)),this,SLOT(updateEnemyCount()));
    activeEnemies_<<enemy;
    QPoint nextLocation = enemy->getNextLocation();
    if(!isComment(nextLocation.x(),nextLocation.y()))
    {
        enemy->startMove();
    }

    updateEnemyCount();

}

/**
 * @brief Initiates the correct functions related to the key pressed.
 * 
 * @param event The event. Mainly it contains the character which corresponds to the key which was pressed. 
 */
void Game::keyPressEvent(QKeyEvent* event)
{
    auto key = event->key();
    if (key == ' '){
        startGame();
    } else if (key == 'U'){
        enterUpgradeMode();
    } else if (key == 'S'){
        enterSellMode();
    } else if (key == 'C'){
        enterBuildCS();
    } else if (key == 'T'){
        enterBuildTA();
    } else if (key == 'E' || key == 'B'){
        enterBuildSE();
    } else if (key == 'L'){
        enterBuildLS();
    } else if (key == 'V'){
        enterBuildVal();
    } else if (key == 'O' || key == '\''){
        enterBuildCom();
    }

}

/**
 * @brief Returns the scene location of a square in the grid.
 * 
 * @param row The row in the grid.
 * @param column The column in the grid.
 * @return QPointF 
 */
QPointF Game::getSquarePos(int row, int column){

    return mapLayout->itemAt(row,column)->graphicsItem()->scenePos();
}

/**
 * @brief Exits the game into the main menu.
 * 
 */
void Game::showMenu(){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->showMenu();
    mode_ = Modes::exit;// added to handle object deletion
    this->deleteLater();

}

/**
 * @brief Build a tower of a certain type at the given coordinates on the grid map.
 * @param row the row of the tower in the game's grid map
 * @param column the column of the tower in the game's grid map
 * @param type an enum that represent certain tower types
 * @return true if the built was successful, false otherwise
 */
bool Game::buildTower(int row, int column, TowerTypes::TYPES type) {
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();
    resetButtonHighlights();

    // if the type is comment
    if (type == TowerTypes::Comment) {
        // check if this is a path and whether there is already a comment
        if (isPath(row, column) && !dynamic_cast<Comment*>(widget) && !isEnemy(row, column) && !isPathEnd(row, column)) {
            // if it is available, start building the comment
            // first, check if the player have enough money or not
            // if yes, build the tower
            if (this->currency_ >= COM_COST) {
                // create a new tower and add it to the scene
                Path* oldPath = dynamic_cast<Path*>(widget);
                Comment* newComment = new Comment(column, row, 10000, oldPath, nullptr);
                QGraphicsWidget* comment = this->addWidget(newComment);



                // remove the current square from the grid
                this->mapLayout->removeItem(item);

                // add the comment to the grid at the given possition
                this->mapLayout->addItem(comment, row, column);

                // Hide the attack ranges of all other towers
                hideAllAttackAreasExcept(QPointF(row,column));

                // deduct the cost of the comment from player's money
                changeCurrency(-COM_COST);
            
                // Notify enemeies of path change
                emit wallAction();
                
                return true;
            }
            else {
                // not enough money
                return false;
            }
        }
        else {
            return false;
        }

    }
    // if there is a tower or a path occupying the square, return false
    else if (dynamic_cast<Tower*>(widget) || isPath(row, column)) {
        return false;
    }
    else {
        // the square is available for build
        // build tower according to the passed in type
        switch (type) {
        case TowerTypes::CS_Student:
        {
            // first, check if the player have enough money or not
            // if yes, build the tower
            if (this->currency_ >= CS_COST) {
                // create a new tower and add it to the scene
                Tower* newTower = new CS_Student(row, column, nullptr);
                QGraphicsWidget* tower = this->addWidget(newTower);

                // remove the current square from the grid
                this->removeItem(item->graphicsItem());
                this->mapLayout->removeItem(item);

                // add a tower to the grid at the given possition
                this->mapLayout->addItem(tower, row, column);

                // Hide the attack ranges of all other towers
                hideAllAttackAreasExcept(QPointF(row,column));

                // deduct the cost of the tower from player's money
                changeCurrency(-CS_COST);

                // add the cost of the tower to tower's total cost
                newTower->addCost(CS_COST);

                // update the tower's description
                newTower->updateDescription();
            }
            else {
                // not enough money
                return false;
            }
            break;
        }
        case TowerTypes::TA:
        {
            // first, check if the player have enough money or not
            // if yes, build the tower
            if (this->currency_ >= TA_COST) {
                // create a new tower and add it to the scene
                Tower* newTower = new TA(row, column, nullptr);
                QGraphicsWidget* tower = this->addWidget(newTower);

                // remove the current square from the grid
                this->removeItem(item->graphicsItem());
                this->mapLayout->removeItem(item);

                // add a tower to the grid at the given possition
                this->mapLayout->addItem(tower, row, column);

                // Hide the attack ranges of all other towers
                hideAllAttackAreasExcept(QPointF(row,column));

                // deduct the cost of the tower from player's money
                changeCurrency(-TA_COST);

                // add the cost of the tower to tower's total cost
                newTower->addCost(TA_COST);

                // update the tower's description
                newTower->updateDescription();
            }
            else {
                // not enough money
                return false;
            }
            break;
        }
        case TowerTypes::SearchEngine:
        {
            // first, check if the player have enough money or not
            // if yes, build the tower
            if (this->currency_ >= SE_COST) {
                // create a new tower and add it to the scene
                Tower* newTower = new Search_Engine(row, column, nullptr);
                QGraphicsWidget* tower = this->addWidget(newTower);

                // remove the current square from the grid
                this->removeItem(item->graphicsItem());
                this->mapLayout->removeItem(item);

                // add a tower to the grid at the given possition
                this->mapLayout->addItem(tower, row, column);

                // Hide the attack ranges of all other towers
                hideAllAttackAreasExcept(QPointF(row,column));

                // deduct the cost of the tower from player's money
                changeCurrency(-SE_COST);

                // add the cost of the tower to tower's total cost
                newTower->addCost(SE_COST);

                // update the tower's description
                newTower->updateDescription();
            }
            else {
                // not enough money
                return false;
            }
            break;
        }
        case TowerTypes::LanguageServer:
        {
            // first, check if the player have enough money or not
            // if yes, build the tower
            if (this->currency_ >= LS_COST) {
                // create a new tower and add it to the scene
                Tower* newTower = new Language_Server(row, column, nullptr);
                QGraphicsWidget* tower = this->addWidget(newTower);

                // remove the current square from the grid
                this->removeItem(item->graphicsItem());
                this->mapLayout->removeItem(item);

                // add a tower to the grid at the given possition
                this->mapLayout->addItem(tower, row, column);

                // Hide the attack ranges of all other towers
                hideAllAttackAreasExcept(QPointF(row,column));

                // deduct the cost of the tower from player's money
                changeCurrency(-LS_COST);

                // add the cost of the tower to tower's total cost
                newTower->addCost(LS_COST);

                // update the tower's description
                newTower->updateDescription();
            }
            else {
                // not enough money
                return false;
            }
            break;
        }
        case TowerTypes::Valgrind:
        {
            // first, check if the player have enough money or not
            // if yes, build the tower
            if (this->currency_ >= VAL_COST) {
                // create a new tower and add it to the scene
                Tower* newTower = new Valgrind(row, column, nullptr);
                QGraphicsWidget* tower = this->addWidget(newTower);

                // remove the current square from the grid
                this->removeItem(item->graphicsItem());
                this->mapLayout->removeItem(item);

                // add a tower to the grid at the given possition
                this->mapLayout->addItem(tower, row, column);

                // Hide the attack ranges of all other towers
                hideAllAttackAreasExcept(QPointF(row,column));

                // deduct the cost of the tower from player's money
                changeCurrency(-VAL_COST);

                // add the cost of the tower to tower's total cost
                newTower->addCost(VAL_COST);

                // update the tower's description
                newTower->updateDescription();
            }
            else {
                // not enough money
                return false;
            }
            break;
        }
        default:
            break;
        }
        coordsOfTowers.append(QPointF(row, column));
        return true;
    }

}

/**
 * @brief Sell the tower at the given position on the grid map, which destroy it and give back some money to the player.
 * @param row the row of the tower in the game's grid map
 * @param column the column of the tower in the game's grid map
 * @return true if the sale was successful, false otherwise
 */
bool Game::sellTower(int row, int column) {
    // change sell color back to green
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    sellButton->setBrush(brush);

    // get the item at the current square
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();
    resetButtonHighlights();
    Tower* tower = dynamic_cast<Tower*>(widget);
    Comment* comment = dynamic_cast<Comment*>(widget);

    // if there is no tower occupying the square, return false
    if (!tower && !comment) {
        return false;
    }
    else if (tower) {
        // get the total cost of the tower
        int totalCost = tower->getTotalCost();

        // create a new square and add it to the scene
        Square* newSquare = new Square(column, row, nullptr);
        QGraphicsWidget* square = this->addWidget(newSquare);

        // remove the current tower from the grid
        this->removeItem(item->graphicsItem());
        this->mapLayout->removeItem(item);

        // add a square to the grid at the given possition
        this->mapLayout->addItem(square, row, column);

        // add the money from selling the tower to player's money
        // current cost penalty for selling a tower is 30%
        changeCurrency((totalCost * (1 - SELL_PENALTY)));

        coordsOfTowers.removeOne(QPointF(row, column));
        coordsOfTowers.squeeze();
        return true;
    } else {
        deleteComment(row, column);
        changeCurrency((COM_COST * (1 - SELL_PENALTY)));
        coordsOfTowers.removeOne(QPointF(row, column));
        coordsOfTowers.squeeze();
        return true;
    }
}

/**
 * @brief The enemy's method of destroying a comment tower. Breaks the comment tower after its duration value passes.
 * @param row The row of the comment tower.
 * @param column The column of the comment tower.
 */
void Game::breakComment(int row, int column) {
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();
    Comment* comment = dynamic_cast<Comment*>(widget);
    comment->startTimer();
}

/**
 * @brief Deletes a comment tower at a given location.
 * @param row The row of the comment tower.
 * @param column The column of the comment tower.
 */
void Game::deleteComment(int row, int column) {
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();
    Comment* comment = dynamic_cast<Comment*>(widget);

    Path* oldPath = comment->getOld();
    PathType oldType = oldPath->getType();
    int oldRotation = oldPath->getRotation();
    Path* newPath = new Path(column, row, oldType, oldRotation, nullptr, nullptr);
    QGraphicsWidget* path = this->addWidget(newPath);
    this->removeItem(item->graphicsItem());
    this->mapLayout->removeItem(item);
    this->mapLayout->addItem(path, row, column);
    comment->deleteLater();
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(100);
    timer->callOnTimeout([this](){emit wallAction();});
    timer->start();
}

/**
 * @brief Get a widget at the given position on the game's grid map.
 * @param row a row in the game's grid map
 * @param column a column in the game's grid map
 * @return a pointer to the widget at the given position in the grid map
 */
QWidget* Game::getWidgetAt(int row, int column) {
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();
    return widget;
}

/**
 * @brief Check if the item at the given position in the grid map is a Tower or not.
 * @param row a row in the game's grid map
 * @param column a column in the game's grid map
 * @return true if the item is a Tower, false otherwise
 */
bool Game::isTower(int row, int column) {
    return dynamic_cast<Tower*>(getWidgetAt(row, column));
}

/**
 * @brief Checks if the square at the given location is a path tile.
 * @param row The row of the square in question.
 * @param column The column of the square in question.
 * @return A boolean denoting if the square located at the given location is a path tile or not.
 */
bool Game::isPath(int row, int column) {
    return dynamic_cast<Path*>(getWidgetAt(row, column));
}

/**
 * @brief Checks if the square at the given location is an end of the enemy's path.
 * @param row The row of the square in question.
 * @param column The column of the square in question.
 * @return A boolean denoting if the square located at the given location is an end of the enemy's path.
 */
bool Game::isPathEnd(int row, int column) {
    Path* path = dynamic_cast<Path*>(getWidgetAt(row, column));
    if (!path) {
        return false;
    } else if (path->getType() != Start && path ->getType() != End) {
        return false;
    }
    return true;
}

/**
 * @brief Checks if the square at the given location contains a comment tower.
 * @param row The row of the location in question.
 * @param column The column of the location in question.
 * @return A boolean denoting if the square at the given location contains a comment tower or not.
 */
bool Game::isComment(int row, int column) {
    return dynamic_cast<Comment*>(getWidgetAt(row, column));
}

/**
 * @brief Checks if the location is populated by enemies.
 * 
 * @param row The row in the grid.
 * @param column The column in the grid.
 * @return true 
 * @return false 
 */
bool Game::isEnemy(int row, int column)
{
    QPoint point(row,column);
    for (Enemy* enemy : activeEnemies_)
    {
        if(enemy->getNextLocation()==point) return true;
    }
    return false;
}

/**
 * @brief Set the game to upgrade mode for upgrading tower.
 */
void Game::enterUpgradeMode() {
    resetButtonHighlights();

    mode_ = Modes::upgrade;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::yellow);
    upgradeButton->setBrush(brush);
}

/**
 * @brief Set the game to build mode, with the tower type CS Student.
 */
void Game::enterBuildCS() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::CS_Student;
    build_CSstudent->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

/**
 * @brief Set the game to build mode, with the tower type TA.
 */
void Game::enterBuildTA() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::TA;
    build_TA->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

/**
 * @brief Set the game to build mode, with the tower type Search Engine.
 */
void Game::enterBuildSE() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::SearchEngine;
    build_SE->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

/**
 * @brief Set the game to build mode, with the tower type Language Server.
 */
void Game::enterBuildLS() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::LanguageServer;
    build_LS->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

/**
 * @brief Set the game to build mode, with the tower type Valgrind.
 */
void Game::enterBuildVal() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::Valgrind;
    build_Valgrind->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

void Game::enterBuildCom() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::Comment;
    build_Comment->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

/**
 * @brief Set the game to sell mode.
 */
void Game::enterSellMode() {
    resetButtonHighlights();

    mode_ = Modes::sell;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::yellow);
    sellButton->setBrush(brush);
}

/**
 * @brief Updates the paths of all the enemies.
 * Path is calculated using breath-first search and returned by getShortestPath(QPoint).
 * Is connected to the wallAction() signal.
 */
void Game::updatePaths()
{

    foreach (Enemy* enemy, activeEnemies_)
    {
        QPoint start = enemy->getMatrixLocation();
        if (enemy->getTimer()->isActive()) {
            start = enemy->getNextLocation();
        }
        QList<QPoint> newMatrixPath = getShortestPath(start);

        enemy->setPath(newMatrixPath,convertCoordinates(newMatrixPath));
        if(!enemy->getTimer()->isActive())
        {
            enemy->getTimer()->start();
        }
    }
}

/**
 * @brief Updates the currently active enemy count.
 * 
 */
void Game::updateEnemyCount()
{
    enemyCount_=activeEnemies_.length();
}

/**
 * @brief Stops the timers of all the enemies in the game.
 * 
 */
void Game::stopEnemies()
{
    foreach (Enemy* enemy, activeEnemies_) {
        enemy->getTimer()->stop();
        delete enemy;
//        this->removeItem(enemy);
    }
}

/**
 * @brief Adds amount of enemies that have been spawned during this level up to this point.
 * 
 * @param amount The amount of enemies added.
 */
void Game::addSpawnedEnemies(int amount)
{
    spawnedThisWave_+= amount;
}

/**
 * @brief Upgrade the tower at the given coordinates on the grid map.
 * @param row the row of the tower in the game's grid map
 * @param column the column of the tower in the game's grid map
 * @return true if the upgrade was successful, false otherwise
 */
bool Game::upgradeTower(int row, int column) {
    QWidget* widget = getWidgetAt(row, column);
    Tower* tower= dynamic_cast<Tower*>(widget);

    // change upgrade color back to green
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    upgradeButton->setBrush(brush);

    // if the square is a tower, upgrade it if possible
    if (tower) {
        // Hide the attack ranges of all other towers
        hideAllAttackAreasExcept(tower->getCoords());

        return tower->upgrade();// return the state of the upgrade
    }
    else {
        // not a tower
        return false;
    }
}

void Game::resetButtonHighlights() {
    build_CSstudent->setStyleSheet(buildButtonStylesheet);
    build_TA->setStyleSheet(buildButtonStylesheet);
    build_SE->setStyleSheet(buildButtonStylesheet);
    build_LS->setStyleSheet(buildButtonStylesheet);
    build_Valgrind->setStyleSheet(buildButtonStylesheet);
    build_Comment->setStyleSheet(buildButtonStylesheet);

    // change upgrade color back to green
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    upgradeButton->setBrush(brush);

    // change sell color back to green
    sellButton->setBrush(brush);
}

void Game::hideAllAttackAreasExcept(QPointF exclude)
{
    for (QPointF towerCoords : coordsOfTowers){
        if (towerCoords != exclude){
            QWidget* widget = getWidgetAt(towerCoords.x(), towerCoords.y());
            Tower* tower= dynamic_cast<Tower*>(widget);
            tower->hideAttackArea();
        } else {
            QWidget* widget = getWidgetAt(towerCoords.x(), towerCoords.y());
            Tower* tower= dynamic_cast<Tower*>(widget);
            tower->showAttackArea();
        }
    }

}

/**
 * @brief Starts the game.
 */
void Game::startGame() {
    if(level_==0 )
    {
        createWave();
    }
}

/**
 * @brief Adds create a specified label and adds it to the layout specified.
 * 
 * @param layout The layout the new created label will be added.
 * @param text The text on the label.
 * @param stylesheet The stylesheet that will be applied to the label.
 */
void Game::addLabelTo(QGraphicsLinearLayout* layout, QString text, QString stylesheet) {
    // Add price visuals
    QLabel * label = new QLabel(text);
    label->setStyleSheet(stylesheet);
    // set font
    QFont font("Roboto", 14);
    label->setAlignment(Qt::AlignCenter);
    label->setFont(font);
    // add nameLabel to the layout
    QGraphicsProxyWidget* widgetName = new QGraphicsProxyWidget();
    widgetName->setWidget(label);
    widgetName->setMaximumHeight(18);
    widgetName->setMaximumWidth(BUILD_BUTTON_SIZE+5);
    layout->addItem(widgetName);
}
