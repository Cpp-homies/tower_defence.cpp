/**
 * @file menu.h
 * @authors Saku Kovanen (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MENU_H
#define MENU_H
#include <QGraphicsView>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsScene>

/**
 * @brief This is the menu scene.
 * 
 */
class Menu : public QGraphicsScene
{
    Q_OBJECT
public:
    Menu(QObject *parent = nullptr);
public slots:
    void showGame(int gamemode); // 0 = sandbox, 1 = easy, 2 = hard
    void showLeaderboard();
    void quit();
private:
};

#endif // MENU_H
