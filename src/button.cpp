#include "button.h"
#include <QBrush>
#include <QGraphicsTextItem>
#include <QFont>
#include <QFontDatabase>
#include <QCursor>

Button::Button(QString name, int w, int h, QGraphicsItem *parent): QGraphicsRectItem(parent)
{
    // Draw the rect
    setRect(0,0,w,h);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    setBrush(brush);

    // draw the text
    text = new QGraphicsTextItem(name, this);
    QFont buttonFont("Roboto", 24);
    text->setFont(buttonFont);
    int xPos = rect().width()/2 - text->boundingRect().width()/2;
    int yPos = rect().height()/2 - text->boundingRect().height()/2;
    text->setPos(xPos, yPos);

    // allow responding to hover events
    setAcceptHoverEvents(true);

    // set cursor when hovering over to pointer
    setCursor(QCursor(Qt::PointingHandCursor));
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent */* unused */)
{
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent */* unused */)
{
    // change color to dark green
    if (brush().color() == Qt::green) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::darkGreen);
        setBrush(brush);
    }
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent * /* unused */)
{
    // change color back to green
    if (brush().color() == Qt::darkGreen) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::green);
        setBrush(brush);
    }

}
