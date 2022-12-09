#ifndef COMMENT_H
#define COMMENT_H

#include "path.h"
#include <QPointF>
#include <QTimer>
#include "mainview.h"

extern MainView* view;

class Comment : public Path {
public:
    Comment(int x, int y, int duration, Path* old, QWidget* parent);
    void changeHealth(int value);
    QPointF towerCenter();
    Path* getOld();
    ~Comment();
    bool isTimerActive();
    void startTimer();
public slots:
    void breakComment();
private:
    int duration_;
    QGraphicsPixmapItem * towerImg;
    Path* old_;
    QTimer* breakdownTimer_;
    int x_;
    int y_;
};

#endif // COMMENT_H
