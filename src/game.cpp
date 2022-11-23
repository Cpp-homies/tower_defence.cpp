#include "game.h"
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QPushButton>
#include "square.h"

#include <QIcon>
#include <QScrollBar>


Game::Game()
{
    scene = new QGraphicsScene(this);
    scene ->setSceneRect(0,0,1000,1000);

    setFixedSize(1000,1000);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout = new QGraphicsGridLayout();
    layout->setSpacing(0);

    for(int i = 0; i<10; ++i)
    {
        for (int j = 0; j < 10; ++j) {

                Square* tile = new Square(nullptr, i, j);
                QGraphicsProxyWidget* backgroundTile = scene->addWidget(tile);
                layout->addItem(backgroundTile,i,j);


        }
    }


    QGraphicsWidget *form = new QGraphicsWidget;
    form->setLayout(layout);
    scene->addItem(form);
    setScene(scene);
}

QPointF Game::getSquarePos(int row, int column){

    return layout->itemAt(row,column)->graphicsItem()->scenePos();
}
