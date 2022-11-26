#ifndef SQUARE_H
#define SQUARE_H
#include <QLabel>
#include <QPoint>

class Square : public QLabel
{
    Q_OBJECT
public:
     Square(QPoint pos, QWidget *parent = nullptr);
     void mousePressEvent(QMouseEvent* event);
     void fire(QPointF target);
private:
     int pos_;
signals:

};

#endif // SQUARE_H
