#ifndef LANGUAGE_SERVER_H
#define LANGUAGE_SERVER_H

#include "tower.h"

/**
 * @brief A Search Engine tower type. Can attack all enemies in range at the same time. Inherited from Tower class.
 */
class Language_Server : public Tower {
public:
    Language_Server();
    Language_Server(int row, int column, QWidget *parent=nullptr);

    void fire(QPointF targetPos);
    virtual bool upgrade();
    void updateDescription();
};

#endif // LANGUAGE_SERVER_H
