#ifndef VALGRIND_H
#define VALGRIND_H

#include "tower.h"

class Valgrind : public Tower {
public:
    Valgrind(int row, int column, QWidget *parent=nullptr);
    virtual bool upgrade();
};

#endif // VALGRIND_H
