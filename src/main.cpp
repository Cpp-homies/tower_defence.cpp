#include <QApplication>
#include "mainview.h"

MainView * view;
/**
 * @brief The main function of the tower_defense program
 * @param argc The number of arguments provided when launching the program via the command line.
 * @param argv An array of strings, containing the arguments provided when launching the program from the command line.
 * @return an integer; 0 if the program ran successfully, another number if otherwise.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    view = new MainView();

    //change this to showMenu() when ready
    view->showGame();

    view->show();

    return a.exec();
}
