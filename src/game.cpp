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


#define BUILD_BUTTON_SIZE 90

// prices of towers can be set here
#define CS_COST 20
#define TA_COST 30
#define SE_COST 25
#define COM_COST 15
#define LS_COST 50
#define VAL_COST 70

// penalty for selling the tower (will be deducted from the tower's total value
#define SELL_PENALTY 0.3


Game::Game(QObject* parent): QGraphicsScene(parent)
{
    // set starting values of health, currency etc
    health_ = 100;
    currency_ = 9999999;
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
    connect(this,SIGNAL(gameWon()),this,SLOT(updateLeaderboard()));
    connect(this,SIGNAL(gameLost()),this,SLOT(stopEnemies()));
    connect(this,SIGNAL(gameLost()),this,SLOT(updateLeaderboard()));

}

void Game::createMap(){
    mapLayout = new QGraphicsGridLayout();

//    QFile test(":/files/testing.map");
//    test.open(QIODevice::WriteOnly);
//    QDataStream out(&test);
//    out << (qint32)16;
//    out << (qint32)9;
//    test.close();

    QFile map(":/files/test.tdmap");
    // TODO: Handle invalid map (either doesn't exist or wrong format)
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
        for (int j = 0; j < height; ++j) {
            QString line = "";
            for (int i = 0; i < width; ++i) {
                if (isPath(j, i)) {
                    line.push_back(" ");
                } else {
                    line.push_back("O");
                }
            }
            qInfo() << line;
        }
    }

    // set margins to 0
    mapLayout->setContentsMargins(0,0,0,0);

    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(mapLayout);
    gameLayout->addItem(form);
    shortest_path_ = getShortestPath(start_);
    qInfo() << shortest_path_;
}

QList<QPoint> Game::getShortestPath(QPoint start) {
    QList<QPoint> path = BFS(start, false);
    if (!path.empty()) {
        return path;
    }
    return BFS(start, true);
}

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

