#include "search_engine.h"

#define LVL2_COST 500
// Stats can be set here
#define RANGE 4
#define SPEED_BUFF 1.1

//Tower(int x, int y, int range, int damage, int attackSpeed, QWidget *parent=nullptr);
Search_Engine::Search_Engine(int row, int column, QWidget *parent)
    : Tower(row, column, RANGE, 10, 3000, parent) {
    // set Search Engine stats
    atkSpeedBuffFactor_ = SPEED_BUFF;// set the BuffFactor extremely high for testing
    canFire_ = false;
    attackInterval_ = 0;
    pierce_ = 0;
    targetAble_[EnemyTypes::CompilerError] = false;

    //    atkSpeedBuffFactor_ = 0.2;
    upgradeLevel_ = 1;
    maxLevel_ = 2;
    buffPulseInterval_ = 2000;

    // set Search Engine graphics
    ogImagePath_ = ":/images/Bing.png";
    this->towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    this->towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );

    // get a list of towers in range
    QList<Tower*> towers = getTowersInRange();

    // buff all towers and mark them as buffed
    for (Tower* tower : towers) {
        if (!tower->hasAtkSpdBuff()) {
            // buff the tower
            tower->atkSpeedBuff(atkSpeedBuffFactor_);

            // update the tower's description
            tower->updateDescription();

            // add the buffed tower's location to the list to marked as buffed
            buffedTowers.prepend(tower->getCoords());
        }
    }

    // add the Search Engine tower into the buffedTowers list so it doesn't buff itself
    buffedTowers.prepend(this->getCoords());

    // connect the buffPulse() to the buffPulseInterval
    buffTimer_ = new QTimer(this);
    connect(buffTimer_,SIGNAL(timeout()),this,SLOT(buffPulse()));
    buffTimer_->start(buffPulseInterval_);

    // stop the attackTimer since support tower doesn't shoot
    attackTimer_->stop();
}

Search_Engine::~Search_Engine() {
    // if the game mode is not exit
    if (view->getGame()->getMode() != Modes::exit) {
        // remove the buff from all buffed towers
        for (QPointF point : buffedTowers) {
          QWidget* widget = view->getGame()->getWidgetAt(point.y(), point.x());
          Tower* tower = dynamic_cast<Tower*>(widget);
          if (tower) {
              // debuff the tower
              tower->atkSpeedDebuff(atkSpeedBuffFactor_);

              // if this is level 2, remove the friend towers' ability to target memory errors
              if (this->upgradeLevel_ >= 2) {
                  tower->targetTableDebuff(EnemyTypes::MemoryError);
              }

              // update the tower's description
              tower->updateDescription();
          }
        }
    }


  // schedule to delete the buff timer
  buffTimer_->deleteLater();
}

bool Search_Engine::upgrade() {
  if (upgradeLevel_ >= maxLevel_) {
      // already max level
      return false;
  }
  else {
      // upgrade the tower according to its level
      upgradeLevel_ += 1;

      switch (upgradeLevel_) {
      case 2:
          // if the player has enough money for the upgrade
          // upgrade the tower
          if (view->getGame()->getCurrency() >= LVL2_COST) {
              // update tower graphics
              ogImagePath_ = ":/images/Google.png";
              towerImg->setPixmap(QPixmap(ogImagePath_));

              // increase speed buff
              atkSpeedBuffFactor_ = 1.2;
              damageBuffFactor_ = 2;

              // setRange
              setRange(5);

              // make nearby enemies able to target memory errors
              for (QPointF point : buffedTowers) {
                  QWidget* widget = view->getGame()->getWidgetAt(point.y(), point.x());
                  Tower* tower = dynamic_cast<Tower*>(widget);
                  if (tower) {
                      // make the tower able to target memory errors
                      tower->targetTableBuff(EnemyTypes::MemoryError);
                  }
              }

              // deduct the cost of the tower from player's money
              view->getGame()->changeCurrency(-LVL2_COST);

              // add the cost of the upgrade to tower's total cost
              addCost(LVL2_COST);
          }
          else {
              // upgrade failed
              return false;
          }
          break;
      }

      // update the tower's description
      this->updateDescription();

      return true;
  }
}

void Search_Engine::updateDescription() {
    // create new Tooltip description for this tower
    description_ = QString("<p><b>-Search Engine level %1-</b><br><br>"
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

// function for periodically check for new towers in range and buff them
void Search_Engine::buffPulse() {
  // get all the nearby towers in range
  QList<Tower*> towers = getTowersInRange();

  // loop through all towers in range
  for (Tower* tower : towers) {
      // buff the tower if it hasn't got buff before
      if (!tower->hasAtkSpdBuff() && !buffedTowers.contains(tower->getCoords())) {
          // buff the tower
          tower->atkSpeedBuff(atkSpeedBuffFactor_);

          // if this is level 2, make the towers able to target memory errors
          if (this->upgradeLevel_ >= 2) {
              tower->targetTableBuff(EnemyTypes::MemoryError);
          }

          // update the tower's description
          tower->updateDescription();

          // add the buffed tower's location to the list to marked as buffed
          buffedTowers.prepend(tower->getCoords());
      }
  }
}
