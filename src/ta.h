#ifndef TA_H
#define TA_H

#include "tower.h"

class TA : public Tower {
public:
    TA();
    TA(int row, int column, QWidget *parent=nullptr);

    bool upgrade();
private:
    double damageBuffFactor_;
    QGraphicsPixmapItem * towerImg;
};


#endif // TA_H
