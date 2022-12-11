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
