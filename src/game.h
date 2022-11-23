#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsGridLayout>

class Game: public QGraphicsScene
{
    Q_OBJECT

public:

    Game(QObject* parent);
    QPointF getSquarePos(int row, int column);

    bool isLost() const;
    void createMap();

    int getHealth() const;
    int getScore() const;
    int getLevel() const;
    int getCurrency() const;

    void changeHealth(int dHealth);
    void changeScore(int points);
    void changeCurrency(int dCurrency);
    void advanceLevel();
    void keyPressEvent(QKeyEvent *keyEvent);

    QGraphicsGridLayout* layout;


private:

    int health_;
    int currency_;
    int time_;
    int wavesCount_;
    int enemyCount_;
    int level_;
    int score_;
};

#endif // GAME_H
