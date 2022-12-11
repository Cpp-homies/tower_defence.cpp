/**
 * @file valgrind.h
 * @authors Hung Vu (hung.h.vu@aalto.fi), Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
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
