#ifndef SQUARE_H
#define SQUARE_H
#include <QLabel>
#include <QGraphicsGridLayout>

class Square : public QLabel
{
    Q_OBJECT
public:
     Square(QWidget *parent = nullptr);
     Square(QWidget *parent, int& x, int& y);
     void mousePressEvent(QMouseEvent* event);
public slots:
     virtual void getTarget() {};// virtual function for Tower class, needed for connecting the timer
protected:
     int x_;
     int y_;
     void fire(QPointF target);
private:

//     QGraphicsGridLayout* parentGrid;
     QList<QPixmap> tilePics;
signals:

};

#endif // SQUARE_H
