#include "ta.h"


/**
*
* TODO : FIX THE BUFF FEATURE, THE GraphicsItem is not a base class of tower
* so it cannot be casted to that
*
*/
//Tower(int x, int y, int range, int damage, int attackSpeed, QWidget *parent=nullptr);
TA::TA(int row, int column, QWidget *parent) : Tower(row, column, 4, 10, 3000) {
    // set TA stats
    damageBuffFactor_ = 1.2;
    upgradeLevel_ = 1;
    maxLevel_ = 2;

    // set TA graphics
    ogImagePath_ = ":/images/TA.png";
    towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );

    // get all the items in the tower's range
    QList <QGraphicsItem*> items_in_range = getItemInRange();
    // buff all towers in range
    for (int i = 0, n = items_in_range.size(); i < n; i++) {
        Tower * tower = dynamic_cast<Tower *>(items_in_range[i]);
        // if this is a tower
        if (tower) {
            tower->damageBuff(damageBuffFactor_);
        }
    }
}

bool TA::upgrade() {
    if (upgradeLevel_ >= maxLevel_) {
        // already max level
        return false;
    }
    else {
        // upgrade the tower according to its level
        upgradeLevel_ += 1;

        switch (upgradeLevel_) {
        case 2:
            // increase range
            setRange(6);

            // update tower graphics
            ogImagePath_ = ":/images/Teacher.png";
            towerImg->setPixmap(QPixmap(ogImagePath_));

            break;
        }

        return true;
    }
}
