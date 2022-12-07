#include "search_engine.h"

#define LVL2_COST 15

//Tower(int x, int y, int range, int damage, int attackSpeed, QWidget *parent=nullptr);
Search_Engine::Search_Engine(int row, int column, QWidget *parent) : Tower(row, column, 4, 10, 3000, parent) {
    // set Search Engine stats
    atkSpeedBuffFactor_ = 5;// set the BuffFactor extremely high for testing
    canFire_ = false;
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
      // buff the tower
      tower->atkSpeedBuff(atkSpeedBuffFactor_);

      // add the buffed tower's location to the list to marked as buffed
      buffedTowers.prepend(tower->getCoords());
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

      return true;
  }
}

// function for periodically check for new towers in range and buff them
void Search_Engine::buffPulse() {
  // get all the nearby towers in range
  QList<Tower*> towers = getTowersInRange();

  // loop through all towers in range
  for (Tower* tower : towers) {
      // buff the tower if it hasn't got buff before
      if (!buffedTowers.contains(tower->getCoords())) {
          // buff the tower
          tower->atkSpeedBuff(atkSpeedBuffFactor_);

          // if this is level 2, make the towers able to target memory errors
          if (this->upgradeLevel_ >= 2) {
              tower->targetTableBuff(EnemyTypes::MemoryError);
          }

          // add the buffed tower's location to the list to marked as buffed
          buffedTowers.prepend(tower->getCoords());
      }
  }
}
