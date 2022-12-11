/**
 * @file mainview.cpp
 * @author Siim Kasela (siim.kasela@aalto.fi), Saku Kovanen (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "mainview.h"
#include "menu.h"

/**
 * @brief Construct a new Main View:: Main View object
 * 
 * @param parent 
 */
MainView::MainView(QWidget *parent): QGraphicsView(parent)
{
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    menu_ = new Menu(this);
    leaderboard_ = new Leaderboard(this);
    setFixedSize(1280,720);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    leaderboard_ = new Leaderboard(this);
    // Set background image
    QBrush brush;
    brush.setTexture(QPixmap(":/images/Mainmenu.png"));
    setBackgroundBrush(brush);

}
/**
 * @brief Returns the game pointer.
 * 
 * @return Game* 
 */
Game* MainView::getGame()
{
    return game_;
}

/**
 * @brief Returns the menu pointer.
 * 
 * @return Menu* 
 */
Menu* MainView::getMenu()
{
    return menu_;
}

/**
 * @brief Returns the leaderboard pointer.
 * 
 * @return Leaderboard* 
 */
Leaderboard* MainView::getLeaderboard()
{
    return leaderboard_;
}

/**
 * @brief Switches to the game scene.
 * 
 * @param gamemode The mode of the game to use.
 */
void MainView::showGame(int gamemode)
{
    game_= new Game(this, gamemode);
    setScene(game_);
    game_->readWaveFile();
    // Set background image
    QBrush brush;
    brush.setTexture(QPixmap(":/images/Game_controls.png"));
    setBackgroundBrush(brush);
}

/**
 * @brief Switches to the menu scene.
 * 
 */
void MainView::showMenu()
{
    setScene(menu_);

    // Set background image
    QBrush brush;
    brush.setTexture(QPixmap(":/images/Mainmenu.png"));
    setBackgroundBrush(brush);
}

/**
 * @brief Switches to the leaderboard scene.
 * 
 */
void MainView::showLeaderboard()
{

    leaderboard_ = new Leaderboard(this);
    // Set background image
    QBrush brush;
    brush.setTexture(QPixmap(":/images/Mainmenu.png"));
    setBackgroundBrush(brush);
    setScene(leaderboard_);
}

