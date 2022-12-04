#include "menu.h"
#include "square.h"
#include "mainview.h"
#include "button.h"
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QGraphicsTextItem>


Menu::Menu(QObject *parent): QGraphicsScene(parent)
{
    setSceneRect(0,0,1280,717);

    // Create the titletext
    QGraphicsTextItem * titleText = new QGraphicsTextItem(QString("tower_defence.cpp"));
    // set font for titletext
    QFont titleFont("Roboto", 50, 900);
    titleText->setDefaultTextColor(Qt::green);
    titleText->setFont(titleFont);
    // set position for titletext
    int txPos = this->width()/2 - titleText->boundingRect().width()/2;
    int tyPos = 150;
    titleText->setPos(txPos,tyPos);
    // add titletext to the scene
    addItem(titleText);

    // Create the playbutton
    Button * playButton = new Button(QString("Play"), 200, 50);
    int pxPos = this->width()/2 - playButton->boundingRect().width()/2;
    int pyPos = 280;
    playButton->setPos(pxPos, pyPos);
    connect(playButton, SIGNAL(clicked()), this, SLOT(showGame()));
    addItem(playButton);

    // Create the leadeboardbutton
    Button * leaderboardButton = new Button(QString("Leaderboard"), 200, 50);
    int lxPos = this->width()/2 - leaderboardButton->boundingRect().width()/2;
    int lyPos = 350;
    leaderboardButton->setPos(lxPos, lyPos);
    connect(leaderboardButton, SIGNAL(clicked()), this, SLOT(showLeaderboard()));
    addItem(leaderboardButton);

    // Create the quitbutton
    Button * quitButton = new Button(QString("Quit"), 200, 50);
    int qxPos = this->width()/2 - quitButton->boundingRect().width()/2;
    int qyPos = 420;
    quitButton->setPos(qxPos, qyPos);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    addItem(quitButton);
}

void Menu::showGame(){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->showGame();
}

void Menu::showLeaderboard(){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->showLeaderboard();
}

void Menu::quit(){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->close();
}
