#ifndef TA_H
#define TA_H

#include "tower.h"
#include <QList>
#include <QPointF>

class TA : public Tower {
public:
    TA();
    TA(int row, int column, QWidget *parent=nullptr);

    ~TA();

    void fire(QPointF targetPos);

    bool upgrade();
    void updateDescription();
public slots:
    void buffPulse();
private:
    double damageBuffFactor_;
    double atkSpeedBuffFactor_;
    int buffPulseInterval_;// time between each check for new tower in range (in ms)
    QTimer* buffTimer_;

    int maxRangeMultiplier_ = 1;

    QList<QPointF> buffedTowers;
};


#endif // TA_H
