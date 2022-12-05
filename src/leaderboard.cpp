#include "leaderboard.h"
#include "mainview.h"
#include "button.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>


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

    QGraphicsWidget *form = new QGraphicsWidget;
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    for(QPair<QString,int> &score:leaderboard_)
    {
        QLabel* highScore = new QLabel();
        highScore->setFixedWidth(titleText->boundingRect().width()-15);
        highScore->setText("<div style=\"float: left\">" + score.first +
                           "</div> <div style=\"float: right; text-align:right\">" +
                           QString::number(score.second) + "</div>");
        highScore->setFont(QFont("Roboto", 20, QFont::Bold));
        highScore->setFrameStyle(QFrame::Raised | QFrame::Panel);
        highScore->setStyleSheet("QLabel { background-color : black; color : lime; padding : 5px; margin : 0 }");
        QGraphicsProxyWidget* player = addWidget(highScore);
        layout->addItem(player);
    }



    form->setLayout(layout);
    addItem(form);

    form->setPos(titleText->scenePos().x(),this->height()/4);

    // main menu button
    Button * menuButton = new Button(QString("Main menu"), 200, 50);
    int lxPos = this->width() - menuButton->boundingRect().width() - 40;
    int lyPos = 40;
    menuButton->setPos(lxPos, lyPos);
    connect(menuButton, SIGNAL(clicked()), this, SLOT(showMenu()));
    menuButton->setZValue(10);
    addItem(menuButton);
}

void Leaderboard::setLeaderBoard(QList<QPair<QString, int> > leaderboard)
{
    leaderboard_ = leaderboard;
}

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

void Leaderboard::showMenu(){
    MainView* view = qobject_cast<MainView*>(this->parent());
    view->showMenu();
}
