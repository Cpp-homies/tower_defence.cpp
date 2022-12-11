/**
 * @file game.h
 * @authors Siim Kasela (siim.kasela@aalto.fi), Saku Kovanen (saku.kovanen@aalto.fi), Harvey Lim (harvey.lim@aalto.fi), Hung Vu (hung.h.vu@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef GAME_H
#define GAME_H

#include "enemy.h"
#include "button.h"
#include <QGraphicsView>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QToolButton>
#include <QMutex>
#include <QThread>
#include <QAtomicInteger>

// enumeration to keep track of the game's mode
namespace Modes{
    enum MODES{normal, build, upgrade, sell, exit};
}

// enumeration to keep track of tower types for build
namespace TowerTypes{
    enum TYPES{CS_Student, TA, SearchEngine, LanguageServer, Valgrind, Comment};
}

/**
 * @brief The class holding everything related to the actual tower defence gameplay.
 */
class Game: public QGraphicsScene
{
    Q_OBJECT
    QThread thread;
public:

    Game(QObject* parent, int gamemode = 0);
    QPointF getSquarePos(int row, int column);
    QWidget* getWidgetAt(int row, int column);
    bool buildTower(int row, int column);
    bool buildTower(int row, int column, TowerTypes::TYPES type);
    bool sellTower(int row, int column);
    bool upgradeTower(int row, int column);

    bool isLost() const;
    bool isWon() const;
    bool isWaveWon();
    void createMap();
    void createGameControls();
    void readWaveFile();

    void playHitsound();
    void playClicksound();

    QList<QPointF> convertCoordinates(QList<QPoint> path);
    QList<QPoint> getShortestPath(QPoint start);
    QList<QPoint> BFS(QPoint start, bool blocked);

    void breakComment(int row, int column);
    void deleteComment(int row, int column);
    
    /**
     * @brief Get the Gamemode:
     * 0 = sandbox, 
     * 1 = easy, 
     * 2 = hard
     * 
     * @return int Corresponds to the gamemode of the game instance.
     */
    int getGamemode() const {return gamemode_;};

    int getHealth() const;
    int getScore() const;
    int getLevel() const;
    int getCurrency() const;
    int getEnemyCount() const;
    Modes::MODES getMode() const;
    TowerTypes::TYPES getBuildType() const;


    void changeScore(int points);
    void changeCurrency(int dCurrency);
    void setMode(Modes::MODES m);
    void advanceLevel();
    void addSpawnedEnemies(int);

    void keyPressEvent(QKeyEvent *keyEvent);
    void resetButtonHighlights();

    void hideAllAttackAreasExcept(QPointF exclude = QPointF(-1, -1));

    QGraphicsGridLayout* mapLayout; //map area where the action is
    QGraphicsLinearLayout* gameLayout; //the whole are of the game, including the controls
    QGraphicsGridLayout* controlsLayout;//change this to your liking

    bool isTower(int row, int column);
    bool isPath(int row, int column);
    bool isPathEnd(int row, int column);
    bool isComment(int row, int column);
    bool isEnemy(int row, int column);
public slots:
    void showMenu();
    void enterUpgradeMode();
    void enterBuildCS();
    void enterBuildTA();
    void enterBuildSE();
    void enterBuildLS();
    void enterBuildVal();
    void enterSellMode();
    void enemyDies(int value);
    void spawnEnemy(int type);
    void updateLeaderboard();
    void showError(QString message);
    void addEnemy(Enemy*);
    void takeDamage(int dHealth);
    void enterBuildCom();
    void updatePaths();
    void updateEnemyCount();
    void stopEnemies();

    void createWave();
    void startGame();

signals:
    void gameWon();
    void gameLost();
    void waveWon();
    void error(QString);
    void wallAction();


private:
    QPoint start_;
    QPoint end_;
    /**
     * @brief The decoded map, represented by a matrix of characters.
     */
    QList<QList<QString>> map_;
    /**
     * @brief A boolean stating if all paths are obstructed or not.
     */
    bool isBlocked_;

    /**
     * @brief Corresponds to the gamemode of the game instance.
     * 
     */
    int gamemode_; // 0 = sandbox, 1 = easy, 2 = hard

    int health_;
    int currency_;
    //int time_;
    int wavesEnemyCount_;
    QAtomicInteger<int> spawnedThisWave_;
    int enemyCount_;
    int level_;
    int finalLevel_;
    int score_;
    /**
     * @brief Income is multiplied by this double before it is added to the currency
     * 
     */
    double incomeMultiplier_ = 1.0;
    /**
     * @brief Income is initially set to this. It will decrease based on the round
     * 
     */
    double initialIncomeMultiplier_ = 2.0;
    QList<QPoint> path_;
    QList<QStringList> waves_;
    QList<Enemy*> activeEnemies_;

    /**
     * @brief The first hitsound sample.
     */
    QSoundEffect hitsound1_;
    /**
     * @brief The second hitsound sample.
     */
    QSoundEffect hitsound2_;
    /**
     * @brief The third hitsound sample.
     */
    QSoundEffect hitsound3_;
    /**
     * @brief The fourth hitsound sample.
     */
    QSoundEffect hitsound4_;
    /**
     * @brief The mouseclick sound sample.
     */
    QSoundEffect clicksound_;

    Modes::MODES mode_;//!< the current game mode. Currently there are 5 modes: normal, upgrade, build, sell, exit.
    TowerTypes::TYPES buildType_;//!< the tower type that will be build, used for build mode
    /**
     * @brief The shortest path between the starting point and the ending point.
     */
    QList<QPoint> shortest_path_;

    /**
     * @brief Displays the round number.
     * 
     */
    QGraphicsTextItem * roundDisplay;
    /**
     * @brief Displays the score accumulated during current run.
     * 
     */
    QGraphicsTextItem * scoreDisplay;
    /**
     * @brief Displays the amount health points left.
     * 
     */
    QGraphicsTextItem * healthDisplay;
    /**
     * @brief Displays the amount of currency.
     * 
     */
    QGraphicsTextItem * wealthDisplay;
    /**
     * @brief Button to enter upgrade mode.
     * 
     */
    Button * upgradeButton;
    /**
     * @brief Button to enter sell mode.
     * 
     */
    Button * sellButton;
    /**
     * @brief Button to start round 1, if round is 0.
     * 
     */
    Button * nextRoundButton;
    /**
     * @brief Button to build CS student.
     * 
     */
    QToolButton* build_CSstudent;
    /**
     * @brief Button to build Teacher's Assistant.
     * 
     */
    QToolButton* build_TA;
    /**
     * @brief Button to build Search Engine.
     * 
     */
    QToolButton* build_SE;
    /**
     * @brief Button to build Language Server
     * 
     */
    QToolButton* build_LS;
    /**
     * @brief Button to build Valgrind.
     * 
     */
    QToolButton* build_Valgrind;
    /**
     * @brief Button to build comment.
     * 
     */
    QToolButton* build_Comment;
    /**
     * @brief Stylesheet for the build buttons.
     * 
     */
    QString buildButtonStylesheet;
    /**
     * @brief List of the coordinates of the towers.
     * 
     */
    QList<QPointF> coordsOfTowers;

    void addLabelTo(QGraphicsLinearLayout* layout, QString towerName, QString stylesheet);
};

#endif // GAME_H
