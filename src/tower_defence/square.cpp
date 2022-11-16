#include "square.h"

Square::Square(QWidget *parent)
    : QLabel(parent)
{
    setPixmap(QPixmap(":/images/tile.jpeg"));
}

void Square::mousePressEvent(QMouseEvent* /* unused */){

    setPixmap(QPixmap(":/images/desk.png"));
}
