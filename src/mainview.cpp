#include "mainview.h"
#include "menu.h"

MainView::MainView(QWidget *parent): QGraphicsView(parent)
{
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    game_= new Game(this);
    menu_ = new Menu(this);
    leaderboard_ = new Leaderboard(this);
    setFixedSize(1280,720);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkGreen);
    setBackgroundBrush(brush);

}

Game* MainView::getGame()
{
    return game_;
}

Menu* MainView::getMenu()
{
    return menu_;
}

Leaderboard* MainView::getLeaderboard()
{
    return leaderboard_;
}

void MainView::showGame()
{
    setScene(game_);
}

void MainView::showMenu()
{
    setScene(menu_);
}

void MainView::showLeaderboard()
{
    setScene(leaderboard_);
}

