#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>

class Game: public QGraphicsScene
{
    Q_OBJECT

public:

    Game(QObject* parent);
    QPointF getSquarePos(int row, int column);
    QWidget* getWidgetAt(int row, int column);
    bool buildTower(int row, int column);

    bool isLost() const;
    void createMap();
    void createGameControls();
    void createWave(QList<QPoint> path);
    QList<QPointF> convertCoordinates(QList<QPoint> path);

    int getHealth() const;
    int getScore() const;
    int getLevel() const;
    int getCurrency() const;

    void changeHealth(int dHealth);
    void changeScore(int points);
    void changeCurrency(int dCurrency);
    void advanceLevel();
    void keyPressEvent(QKeyEvent *keyEvent);

    QGraphicsGridLayout* mapLayout; //map area where the action is
    QGraphicsLinearLayout* gameLayout; //the whole are of the game, including the controls
    QGraphicsLinearLayout* controlsLayout;//change this to your liking

    bool isTower(int row, int column);

private:

    int health_;
    int currency_;
    int time_;
    int wavesCount_;
    int enemyCount_;
    int level_;
    int score_;
    QList<QPoint> shortest_path_;
};

#endif // GAME_H