void Game::createGameControls()
{
    //for testing purposes, all Layouts will auto adjust the size
    controlsLayout = new QGraphicsGridLayout();

    // set spacings
    controlsLayout->setContentsMargins(80,270,0,0);
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
    int HIyPos = 140;
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
    int WIyPos = 140;
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
    buildButtonStylesheet = "background-color: white; border: 1px solid white";
        // create CSstudent button
    build_CSstudent = new QToolButton();
    // create the icon for the button
    build_CSstudent->setIcon(QIcon(QPixmap(":/images/CStudent1.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_CSstudent->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_CSstudent->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_CSstudent, SIGNAL(clicked()), this, SLOT(enterBuildCS()));

    // Add price visuals
    QLabel * cstCostVisual = new QLabel(QString::number(CS_COST));
    // set font
    QFont cstCostFont("Roboto", 16);
    cstCostVisual->setFont(cstCostFont);
    QGraphicsLinearLayout* cstudentButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    cstudentButtonAndPrice->setSpacing(0);

        // add the button and price to the control layout
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* cstWidget = addWidget(build_CSstudent);
    cstudentButtonAndPrice->addItem(cstWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* cstWidgetCost = addWidget(cstCostVisual);
    cstWidgetCost->setMaximumHeight(20);
    cstudentButtonAndPrice->addItem(cstWidgetCost);
    controlsLayout->addItem(cstudentButtonAndPrice, 1, 1);


        // create TA button
    build_TA = new QToolButton();
    // create the icon for the button
    build_TA->setIcon(QIcon(QPixmap(":/images/TA.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_TA->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_TA->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_TA, SIGNAL(clicked()), this, SLOT(enterBuildTA()));

    // Add price visuals
    QLabel * teaCostVisual = new QLabel(QString::number(TA_COST));
    // set font
    QFont teaCostFont("Roboto", 16);
    teaCostVisual->setFont(teaCostFont);
    QGraphicsLinearLayout* teachersassistantButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    teachersassistantButtonAndPrice->setSpacing(0);

        // add the button and price to the control layout
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* teaWidget = addWidget(build_TA);
    teachersassistantButtonAndPrice->addItem(teaWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* teaWidgetCost = addWidget(teaCostVisual);
    teaWidgetCost->setMaximumHeight(20);
    teachersassistantButtonAndPrice->addItem(teaWidgetCost);
    controlsLayout->addItem(teachersassistantButtonAndPrice, 1, 2);


        // create Search Engine button
    build_SE = new QToolButton();
    // create the icon for the button
    build_SE->setIcon(QIcon(QPixmap(":/images/Bing.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_SE->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_SE->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_SE, SIGNAL(clicked()), this, SLOT(enterBuildSE()));

    // Add price visuals
    QLabel * seaCostVisual = new QLabel(QString::number(SE_COST));
    // set font
    QFont seaCostFont("Roboto", 16);
    seaCostVisual->setFont(seaCostFont);
    QGraphicsLinearLayout* searchengineButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    searchengineButtonAndPrice->setSpacing(0);

        // add the button and price to the control layout
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* seaWidget = addWidget(build_SE);
    searchengineButtonAndPrice->addItem(seaWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* seaWidgetCost = addWidget(seaCostVisual);
    seaWidgetCost->setMaximumHeight(20);
    searchengineButtonAndPrice->addItem(seaWidgetCost);
    controlsLayout->addItem(searchengineButtonAndPrice, 1, 3);


        // create Language server button
    build_LS = new QToolButton();
    // create the icon for the button
    build_LS->setIcon(QIcon(QPixmap(":/images/Language_server.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_LS->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_LS->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_LS, SIGNAL(clicked()), this, SLOT(enterBuildLS()));

    // Add price visuals
    QLabel * lanCostVisual = new QLabel(QString::number(LS_COST));
    // set font
    QFont lanCostFont("Roboto", 16);
    lanCostVisual->setFont(lanCostFont);
    QGraphicsLinearLayout* languageserverButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    languageserverButtonAndPrice->setSpacing(0);

        // add the button and price to the control layout
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* lanWidget = addWidget(build_LS);
    languageserverButtonAndPrice->addItem(lanWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* lanWidgetCost = addWidget(lanCostVisual);
    lanWidgetCost->setMaximumHeight(20);
    languageserverButtonAndPrice->addItem(lanWidgetCost);
    controlsLayout->addItem(languageserverButtonAndPrice, 2, 1);


        // create Valgrind button
    build_Valgrind = new QToolButton();
    // create the icon for the button
    build_Valgrind->setIcon(QIcon(QPixmap(":/images/Valgrind.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_Valgrind->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_Valgrind->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_Valgrind, SIGNAL(clicked()), this, SLOT(enterBuildVal()));

    // Add price visuals
    QLabel * valCostVisual = new QLabel(QString::number(VAL_COST));
    // set font
    QFont valCostFont("Roboto", 16);
    valCostVisual->setFont(valCostFont);
    QGraphicsLinearLayout* valgrindButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    valgrindButtonAndPrice->setSpacing(0);

        // add the button and price to the control layout
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* valWidget = addWidget(build_Valgrind);
    valgrindButtonAndPrice->addItem(valWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* valWidgetCost = addWidget(valCostVisual);
    valWidgetCost->setMaximumHeight(20);
    valgrindButtonAndPrice->addItem(valWidgetCost);
    controlsLayout->addItem(valgrindButtonAndPrice, 2, 2);



        // create Comment button
    build_Comment = new QToolButton();
    // create the icon for the button
    build_Comment->setIcon(QIcon(QPixmap(":/images/Comment.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_Comment->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_Comment->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_Comment, SIGNAL(clicked()), this, SLOT(enterBuildCom()));

    // Add price visuals
    QLabel * comCostVisual = new QLabel(QString::number(COM_COST));
    // set font
    QFont comCostFont("Roboto", 16);
    comCostVisual->setFont(comCostFont);
    QGraphicsLinearLayout* commentButtonAndPrice= new QGraphicsLinearLayout(Qt::Vertical);
    commentButtonAndPrice->setSpacing(0);

        // add the button and price to the control layout
    // add button to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* comWidget = addWidget(build_Comment);
    commentButtonAndPrice->addItem(comWidget);
    // add valCostVisual to the linearlayout ButtonAndPrice
    QGraphicsProxyWidget* comWidgetCost = addWidget(comCostVisual);
    comWidgetCost->setMaximumHeight(20);
    commentButtonAndPrice->addItem(comWidgetCost);
    controlsLayout->addItem(commentButtonAndPrice, 2, 3);


    // add the control layout to the game layout
    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(controlsLayout);
    gameLayout->addItem(form);
}

//creates a wave of enemies for one level according to the description in wave.txt
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
    ++level_;
    roundDisplay->setPlainText(QString::number(level_));







}

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

void Game::updateLeaderboard()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"_leaderboard.dat");


    if(!file.open(QIODevice::ReadWrite))
    {

        emit error("Could not open leaderboard file: "+file.errorString());
        return;
    }
    QDataStream stream(&file);

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

}

void Game::showError(QString message)
{
    QMessageBox::warning(qobject_cast<QWidget*>(this), tr("Error"),
                 message);

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



bool Game::isLost() const{
    return health_<=0;
}

bool Game::isWon() const
{
    return  health_>0 && level_==finalLevel_ ;
}

bool Game::isWaveWon()
{

    if(spawnedThisWave_ == wavesEnemyCount_ && enemyCount_ == 0)
    {
        spawnedThisWave_ = QAtomicInteger(0);
        wavesEnemyCount_ = 0;
        return true;
    } else return false;

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

void Game::takeDamage (int dHealth) {
    health_-=dHealth;
    healthDisplay->setPlainText(QString::number(health_));
    Enemy* enemy = qobject_cast<Enemy*>(sender());
    activeEnemies_.removeOne(enemy);
    activeEnemies_.squeeze();
    updateEnemyCount();
    delete enemy;
    if(isLost())
    {
        emit gameLost();
        return;
    }
    if(isWaveWon())
    {
        isWon() ? emit gameWon() :  createWave();

    }
}

void Game::changeCurrency (int dCurrency) {
    currency_+=dCurrency;
    wealthDisplay->setPlainText(QString::number(currency_));
}

void Game::changeScore (int dPoints) {
    score_+=dPoints;
    scoreDisplay->setPlainText(QString::number(score_));
}

void Game::setMode(Modes::MODES m) {
    mode_ = m;
}

void Game::advanceLevel () {
    level_++;
}

void Game::enemyDies(int value)
{
    changeScore(value);
    changeCurrency(value);
    Enemy* enemy = qobject_cast<Enemy*>(sender());
    activeEnemies_.removeOne(enemy);
    activeEnemies_.squeeze();
    updateEnemyCount();
    QTimer* timer = new QTimer();
    timer->setSingleShot(true);
    timer->setInterval(500);
    timer->callOnTimeout([this](){
        if(this->isWaveWon())
        {
            this->isWon() ? emit this->gameWon() : this->createWave() ;
        }
    });
    timer->start();

}

void Game::addEnemy(Enemy* enemy)
{
    addItem(enemy);
    enemy->startMove();
    connect(enemy,SIGNAL(enemyDies(int)),this,SLOT(enemyDies(int)));
    connect(enemy,SIGNAL(addedEnemy(Enemy*)),this,SLOT(addEnemy(Enemy*)));
    connect(enemy,SIGNAL(dealsDamage(int)),this,SLOT(takeDamage(int)));
    connect(enemy,SIGNAL(enemyDies(int)),this,SLOT(updateEnemyCount()));
    connect(enemy,SIGNAL(addedEnemy(Enemy*)),this,SLOT(updateEnemyCount()));
    activeEnemies_<<enemy;
    updateEnemyCount();

}

//just testing scene changing
//can be used for other purposes
void Game::keyPressEvent(QKeyEvent* event)
{
    auto key = event->key();
    if (key == ' '){
        startGame();
    }

}

QPointF Game::getSquarePos(int row, int column){

    return mapLayout->itemAt(row,column)->graphicsItem()->scenePos();
}

void Game::showMenu(){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->showMenu();
    mode_ = Modes::exit;// added to handle object deletion
    this->deleteLater();

}

/**
 *
 * The below function was used for testing and is now outdated
 *
 */
// build a default tower (CS Student)
//bool Game::buildTower(int row, int column) {
//    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
//    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();

//    // if there is a tower occupying the square, return false
//    if (dynamic_cast<Tower*>(widget) || isPath(row, column)) {
//        return false;
//    }
//    else {
//        // create a new tower and add it to the scene
//        QGraphicsWidget* tower = this->addWidget(new CS_Student(row, column, nullptr));

//        // remove the current square from the grid
//        this->removeItem(item->graphicsItem());
//        this->mapLayout->removeItem(item);

//        // add a tower to the grid at the given possition
//        this->mapLayout->addItem(tower, row, column);

//        return true;
//    }
//}

bool Game::buildTower(int row, int column, TowerTypes::TYPES type) {
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();
    resetButtonHighlights();

    // if the type is comment
    if (type == TowerTypes::Comment) {
        // check if this is a path and whether there is already a comment
        if (isPath(row, column) && !dynamic_cast<Comment*>(widget) && !isEnemy(row, column)) {
            // if it is available, start building the comment
            // first, check if the player have enough money or not
            // if yes, build the tower
            if (this->currency_ >= COM_COST) {
                // create a new tower and add it to the scene
                Comment* newComment = new Comment(column, row, 10000, dynamic_cast<Path*>(widget), nullptr);
                QGraphicsWidget* comment = this->addWidget(newComment);

                // Notify enemeies of path change
                emit wallAction();

                // remove the current square from the grid
                this->mapLayout->removeItem(item);

                // add the comment to the grid at the given possition
                this->mapLayout->addItem(comment, row, column);

                // Hide the attack ranges of all other towers
                hideAllAttackAreasExcept(QPointF(row,column));

                // deduct the cost of the comment from player's money
                changeCurrency(-COM_COST);
            
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
            if (this->currency_ >= TA_COST) {
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
                changeCurrency(-SE_COST);

                // add the cost of the tower to tower's total cost
                newTower->addCost(SE_COST);
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

    // if there is no tower occupying the square, return false
    if (!tower) {
        return false;
    }
    else {
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

bool Game::isPath(int row, int column) {
    return dynamic_cast<Path*>(getWidgetAt(row, column));
}

bool Game::isComment(int row, int column) {
    return dynamic_cast<Comment*>(getWidgetAt(row, column));
}

bool Game::isEnemy(int row, int column)
{
    QPoint point(row,column);
    for (Enemy* enemy : activeEnemies_)
    {
        if(enemy->getNextLocation()==point) return true;
    }
    return false;
}

void Game::enterUpgradeMode() {
    resetButtonHighlights();

    mode_ = Modes::upgrade;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::yellow);
    upgradeButton->setBrush(brush);
}

void Game::enterBuildCS() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::CS_Student;
    build_CSstudent->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

void Game::enterBuildTA() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::TA;
    build_TA->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

void Game::enterBuildSE() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::SearchEngine;
    build_SE->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

void Game::enterBuildLS() {
    resetButtonHighlights();

    mode_ = Modes::build;
    buildType_ = TowerTypes::LanguageServer;
    build_LS->setStyleSheet("background-color: rgb(0,255,0); border: 1px solid black");
}

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

void Game::enterSellMode() {
    resetButtonHighlights();

    mode_ = Modes::sell;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::yellow);
    sellButton->setBrush(brush);
}

void Game::updatePaths()
{
    foreach (Enemy* enemy, activeEnemies_)
    {
        QList<QPoint> newMatrixPath = getShortestPath(enemy->getMatrixLocation());
        enemy->setPath(newMatrixPath,convertCoordinates(newMatrixPath));
        if(!enemy->getTimer()->isActive())
        {
            enemy->getTimer()->start();
        }
    }
}

void Game::updateEnemyCount()
{
    enemyCount_=activeEnemies_.length();
}

void Game::stopEnemies()
{
    foreach (Enemy* enemy, activeEnemies_) {
        enemy->getTimer()->stop();
        delete enemy;
//        this->removeItem(enemy);
    }
}

void Game::addSpawnedEnemies(int amount)
{
    spawnedThisWave_+= amount;
}

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
        }
    }

}

void Game::startGame() {
    if(level_==0 )
    {
        createWave();
    }
}
