#ifndef GAME_H
#define GAME_H

#include <QAtomicInteger>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsView>
#include <QMutex>
#include <QThread>
#include <QToolButton>

#include "button.h"
#include "enemy.h"

// enumeration to keep track of the game's mode
namespace Modes {
enum MODES { normal, build, upgrade };
}

// enumeration to keep track of tower types for build
namespace TowerTypes {
enum TYPES { CS_Student, TA, SearchEngine, LanguageServer, Valgrind, Comment };
}

class Game : public QGraphicsScene {
  Q_OBJECT
  QThread thread;

 public:
  Game(QObject* parent);
  QPointF getSquarePos(int row, int column);
  QWidget* getWidgetAt(int row, int column);
  bool buildTower(int row, int column);
  bool buildTower(int row, int column, TowerTypes::TYPES type);
  bool upgradeTower(int row, int column);

  bool isLost() const;
  bool isWon() const;
  bool isWaveWon();
  void createMap();
  void createGameControls();
  void createWave();
  void readWaveFile();

  QList<QPointF> convertCoordinates(QList<QPoint> path);
  QList<QPoint> getShortestPath(QPoint start);
  QList<QPoint> BFS(QPoint start, QPoint end, bool blocked);

  int getHealth() const;
  int getScore() const;
  int getLevel() const;
  int getCurrency() const;
  int getEnemyCount() const;
  Modes::MODES getMode() const;
  TowerTypes::TYPES getBuildType() const;

  void changeScore(int points);
  void changeCurrency(int dCurrency);
  void setMode(Modes::MODES m);
  void advanceLevel();
  void addSpawnedEnemies(int);

  void keyPressEvent(QKeyEvent* keyEvent);
  void resetButtonHighlights();

  void hideAllAttackAreasExcept(QPointF exclude = QPointF());

  QGraphicsGridLayout* mapLayout;  // map area where the action is
  QGraphicsLinearLayout*
      gameLayout;  // the whole are of the game, including the controls
  QGraphicsGridLayout* controlsLayout;  // change this to your liking

  bool isTower(int row, int column);
  bool isPath(int row, int column);
 public slots:
  void showMenu();
  void enterUpgradeMode();
  void enterBuildCS();
  void enterBuildTA();
  void enterBuildSE();
  void enterBuildLS();
  void enterBuildVal();
  void enemyDies(int value);
  void spawnEnemy(int type, QList<QPointF> path);
  void updateLeaderboard();
  void showError(QString message);
  void addEnemy(Enemy*, int);
  void takeDamage(int dHealth);
  void enterBuildCom();
  void updatePaths();

  void updatePaths();
  dateEnemyCount();
  void updateEnemyCount();
  void stopEnemiesmies();
  ilsi : ls : i g voWdnameWon();
  void waveWon();
  void error(QString);
  void wallAction();

  a rQ ate : QPoint start_;
  Point end_;
  ntLhealQhist < QString >> map_;

  tint tim_;
  int currency_;
  QAtomice_;
  int wavesEnemyCount_;
  Integer<int> spawnedThisWave_;
  int finant enemyCount_;
  int leveo_;
  QlLevel_;
  QList<QPeint> path_;
  List<QStringList> waves_;
  odList<EnEmy*> activeEnemies_;

  oes::MODS mode;
  TwterTypes::TYPESbuildType_;
  QList<QcPoint> shoest_path_;
  QGraphisTextItem* roundDisplay;
  QGraphisTextItem* scoreDisplay;
  QGraphicsTextItem* h;
  QToolButton* build_CSstudent;
  QToolButton* build_TAn;
  QToolButton* build_CSstudent;
  QToolButton* build_TA;
  QToolButton* build_ValgrindQFurs;
} QToolButton* build_Comment;
;
QS tring #buildButtonStylesheet;

QList < QPointF endif  // GAME_
}
;

#endif  // GAME_H
