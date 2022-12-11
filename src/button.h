/**
 * @file button.h
 * @author Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

/**
 * @brief This is the button object.
 * 
 */
class Button:public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    Button(QString name, int w, int h, QColor backgroundColor = Qt::green
            , QGraphicsItem* parent =NULL);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

signals:
    void clicked();

private:
    /**
     * @brief Text displayed on button.
     * 
     */
    QGraphicsTextItem * text;

    /**
     * @brief Color of the button.
     * 
     */
    QColor backgroundColor_;
};

#endif // BUTTON_H
