#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>

// enumeration to keep track of the game's mode
namespace Modes{
    enum MODES{normal, build, upgrade};
}

// enumeration to keep track of tower types for build
namespace TowerTypes{
    enum TYPES{CS_Student, TA, SearchEngine, LanguageServer, Valgrind};
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
    QList<QPointF> convertCoordinates(QList<QPoint> path);
    void readWaveFile();

    int getHealth() const;
    int getScore() const;
    int getLevel() const;
    int getCurrency() const;
    int getEnemyCount() const;
    Modes::MODES getMode() const;
    TowerTypes::TYPES getBuildType() const;

    void takeDamage(int dHealth);
    void changeScore(int points);
    void changeCurrency(int dCurrency);
    void setMode(Modes::MODES m);
    void advanceLevel();
    void enemyDies();
    void keyPressEvent(QKeyEvent *keyEvent);

    QGraphicsGridLayout* mapLayout; //map area where the action is
    QGraphicsLinearLayout* gameLayout; //the whole are of the game, including the controls
    QGraphicsLinearLayout* controlsLayout;//change this to your liking

    bool isTower(int row, int column);
public slots:
    void showMenu();
    void enterUpgradeMode();
    void enterBuildCS();
    void enterBuildTA();
    void enterBuildSE();
    void enterBuildLS();
    void enterBuildVal();
    void spawnEnemy(int type, QList<QPointF> path);
    void updateLeadrboard();
    void showError(QString message);

signals:
    void gameWon();
    void gameLost();
    void waveWon();
    void error(QString);


private:

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

    Modes::MODES mode_;
    TowerTypes::TYPES buildType_;
    QList<QPoint> shortest_path_;
};

#endif // GAME_H
