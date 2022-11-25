#include "mainview.h"
#include "menu.h"
/**
 * @brief MainView Class initializer.
 * @param parent The parent widget of the class.
 */
MainView::MainView(QWidget *parent): QGraphicsView(parent)
{
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    game_= new Game(this);
    menu_ = new Menu(this);
    leaderboard_ = new Leaderboard(this);
    setFixedSize(800,700);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


}
/**
 * @brief Returns the game interface part.
 * @return A Game object.
 */
Game* MainView::getGame()
{
    return game_;
}
/**
 * @brief Returns the menu interface part.
 * @return A Menu object.
 */
Menu* MainView::getMenu()
{
    return menu_;
}
/**
 * @brief Returns the leaderboard interface part.
 * @return A Leaerboard object.
 */
Leaderboard* MainView::getLeaderboard()
{
    return leaderboard_;
}
/**
 * @brief Displays the game interface part.
 */
void MainView::showGame()
{
    setScene(game_);
}
/**
 * @brief Displays the menu interface part.
 */
void MainView::showMenu()
{
    setScene(menu_);
}
/**
 * @brief Displays the leaderboard interface part.
 */
void MainView::showLeaderboard()
{
    setScene(leaderboard_);
}

