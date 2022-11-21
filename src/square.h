#ifndef SQUARE_H
#define SQUARE_H
#include <QLabel>

class Square : public QLabel
{
    Q_OBJECT
public:
     Square(QWidget *parent = nullptr);
     void mousePressEvent(QMouseEvent* event);
     void fire(QPointF target);
private:
     QList<QPixmap> tilePics;
signals:

};

#endif // SQUARE_H
