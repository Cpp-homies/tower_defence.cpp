/**
 * @file enemy.h
 * @author Siim Kasela (siim.kasela@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef ENEMY_H
#define ENEMY_H


#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QSoundEffect>

//enums of main and subtypes, could help with scaling

//main 3 types of enemies
//is the type_ variable
enum class EnemyType
{
    CompilerError=1,
    MemoryError=2,
    RuntimeError=3
};
//subtypes of enemies
//is the name_ variable of the enemies
enum CompilerErrorType
{
    SyntaxError=1,
    Exception=2
};
enum MemoryErrorType
{
    InvalidRead=3,
    InvalidWrite=4,
    XBytesAreLost=5,
    MismatchedDeleteFree=6
};
enum RuntimeErrorType
{
    MemoryStackMinion=8,
    StackOverflow=7
};

/**
 * @brief Virtual class of the enemies. All enemies in the game are inherited from this class.
 * 
 */
class Enemy: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(EnemyType type, QList<QPointF> path,QList<QPoint> matrixPath, int health = 0, int damage = 0, int speed = 0, QGraphicsItem * parent=0);

    virtual void attack(){}
    virtual void die();
    virtual void takeDamage(int damage);
    void setSpeed(int speed);
    void startMove();
    void setPath(QList<QPoint> matrixPath, QList<QPointF> path);
    QPoint getMatrixLocation() const;
    QTimer *getTimer ();
    QPoint getNextLocation() const;

    EnemyType getType() const;

public slots:

    void move();

signals:

    void enemyDies(int);
    void dealsDamage(int);
    void addedEnemy(Enemy*);

protected:
    /**
     * @brief The health of the enemy.
     * 
     */
    int health_;
    /**
     * @brief The damage what the enemy will deal when it reaches to the end of the path.
     * 
     */
    int damage_;
    /**
     * @brief The speed at which the enemies move along the path. Should be set between the values 0-70.
     * 
     */
    int speed_;
    /**
     * @brief The value of the enemy that is turned into points and currency once the enemy is killed.
     * 
     */
    int pointValue_;
    /**
     * @brief The QPointF(pixels) list of the path where the enemy moves along. 
     * 
     */
    QList<QPointF> path_;
    /**
     * @brief The QPoint list of the path. (Grid locations)
     * 
     */
    QList<QPoint> matrixPath_;
    /**
     * @brief The next destination of the enemy.
     * 
     */
    QPointF dest_;
    /**
     * @brief An index to keep track of the enemy movement along the path.
     * 
     */
    int point_index_;
    /**
     * @brief The type of the enemy specified by the enum EnemyType
     * 
     */
    EnemyType type_;
    /**
     * @brief The timer which is used to move the enemy.
     * 
     */
    QTimer* timer_;

};

#endif // ENEMY_H
