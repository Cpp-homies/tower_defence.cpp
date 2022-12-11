/**
 * @file button.cpp
 * @author Saku Kovanen (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "button.h"
#include <QBrush>
#include <QGraphicsTextItem>
#include <QFont>
#include <QFontDatabase>
#include <QCursor>

/**
 * @brief Construct a new Button:: Button object. Creates a button.
 * 
 * @param name Text shown on the button.
 * @param w Width of the button.
 * @param h Height of the button.
 * @param backgroundColor Color of the button.
 * @param parent The parent of the button.
 */
Button::Button(QString name, int w, int h, QColor backgroundColor, QGraphicsItem *parent):
    QGraphicsRectItem(parent), backgroundColor_(backgroundColor)
{
    // Draw the rect
    setRect(0,0,w,h);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(backgroundColor_);
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

/**
 * @brief Changes the button color when mouse hovers over the button.
 * 
 */
void Button::hoverEnterEvent(QGraphicsSceneHoverEvent * /* unused */)
{
    // change color to dark green
    if (brush().color() == backgroundColor_) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::darkGreen);
        setBrush(brush);
    }
}

/**
 * @brief Changes the button color back to original color, when mouse no longer hovers over it.
 * 
 */
void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent * /* unused */)
{
    // change color back to green
    if (brush().color() == Qt::darkGreen) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(backgroundColor_);
        setBrush(brush);
    }

}
