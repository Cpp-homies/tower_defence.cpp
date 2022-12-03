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
#include "ta.h"
#include "path.h"

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

#define BUILD_BUTTON_SIZE 90

// prices of towers can be set here
#define CS_COST 20
#define TA_COST 30

// penalty for selling the tower (will be deducted from the tower's total value
#define SELL_PENALTY 0.3
extern MainView * view;

Game::Game(QObject* parent): QGraphicsScene(parent)
{
    // set starting values of health, currency etc
    health_ = 100;
    currency_ = 100;
    level_ = 0;
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
    //connects error signal with a message box
    connect(this,SIGNAL(error(QString)),this, SLOT(showError(QString)));
    connect(this,SIGNAL(wallAction()),this,SLOT(updatePaths()));
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
                    Square* tile = new Square(j,i,nullptr);
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
                                tile = new Path(j, i, Straight, 90, nullptr);
                            } else {
                                tile = new Path(j, i, Straight, 0, nullptr);
                            }
                        }

                    } else if (value == "A") {
                        start_ = QPoint(j, i);
                        if (left) {
                            tile = new Path(j, i, Start, 0, nullptr);
                        } else if (up) {
                            tile = new Path(j, i, Start, 90, nullptr);
                        } else if (right) {
                            tile = new Path(j, i, Start, 180, nullptr);
                        } else {
                            tile = new Path(j, i, Start, 270, nullptr);
                        }
                    } else {
                        end_ = QPoint(j, i);
                        if (left) {
                            tile = new Path(j, i, End, 0, nullptr);
                        } else if (up) {
                            tile = new Path(j, i, End, 90, nullptr);
                        } else if (right) {
                            tile = new Path(j, i, End, 180, nullptr);
                        } else {
                            tile = new Path(j, i, End, 270, nullptr);
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
    qInfo() << shortest_path_;
}

QList<QPoint> Game::getShortestPath(QPoint start) {
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
            if (i.x() < total_cols && i.x() >= 0 && i.y() < total_rows && i.y() >= 0 && isPath(i.x(), i.y()) && !visited.contains(i)) {
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
    controlsLayout->setContentsMargins(80,300,0,0);
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
    Button * menuButton = new Button(QString("Main menu"), 200, 50);
    int lxPos = this->width() - menuButton->boundingRect().width() - 40;
    int lyPos = this->height() - menuButton->boundingRect().height() - 40;
    menuButton->setPos(lxPos, lyPos);
    connect(menuButton, SIGNAL(clicked()), this, SLOT(showMenu()));
    menuButton->setZValue(10);
    addItem(menuButton);

    // upgrade button
    upgradeButton = new Button(QString("Upgrade Tower"), 240, 50);

    int uxPos = this->width() - upgradeButton->boundingRect().width() - menuButton->boundingRect().width() - 80;
    int uyPos = this->height() - upgradeButton->boundingRect().height() - 40;
    upgradeButton->setPos(uxPos, uyPos);
    connect(upgradeButton, SIGNAL(clicked()), this, SLOT(enterUpgradeMode()));
    upgradeButton->setZValue(10);
    addItem(upgradeButton);

    // sell tower button
    sellButton = new Button(QString("Sell Tower"), 200, 50);

    int sxPos = this->width() - sellButton->boundingRect().width() - menuButton->boundingRect().width()
                    + (upgradeButton->boundingRect().width() - sellButton->boundingRect().width());
    int syPos = this->height() - sellButton->boundingRect().height() - 100;
    sellButton->setPos(sxPos, syPos);
    connect(sellButton, SIGNAL(clicked()), this, SLOT(enterSellMode()));
    sellButton->setZValue(10);
    addItem(sellButton);

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

    // add the button to the control layout
    QGraphicsProxyWidget* CSstudentWidget = addWidget(build_CSstudent);
    controlsLayout->addItem(CSstudentWidget, 1, 1);


        // create TA button
    build_TA = new QToolButton();
    // create the icon for the button
    build_TA->setIcon(QIcon(QPixmap(":/images/TA.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_TA->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_TA->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_TA, SIGNAL(clicked()), this, SLOT(enterBuildTA()));

    // add the button to the control layout
    QGraphicsProxyWidget* TAWidget = addWidget(build_TA);
    controlsLayout->addItem(TAWidget, 1, 2);


        // create Search Engine button
    build_SE = new QToolButton();
    // create the icon for the button
    build_SE->setIcon(QIcon(QPixmap(":/images/Bing.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_SE->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_SE->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_SE, SIGNAL(clicked()), this, SLOT(enterBuildSE()));

    // add the button to the control layout
    QGraphicsProxyWidget* SEWidget = addWidget(build_SE);
    controlsLayout->addItem(SEWidget, 1, 3);


        // create Language server button
    build_LS = new QToolButton();
    // create the icon for the button
    build_LS->setIcon(QIcon(QPixmap(":/images/Language_server.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_LS->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_LS->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_LS, SIGNAL(clicked()), this, SLOT(enterBuildLS()));

    // add the button to the control layout
    QGraphicsProxyWidget* LSWidget = addWidget(build_LS);
    controlsLayout->addItem(LSWidget, 2, 1);


        // create Valgrind button
    build_Valgrind = new QToolButton();
    // create the icon for the button
    build_Valgrind->setIcon(QIcon(QPixmap(":/images/Valgrind.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_Valgrind->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_Valgrind->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_Valgrind, SIGNAL(clicked()), this, SLOT(enterBuildVal()));

    // add the button to the control layout
    QGraphicsProxyWidget* ValWidget = addWidget(build_Valgrind);
    controlsLayout->addItem(ValWidget, 2, 2);


        // create Comment button
    build_Comment = new QToolButton();
    // create the icon for the button
    build_Comment->setIcon(QIcon(QPixmap(":/images/Comment.png").scaled(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE)));
    build_Comment->setIconSize(QSize(BUILD_BUTTON_SIZE, BUILD_BUTTON_SIZE));
    build_Comment->setStyleSheet(buildButtonStylesheet);

    // connect the button with the enterBuildMode function
    connect(build_Comment, SIGNAL(clicked()), this, SLOT(enterBuildCom()));

    // add the button to the control layout
    QGraphicsProxyWidget* ComWidget = addWidget(build_Comment);
    controlsLayout->addItem(ComWidget, 2, 3);


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
    //the path the enemies take
    QList<QPointF> convertedPath = convertCoordinates(shortest_path_);
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
            //after every timeout spawn an enemy
            timer->callOnTimeout([this, type, convertedPath](){this->spawnEnemy(type, convertedPath);});
            timer->setInterval(delay);
            //stop current timer when all enemies are spawned
            connect(nextEnemiesTimer, SIGNAL(timeout()), timer, SLOT(stop()));
            if(flag)
            {
                flag = false;
                //start both immediately since there are no enemies
                timer->start();
                nextEnemiesTimer->start((amount+1)*delay);
            } else
            {
                //wait till the last enemy type has stopped spawning
                connect(timerBuffer, SIGNAL(timeout()),timer, SLOT(start()));
                connect(timerBuffer, SIGNAL(timeout()),nextEnemiesTimer, SLOT(start()) );
            }

            //save a timer to the buffer to connect it during the next loop
            timerBuffer = nextEnemiesTimer;
            //how many enemies will be created in this loop
            enemyCount_+=amount;
        }

    }
    ++level_;
    roundDisplay->setPlainText(QString::number(level_));







}

void Game::spawnEnemy(int type,QList<QPointF> path)
{
    if(type==1 || type==2)
    {
        CompilerError* enemy = new CompilerError(static_cast<CompilerErrorType>(type), path, shortest_path_);
        addEnemy((Enemy*)enemy,0);
    }
    else if(type==3 || type==4 || type==5 || type==6)
    {
        MemoryError* enemy = new MemoryError(static_cast<MemoryErrorType>(type), path, shortest_path_);
        addEnemy((Enemy*)enemy,0);
    }
    else if(type==7)
    {
        RuntimeError* enemy = new RuntimeError(static_cast<RuntimeErrorType>(type), path, shortest_path_);
        addEnemy((Enemy*)enemy,0);
    }

}

void Game::updateLeadrboard()
{

}

void Game::showError(QString message)
{
    QMessageBox::information(qobject_cast<QWidget*>(this), tr("Error"),
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
    return enemyCount_==0 && health_>0 && level_==finalLevel_;
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
    if(--enemyCount_==0 )
    {
        isLost() ? emit gameLost() : (isWon() ? emit gameWon() : createWave());

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
    if(--enemyCount_==0)
    {
        isWon() ? emit gameWon() : createWave();
    }
    Enemy* enemy = qobject_cast<Enemy*>(sender());
    activeEnemies_.removeOne(enemy);
    activeEnemies_.squeeze();
}

void Game::addEnemy(Enemy* enemy, int advanceCount)
{
    enemyCount_+=advanceCount;
    addItem(enemy);
    enemy->startMove();
    connect(enemy,SIGNAL(enemyDies(int)),this,SLOT(enemyDies(int)));
    connect(enemy,SIGNAL(addedEnemy(Enemy*,int)),this,SLOT(addEnemy(Enemy*,int)));
    connect(enemy,SIGNAL(dealsDamage(int)),this,SLOT(takeDamage(int)));
    activeEnemies_<<enemy;
}

//just testing scene changing
//can be used for other purposes
void Game::keyPressEvent(QKeyEvent* /* unused */)
{
    if(enemyCount_==0 && !isWon() )
    {

        createWave();

    }

}

QPointF Game::getSquarePos(int row, int column){

    return mapLayout->itemAt(row,column)->graphicsItem()->scenePos();
}

void Game::showMenu(){
    view->showMenu();
    this->clear();
}

// build a default tower (CS Student)
bool Game::buildTower(int row, int column) {
    QGraphicsLayoutItem* item = this->mapLayout->itemAt(row, column);
    QWidget* widget = (dynamic_cast<QGraphicsProxyWidget*>(item))->widget();

    // if there is a tower occupying the square, return false
    if (dynamic_cast<Tower*>(widget)) {
        return false;
    }
    else {
        // create a new tower and add it to the scene
        QGraphicsWidget* tower = this->addWidget(new CS_Student(row, column, nullptr));

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
    resetButtonHighlights();

    // if there is a tower occupying the square, return false
    if (dynamic_cast<Tower*>(widget) || dynamic_cast<Path*>(widget)) {
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
        Square* newSquare = new Square(row, column, nullptr);
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
    }
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


