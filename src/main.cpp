/**
 * @file main.cpp
 * @authors Siim Kasela (siim.kasela@aalto.fi), Saku Kovanen (saku.kovanen@aalto.fi), rv (harvey.lim@aalto.fi), Hung Vu (hung.h.vu@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <QApplication>
#include "mainview.h"

MainView * view;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/images/CStudent1.png"));

    view = new MainView();

    //change this to showMenu() when ready
    view->showMenu();

    view->show();
    QObject::connect(&a,&QApplication::aboutToQuit,[]()
    {
        view->getGame()->setMode(Modes::exit);
        delete view;
    });
    return a.exec();
}
