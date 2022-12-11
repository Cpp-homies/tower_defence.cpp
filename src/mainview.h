/**
 * @file mainview.h
 * @author Siim Kasela (siim.kasela@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "game.h"
#include "menu.h"
#include "leaderboard.h"

#include <QGraphicsView>

/**
 * @brief Class for representing the View that contains the Menu, Game and Leaderboard.
 * 
 */
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
    /**
     * @brief The game scene.
     * 
     */
    Game* game_ = nullptr;
    /**
     * @brief The menu scene.
     * 
     */
    Menu* menu_;
    /**
     * @brief The leaderboard scene.
     * 
     */
    Leaderboard* leaderboard_;
};

#endif // MAINVIEW_H
