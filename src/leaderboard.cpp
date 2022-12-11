/**
 * @file leaderboard.cpp
 * @authors Siim (siim.kasela@aalto.fi), Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "leaderboard.h"
#include "mainview.h"
#include "button.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>

/**
 * @brief Construct a new Leaderboard:: Leaderboard object. Places the graphics into the leaderboard scene.
 * 
 * @param parent The parent object of the leaderboard scene.
 */
Leaderboard::Leaderboard(QObject* parent): QGraphicsScene(parent)
{
    // set size 1280x717 (use 717 height because we dont want scroll)
    setSceneRect(0,0,1280,717);


    // Create the titletext
    QGraphicsTextItem * titleText = new QGraphicsTextItem(QString("Leaderboard"));
    // set font for titletext
    QFont titleFont("Roboto", 50, 900);
    titleText->setDefaultTextColor(Qt::green);
    titleText->setFont(titleFont);
    // set position for titletext
    int txPos = this->width()/2 - titleText->boundingRect().width()/2;
    int tyPos = 50;
    titleText->setPos(txPos,tyPos);
    // add titletext to the scene
    addItem(titleText);

    readFile();

    QGraphicsWidget *scoresForm = new QGraphicsWidget;
    QGraphicsWidget *namesForm = new QGraphicsWidget;
    QGraphicsLinearLayout* scoresLayout = new QGraphicsLinearLayout(Qt::Vertical);
    QGraphicsLinearLayout* namesLayout = new QGraphicsLinearLayout(Qt::Vertical);
    for(QPair<QString,int> &score:leaderboard_)
    {
        QLabel* highScore = new QLabel();
        highScore->setText("<span style=\"color : lime; margin 0;\">"
                           + QString::number(score.second) + "</span>");
        highScore->setFont(QFont("Roboto", 20, QFont::Bold));
        highScore->setFrameStyle(QFrame::Raised | QFrame::Panel);
        highScore->setStyleSheet
            ("QLabel { background-color : transparent; color : lime; margin : 0; text-align:left }");
        QGraphicsProxyWidget* player = addWidget(highScore);
        player->setContentsMargins(0,0,0,0);
        scoresLayout->addItem(player);

        QLabel* highScoreName = new QLabel();
        highScoreName->setText("<div style=\"color : lime; margin: 0; text-align:right\">"
                               + score.first.left(30) + "</div>");
        highScoreName->setFont(QFont("Roboto", 20, QFont::Bold));
        highScoreName->setFrameStyle(QFrame::Raised | QFrame::Panel);
        highScoreName->setStyleSheet
            ("QLabel { background-color : transparent; color : lime; margin : 0; padding : 0; text-align:right }");
        QGraphicsProxyWidget* playerName = addWidget(highScoreName);
        playerName->setContentsMargins(0,0,0,0);
        namesLayout->addItem(playerName);
        namesLayout->setAlignment(playerName, Qt::AlignRight);
    }

    scoresLayout->setContentsMargins(0,0,0,0);
    namesLayout->setContentsMargins(0,0,0,0);
    scoresLayout->setSpacing(10);
    namesLayout->setSpacing(10);

    scoresForm->setLayout(scoresLayout);
    addItem(scoresForm);

    scoresForm->setPos(titleText->scenePos().x() + titleText->boundingRect().width()/2 + 1
                       , titleText->y()+titleText->boundingRect().height()+20);


    namesForm->setLayout(namesLayout);
    addItem(namesForm);

    namesForm->setPos(titleText->scenePos().x() - 1 - namesForm->minimumWidth() + titleText->boundingRect().width()/2
                      , titleText->y()+titleText->boundingRect().height()+20);

    // main menu button
    Button * menuButton = new Button(QString("Main menu"), 200, 50);
    int lxPos = this->width() - menuButton->boundingRect().width() - 40;
    int lyPos = 40;
    menuButton->setPos(lxPos, lyPos);
    connect(menuButton, SIGNAL(clicked()), this, SLOT(showMenu()));
    menuButton->setZValue(10);
    addItem(menuButton);
}

/**
 * @brief Sets leaderboard.
 * 
 * @param leaderboard New leaderboard.
 */
void Leaderboard::setLeaderBoard(QList<QPair<QString, int> > leaderboard)
{
    leaderboard_ = leaderboard;
}

/**
 * @brief Reads the leaderboard data file from OS specific data location. On windows %appdata%
 * 
 */
void Leaderboard::readFile()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"_leaderboard.dat");
    if(!file.open(QIODevice::ReadWrite))
    {

        QMessageBox::critical(qobject_cast<QWidget*>(this), tr("Error"),
                     "Could not open leaderboard file: "+file.errorString());

        return;
    }
    QDataStream stream(&file);
    if(stream.atEnd())
    {
        return;
    }
    QList<QPair<QString,int>> leaderboard;
    while(!stream.atEnd())
    {
        QString name;
        int score;
        stream>>name>>score;
        leaderboard<<QPair<QString,int>(name,score);
    }
    leaderboard_=leaderboard;
    file.close();
}

/**
 * @brief Shows menu.
 * 
 */
void Leaderboard::showMenu(){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->showMenu();
}
