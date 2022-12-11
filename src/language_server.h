/**
 * @file language_server.h
 * @authors Hung Vu (hung.h.vu@aalto.fi), Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef LANGUAGE_SERVER_H
#define LANGUAGE_SERVER_H

#include "tower.h"

/**
 * @brief A Language Server tower type. Can attack all enemies in range at the same time. Inherited from Tower class.
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
