#include "valgrind.h"

Valgrind::Valgrind(int row, int column, QWidget *parent/*unused*/) : Tower(row, column, 15, 40, 10000, parent) {
    // set Valgrind stats
    upgradeLevel_ = 1;
    maxLevel_ = 1;
    pierce_ = 3;

    // valgrind can also target memory errors and bosses right at level 1 as well
    targetAble_[EnemyTypes::CompilerError] = true;
    targetAble_[EnemyTypes::RuntimeError] = true;

    // set Valgrind graphics
    ogImagePath_ = ":/images/Valgrind.png";
    projectileImagePath_ = ":/images/Valgrind_projectile.png";
    this->towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    this->towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );

}
bool Valgrind::upgrade() {
    if (upgradeLevel_ >= maxLevel_) {
        // already max level
        return false;
    }
    else {
        // upgrade the tower according to its level
        upgradeLevel_ += 1;

        /**
        * Valgrind does not have further level designed yet at the moment
        */

        return true;
    }
}
