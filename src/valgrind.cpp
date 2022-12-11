#include "valgrind.h"
#include "projectile.h"

// Stats can be set here
#define RANGE 50
#define DAMAGE 50
#define ATTACK_INTERVAL 3000 // ms
#define PIERCE  0
#define PROJECTILE_MOVE_INTERVAL 3 // default 10, lower to get faster projectiles
#define PROJECTILE_STEP_SIZE 30 // theoretical max is 32. If large, projectile may skip over enemies

Valgrind::Valgrind(int row, int column, QWidget *parent/*unused*/)
    : Tower(row, column, RANGE, DAMAGE, ATTACK_INTERVAL, parent) {
    // set Valgrind stats
    upgradeLevel_ = 1;
    maxLevel_ = 1;
    pierce_ = PIERCE;

    // valgrind can also target memory errors and bosses right at level 1 as well
    targetAble_[EnemyTypes::MemoryError] = true;
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

void Valgrind::updateDescription() {
    // create new Tooltip description for this tower
    description_ = QString("<p><b>-Valgrind level %1-</b><br><br>"
                           "<b>Damage: </b>%2<br>"
                           "<b>Attack interval: </b>%3s<br>"
                           "<b>Range: </b>%4<br>"
                           "<b>Pierce: </b>%5<br>"
                           "<b>Total value: </b>%6<br>"
                           "<b>Can target: </b>").arg(QString::number(upgradeLevel_),
                                                            QString::number(damage_),
                                                            QString::number(attackInterval_ / 1000, 'f', 2),
                                                            QString::number(range_),
                                                            QString::number(pierce_),
                                                            QString::number(totalCost_));

    // list out the enemy types that this tower can target at the moment
    for (int i = 0, n = 3; i < n; ++i) {
        QString str = "";
        // check the enemy type
        switch (i) {
        // check if current tower can target given type
        // if yes, append it to the tower's description
        case EnemyTypes::RuntimeError:
        {
            if(targetAble_[EnemyTypes::RuntimeError] || targetAbleBuff_[EnemyTypes::RuntimeError]) {
                // add comma at the start of every element except the first one
                if (i != 0) {
                    str.append(", ");
                }
                str.append("Runtime Errors");
            }
            break;
        }
        case EnemyTypes::MemoryError:
        {
            if(targetAble_[EnemyTypes::MemoryError] || targetAbleBuff_[EnemyTypes::MemoryError]) {
                // add comma at the start of every element except the first one
                if (i != 0) {
                    str.append(", ");
                }
                str.append("Memory Errors");
            }
            break;
        }
        case EnemyTypes::CompilerError:
        {
            if(targetAble_[EnemyTypes::CompilerError] || targetAbleBuff_[EnemyTypes::CompilerError]) {
                // add comma at the start of every element except the first one
                if (i != 0) {
                    str.append(", ");
                }
                str.append("Compiler Errors");
            }
            break;
        }
        default:
            break;
        }

        description_.append(str);
    }

    // close the description
    description_.append("</p>");

    this->setToolTip(description_);
}

void Valgrind::fire(QPointF targetPos) {

    Projectile* projectile = new Projectile(damage_, projectileImagePath_, pierce_, PROJECTILE_STEP_SIZE);
    projectile->setPos(view->getGame()->getSquarePos(y_,x_) -
                       QPoint((projectile->boundingRect().width() - width())/2
                              ,(projectile->boundingRect().height() - height())/2)); //takes the same coordinates as the tower
    QLineF ln(view->getGame()->getSquarePos(y_,x_),targetPos); //path of the projectile
    int angle = -1 * ln.angle(); //the angle from tower to target
    int maxTowerRange = ceil(this->attack_area_->boundingRect().width() * range_ /2);
    projectile->setMaxRange(maxTowerRange);// set max range of the projectile to the range of the tower
    projectile->setMoveFrequency(PROJECTILE_MOVE_INTERVAL);

    //set the projectile image to rotate around it's centre and then add it to the scene
    projectile->setTransformOriginPoint(projectile->pixmap().width()/2,projectile->pixmap().height()/2);
    projectile->setRotation(angle);
    view->getGame()->addItem(projectile);


    // if the angle towards the enemy changes,
    // undo the previous rotation and update to the new rotation angle
    if (this->rotationAngle_ != angle) {
        // the original image rotate by the new angle
        QTransform transform;
        transform.rotate(angle);
        towerImg->setTransformOriginPoint(QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2));
        //towerImg->setTransform(transform);
        towerImg->setRotation(angle);
        //Square::setPixmap(QPixmap(ogImagePath_).transformed(transform).scaled(pixmap().size(), Qt::KeepAspectRatioByExpanding));
        rotationAngle_ = angle;// update the rotation angle
    }


}
