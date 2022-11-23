#include "leaderboard.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QLabel>

Leaderboard::Leaderboard(QObject* parent): QGraphicsScene(parent)
{
    //only testing purposes
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    QLabel* test = new QLabel();
    test->setText(QString("success!!"));
    QGraphicsProxyWidget* player = addWidget(test);
    layout->addItem(player);
    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    addItem(form);
}
