#ifndef MENU_H
#define MENU_H
#include <QGraphicsView>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>

#include <QGraphicsScene>

class Menu : public QGraphicsScene
{
    Q_OBJECT
public:
    Menu(QObject *parent = nullptr);

    QGraphicsLinearLayout * menuLayout;
public slots:
    void showGame();
    void showLeaderboard();
    void quit();
private:
};

#endif // MENU_H
