#include <QApplication>
#include "mainview.h"

MainView * view;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    view = new MainView();

    //change this to showMenu() when ready
    view->showGame();

    view->show();

    return a.exec();
}
