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
#include <QSignalMapper>


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

    // Create a playbutton
    Button * easyPlayButton = new Button(QString("Easy"), 200, 50);
    int epxPos = this->width()/2 - easyPlayButton->boundingRect().width()/2;
    int epyPos = 280;
    easyPlayButton->setPos(epxPos, epyPos);
    addItem(easyPlayButton);

    // Create a playbutton
    Button * hardPlayButton = new Button(QString("Hard"), 200, 50, Qt::darkRed);
    int hpxPos = epxPos + hardPlayButton->boundingRect().width() + 20;
    int hpyPos = epyPos;
    hardPlayButton->setPos(hpxPos, hpyPos);
    addItem(hardPlayButton);

    // Create a playbutton
    Button * sandboxPlayButton = new Button(QString("Sandbox"), 200, 50, Qt::darkYellow);
    int spxPos = epxPos - hardPlayButton->boundingRect().width() - 20;
    int spyPos = epyPos;
    sandboxPlayButton->setPos(spxPos, spyPos);
    addItem(sandboxPlayButton);

    // map signals
    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(easyPlayButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(hardPlayButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(sandboxPlayButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(easyPlayButton, 1);
    signalMapper->setMapping(hardPlayButton, 2);
    signalMapper->setMapping(sandboxPlayButton, 0);

    connect(signalMapper, SIGNAL(mappedInt(int)), this, SLOT(showGame(int)));

    // Create the leadeboardbutton
    Button * leaderboardButton = new Button(QString("Leaderboard"), 200, 50);
    int lxPos = this->width()/2 - leaderboardButton->boundingRect().width()/2;
    int lyPos = hpyPos + 100;
    leaderboardButton->setPos(lxPos, lyPos);
    connect(leaderboardButton, SIGNAL(clicked()), this, SLOT(showLeaderboard()));
    addItem(leaderboardButton);

    // Create the quitbutton
    Button * quitButton = new Button(QString("Quit"), 200, 50);
    int qxPos = this->width()/2 - quitButton->boundingRect().width()/2;
    int qyPos = lyPos + 100;
    quitButton->setPos(qxPos, qyPos);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    addItem(quitButton);
}

void Menu::showGame(int gamemode){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->showGame(gamemode);
}

void Menu::showLeaderboard(){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->showLeaderboard();
}

void Menu::quit(){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->close();
}
