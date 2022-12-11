#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H
#include "tower.h"
#include <QList>
#include <QPointF>

/**
 * @brief A Search Engine tower type. Can buff nearby tower, but does not shoot at enemies. Inherited from Tower class.
 */
class Search_Engine : public Tower {
public:
    Search_Engine();
    Search_Engine(int row, int column, QWidget *parent=nullptr);

    ~Search_Engine();

    bool upgrade();
    void updateDescription();
public slots:
    void buffPulse();
private:
    double damageBuffFactor_; //!< the damage buff factor of this TA tower, use for buffing other towers
    double atkSpeedBuffFactor_; //!< the attack speed buff factor of this TA tower, use for buffing other towers
    int buffPulseInterval_; //!< time between each buff pulse, which buff towers in range if they haven't got buffed yet. In mili-seconds.
    QTimer* buffTimer_; //!< timer for timing the buff pulse, which buff towers in range if they haven't got buffed yet

    QList<QPointF> buffedTowers; //!< a list of towers buffed by this tower
};

#endif // SEARCH_ENGINE_H
