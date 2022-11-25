#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
/**
 * @brief The Game class. Contains the game interface part.
 */
class Game: public QGraphicsScene
{
    Q_OBJECT

public:
    Game(QObject* parent);
    QPointF getSquarePos(int row, int column);

    bool isLost() const;
    void createMap();
    void createGameControls();

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

private:
    /**
     * @brief The amount of reputation points (health) the player has.
     */
    int health_;
    /**
     * @brief The amount of currency the player has.
     */
    int currency_;
    /**
     * @brief The current time of the game. (?)
     */
    int time_;
    /**
     * @brief The current number of waves.
     */
    int wavesCount_;
    /**
     * @brief The number of enemies killed. (?)
     */
    int enemyCount_;
    /**
     * @brief The current level of the game.
     */
    int level_;
    /**
     * @brief The current score.
     */
    int score_;
};

#endif // GAME_H
