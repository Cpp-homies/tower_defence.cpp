#include "mainview.h"
#include "menu.h"


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
    game_= new Game(this);
    setScene(game_);
    game_->readWaveFile();
    // Set background image
    QBrush brush;
    brush.setTexture(QPixmap(":/images/Game_controls.png"));
    setBackgroundBrush(brush);
}

void MainView::showMenu()
{
    setScene(menu_);

    // Set background image
    QBrush brush;
    brush.setTexture(QPixmap(":/images/Mainmenu.png"));
    setBackgroundBrush(brush);
}

void MainView::showLeaderboard()
{

    leaderboard_ = new Leaderboard(this);
    // Set background image
    QBrush brush;
    brush.setTexture(QPixmap(":/images/Mainmenu.png"));
    setBackgroundBrush(brush);
    setScene(leaderboard_);
}

