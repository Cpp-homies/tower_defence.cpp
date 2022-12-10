#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "game.h"
#include "menu.h"
#include "leaderboard.h"

#include <QGraphicsView>

class MainView :  public QGraphicsView
{
    Q_OBJECT

public:
    MainView(QWidget *parent = nullptr);

    Game* getGame();
    Menu* getMenu();
    Leaderboard* getLeaderboard();
public slots:
    void showGame(int gamemode); // 0 = sandbox, 1 = easy, 2 = hard
    void showMenu();
    void showLeaderboard();


private:
    Game* game_ = nullptr;
    Menu* menu_;
    Leaderboard* leaderboard_;
};

#endif // MAINVIEW_H
