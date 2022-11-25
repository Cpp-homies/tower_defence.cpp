#include "leaderboard.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QLabel>
/**
 * @brief Leaderboard contructor
 * @param parent The parent object of the leaderboard.
 */
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
