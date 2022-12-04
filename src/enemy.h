#ifndef ENEMY_H
#define ENEMY_H


#include <QGraphicsPixmapItem>
#include <QTimer>

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

    EnemyType getType() const;

public slots:

    void move();

signals:

    void enemyDies(int);
    void dealsDamage(int);
    void addedEnemy(Enemy*);

protected:

    int health_;
    int damage_;
    int speed_; // 0 to 70
    int pointValue_;
//    Game game_;
    QList<QPointF> path_;
    QList<QPoint> matrixPath_;
    QPointF dest_;
    int point_index_;
    EnemyType type_;
    QTimer* timer_;
};

#endif // ENEMY_H
