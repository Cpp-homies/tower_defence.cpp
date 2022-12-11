#ifndef VALGRIND_H
#define VALGRIND_H

#include "tower.h"

/**
 * @brief A Valgrind tower type. Can attack all enemies on the map. Inherited from Tower class.
 */
class Valgrind : public Tower {
public:
    Valgrind(int row, int column, QWidget *parent=nullptr);
    virtual bool upgrade();
    void updateDescription();
    void fire(QPointF targetPos);
};

#endif // VALGRIND_H
