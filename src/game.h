#ifndef GAME_H
#define GAME_H

#include "enemy.h"
#include <QGraphicsView>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>

// enumeration to keep track of the game's mode
namespace Modes{
    enum MODES{normal, build, upgrade};
}

// enumeration to keep track of tower types for build
namespace TowerTypes{
    enum TYPES{CS_Student, TA, SearchEngine, LanguageServer, Valgrind, Comment};
}

class Game: public QGraphicsScene
{
    Q_OBJECT

public:

    Game(QObject* parent);
    QPointF getSquarePos(int row, int column);
    QWidget* getWidgetAt(int row, int column);
    bool buildTower(int row, int column);
    bool buildTower(int row, int column, TowerTypes::TYPES type);
    bool upgradeTower(int row, int column);

    bool isLost() const;
    bool isWon() const;
    void createMap();
    void createGameControls();
    void createWave();
    void readWaveFile();
    QList<QPointF> convertCoordinates(QList<QPoint> path);
    QList<QPoint> getShortestPath(QPoint start);

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


    void keyPressEvent(QKeyEvent *keyEvent);

    QGraphicsGridLayout* mapLayout; //map area where the action is
    QGraphicsLinearLayout* gameLayout; //the whole are of the game, including the controls
    QGraphicsGridLayout* controlsLayout;//change this to your liking

    bool isTower(int row, int column);
    bool isPath(int row, int column);
    bool isComment(int row, int column);
public slots:
    void showMenu();
    void enterUpgradeMode();
    void enterBuildCS();
    void enterBuildTA();
    void enterBuildSE();
    void enterBuildLS();
    void enterBuildVal();
    void enemyDies(int value);
    void spawnEnemy(int type, QList<QPointF> path);
    void updateLeadrboard();
    void showError(QString message);
    void addEnemy(Enemy*,int);
    void takeDamage(int dHealth);
    void enterBuildCom();
    void updatePaths();


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

    int health_;
    int currency_;
    int time_;
    int wavesCount_;
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
};

#endif // GAME_H
