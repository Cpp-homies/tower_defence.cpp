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

class Game: public QGraphicsScene
{
    Q_OBJECT
    QThread thread;
public:

    Game(QObject* parent);
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

    QList<QPointF> convertCoordinates(QList<QPoint> path);
    QList<QPoint> getShortestPath(QPoint start);
    QList<QPoint> BFS(QPoint start, bool blocked);

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

    void hideAllAttackAreasExcept(QPointF exclude = QPointF());

    QGraphicsGridLayout* mapLayout; //map area where the action is
    QGraphicsLinearLayout* gameLayout; //the whole are of the game, including the controls
    QGraphicsGridLayout* controlsLayout;//change this to your liking

    bool isTower(int row, int column);
    bool isPath(int row, int column);
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
    QList<QList<QString>> map_;
    bool isBlocked_;

    int health_;
    int currency_;
    int time_;
    int wavesEnemyCount_;
    QAtomicInteger<int> spawnedThisWave_;
    int enemyCount_;
    int level_;
    int finalLevel_;
    int score_;
    QList<QPoint> path_;
    QList<QStringList> waves_;
    QList<Enemy*> activeEnemies_;


    Modes::MODES mode_;
    TowerTypes::TYPES buildType_;
    QList<QPoint> shortest_path_;

    QGraphicsTextItem * roundDisplay;
    QGraphicsTextItem * scoreDisplay;
    QGraphicsTextItem * healthDisplay;
    QGraphicsTextItem * wealthDisplay;
    Button * upgradeButton;
    Button * sellButton;
    Button * nextRoundButton;
    QToolButton* build_CSstudent;
    QToolButton* build_TA;
    QToolButton* build_SE;
    QToolButton* build_LS;
    QToolButton* build_Valgrind;
    QToolButton* build_Comment;
    QString buildButtonStylesheet;

    QList<QPointF> coordsOfTowers;

    void addLabelTo(QGraphicsLinearLayout* layout, QString towerName, QString stylesheet);
};

#endif // GAME_H
