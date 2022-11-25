#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QGraphicsScene>

class Leaderboard: public QGraphicsScene
{
    Q_OBJECT
public:
    Leaderboard(QObject* parent  = 0);

public slots:
    void showMenu();

private:


};

#endif // LEADERBOARD_H
