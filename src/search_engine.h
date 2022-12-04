#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H
#include "tower.h"
#include <QList>
#include <QPointF>

class Search_Engine : public Tower {
public:
    Search_Engine();
    Search_Engine(int row, int column, QWidget *parent=nullptr);

    ~Search_Engine();

    bool upgrade();
public slots:
    void buffPulse();
private:
    double damageBuffFactor_;
    double atkSpeedBuffFactor_;
    int buffPulseInterval_;// time between each check for new tower in range (in ms)
    QTimer* buffTimer_;

    QList<QPointF> buffedTowers;
};

#endif // SEARCH_ENGINE_H
