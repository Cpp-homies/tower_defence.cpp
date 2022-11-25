#ifndef SQUARE_H
#define SQUARE_H
#include <QLabel>
/**
 * @brief The Square class. Square objects create the map of the game.
 */
class Square : public QLabel
{
    Q_OBJECT
public:
     Square(int x, int y, QWidget *parent = nullptr);
     void mousePressEvent(QMouseEvent* event);
     void fire(QPointF target);
private:
     QList<QPixmap> tilePics;
     /**
      * @brief The X-coordinate (column) of the square.
      */
     int x_;
     /**
      * @brief The Y-coordinate (row) of the square.
      */
     int y_;
signals:

};

#endif // SQUARE_H
