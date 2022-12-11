/**
 * @file ta.h
 * @authors Hung Vu (hung.h.vu@aalto.fi), Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TA_H
#define TA_H

#include "tower.h"
#include <QList>
#include <QPointF>

/**
 * @brief A TA tower type. Can attack enemies in range and buff nearby towers. Inherited from Tower class.
 */
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
    double damageBuffFactor_;//!< the damage buff factor of this TA tower, use for buffing other towers
    double atkSpeedBuffFactor_;//!< the attack speed buff factor of this TA tower, use for buffing other towers
    int buffPulseInterval_;//!< time between each buff pulse, which buff towers in range if they haven't got buffed yet. In mili-seconds.
    QTimer* buffTimer_;//!< timer for timing the buff pulse, which buff towers in range if they haven't got buffed yet

    int maxRangeMultiplier_ = 1;//!< for increasing the tower's max range

    QList<QPointF> buffedTowers;//!< a list of towers buffed by this tower
};


#endif // TA_H
