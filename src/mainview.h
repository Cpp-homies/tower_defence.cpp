#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "game.h"
#include "menu.h"
#include "leaderboard.h"

#include <QGraphicsView>

/**
 * @brief The MainView class.
 * Extends the QGraphicsView class. Holds all interface parts.
 */
class MainView :  public QGraphicsView
{
    Q_OBJECT
public:

    MainView(QWidget *parent = nullptr);

    Game* getGame();

    Menu* getMenu();

    Leaderboard* getLeaderboard();

    void showGame();

    void showMenu();

    void showLeaderboard();

private:
    /**
     * @brief The game interface part of the current view.
     */
    Game* game_;
    /**
     * @brief The menu interface part of the current view.
     */
    Menu* menu_;
    /**
     * @brief The leaderboard interface part of the current view.
     */
    Leaderboard* leaderboard_;
};

#endif // MAINVIEW_H
