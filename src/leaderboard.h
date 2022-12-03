#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QGraphicsScene>

class Leaderboard: public QGraphicsScene
{
    Q_OBJECT
public:
    Leaderboard(QObject* parent  = 0);
    void setLeaderBoard(QList<QPair<QString,int>> leaderboard);
    void readFile();

public slots:
    void showMenu();

private:
    QList<QPair<QString,int>> leaderboard_;

};

#endif // LEADERBOARD_H
