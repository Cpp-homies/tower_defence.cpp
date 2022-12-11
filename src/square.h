/**
 * @file square.h
 * @author Hung (), Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SQUARE_H
#define SQUARE_H
#include <QLabel>
#include <QGraphicsGridLayout>

/**
 * @brief This is the square object. It represents a square which is a tower or an empty square where a tower can be placed or a path.
 * 
 */
class Square : public QLabel
{
    Q_OBJECT
public:
     Square(int x, int y, QWidget *parent = nullptr);
     void mousePressEvent(QMouseEvent* event);
     virtual bool isTower();
     virtual void showHideAttackArea(){};
     QPointF getCoords() {return QPointF(x_,y_);}
public slots:
     virtual void getTarget() {};// virtual function for Tower class, needed for connecting the timer
     virtual void buffPulse() {};// virtual function for support towers, needed for connecting the timer
protected:
     /**
      * @brief The X-coordinate of the square.
      * 
      */
     int x_;
     /**
      * @brief The Y-coordinate of the square.
      * 
      */
     int y_;
     void fire(QPointF target);

private:
     QPixmap rotate(int angle, QPixmap pixmap);
};

#endif // SQUARE_H
