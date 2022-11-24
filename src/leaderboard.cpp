#include "leaderboard.h"
#include "mainview.h"
#include "button.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QLabel>

extern MainView * view;

Leaderboard::Leaderboard(QObject* parent): QGraphicsScene(parent)
{
    // set size 1280x717 (use 717 height because we dont want scroll)
    setSceneRect(0,0,1280,717);

    //only testing purposes
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    QLabel* test = new QLabel();
    test->setText(QString("success!!"));
    QGraphicsProxyWidget* player = addWidget(test);
    layout->addItem(player);
    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    addItem(form);

    // main menu button
    Button * menuButton = new Button(QString("Main menu"), 200, 50);
    int lxPos = this->width() - menuButton->boundingRect().width() - 10;
    int lyPos = 10;
    menuButton->setPos(lxPos, lyPos);
    connect(menuButton, SIGNAL(clicked()), this, SLOT(showMenu()));
    menuButton->setZValue(10);
    addItem(menuButton);
}

void Leaderboard::showMenu(){
    view->showMenu();
}
