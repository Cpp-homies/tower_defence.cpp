#ifndef VALGRIND_H
#define VALGRIND_H

#include "tower.h"

class Valgrind : public Tower {
public:
    Valgrind(int row, int column, QWidget *parent=nullptr);
    virtual bool upgrade();
    void updateDescription();
    void fire(QPointF targetPos);
};

#endif // VALGRIND_H
