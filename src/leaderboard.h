/**
 * @file leaderboard.h
 * @authors Siim (siim.kasela@aalto.fi), Saku Kovanen (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QGraphicsScene>

/**
 * @brief This is the leaderboard scene.
 * 
 */
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
    /**
     * @brief Holds the leaderboard.
     * 
     */
    QList<QPair<QString,int>> leaderboard_;

};

#endif // LEADERBOARD_H
