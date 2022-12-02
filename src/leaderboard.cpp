#include "leaderboard.h"
#include "mainview.h"
#include "button.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>

extern MainView * view;

Leaderboard::Leaderboard(QObject* parent): QGraphicsScene(parent)
{
    // set size 1280x717 (use 717 height because we dont want scroll)
    setSceneRect(0,0,1280,717);
    readFile();
    //only testing purposes
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    for(QPair<QString,int> &score:leaderboard_)
    {
        QLabel* test = new QLabel();
        test->setText(score.first+ " " +QString::number(score.second));
        QGraphicsProxyWidget* player = addWidget(test);
        layout->addItem(player);
    }


    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    addItem(form);

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
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"leaderboard.dat");
    if(!file.open(QIODevice::ReadOnly))
    {

        QMessageBox::information(qobject_cast<QWidget*>(this), tr("Error"),
                     "Could not open leaderboard file: "+file.errorString());

        return;
    }
    QDataStream stream(&file);
    if(stream.atEnd())
    {
        QMessageBox::information(qobject_cast<QWidget*>(this), tr("Error"),
                     "Leaderboard file is empty!"+file.errorString());
        file.close();
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
    view->showMenu();
}
