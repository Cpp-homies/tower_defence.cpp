#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsGridLayout>

class Game: public QGraphicsView
{
    Q_OBJECT

public:

    Game();
    QPointF getSquarePos(int row, int column);

private:

    QGraphicsGridLayout* layout;
    QGraphicsScene * scene;
};

#endif // GAME_H
