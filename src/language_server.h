#ifndef LANGUAGE_SERVER_H
#define LANGUAGE_SERVER_H

#include "tower.h"


class Language_Server : public Tower {
public:
    Language_Server();
    Language_Server(int row, int column, QWidget *parent=nullptr);

    void fire(QPointF targetPos);
    virtual bool upgrade() {};
};

#endif // LANGUAGE_SERVER_H
