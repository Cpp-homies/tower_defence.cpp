#ifndef SQUARE_H
#define SQUARE_H
#include <QLabel>
#include <QGraphicsGridLayout>

class Square : public QLabel
{
    Q_OBJECT
public:
//     Square(QWidget *parent = nullptr);
     Square(int x, int y, QWidget *parent = nullptr);
     void mousePressEvent(QMouseEvent* event);
     virtual bool isTower();
     virtual void showHideAttackArea(){};
public slots:
     virtual void getTarget() {};// virtual function for Tower class, needed for connecting the timer
protected:
     int x_;
     int y_;
     void fire(QPointF target);

private:
     QPixmap rotate(int angle, QPixmap pixmap);
signals:

};

#endif // SQUARE_H
