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
    QGraphicsScene * scene;
    QGraphicsGridLayout* layout;
private:


};

#endif // GAME_H
