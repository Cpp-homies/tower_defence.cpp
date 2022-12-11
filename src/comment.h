/**
 * @file comment.h
 * @authors Harvey Lim (harvey.lim@aalto.fi)
 * @brief File containing the comment tower class and its members.
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef COMMENT_H
#define COMMENT_H

#include "path.h"
#include <QPointF>
#include <QTimer>
#include "mainview.h"

extern MainView* view;
/**
 * @brief The Comment class. Represents the comment tower.
 */
class Comment : public Path {
public:
    Comment(int x, int y, int duration, Path* old, QWidget* parent);
    QPointF towerCenter();
    Path* getOld();
    ~Comment();
    bool isTimerActive();
    void startTimer();
public slots:
    void breakComment();
private:
    /**
     * @brief Duration of the breakdown timer.
     */
    int duration_;
    /**
     * @brief Pixmap image of the Comment tower.
     */
    QGraphicsPixmapItem * towerImg;
    /**
     * @brief The old path which the comment tower replaced when it was built.
     */
    Path* old_;
    /**
     * @brief A timer which starts counting down to the destruction of the comment tower. Has a duration determined by duration_.
     */
    QTimer* breakdownTimer_;
    /**
     * @brief The x-coordinate of the Comment tower.
     */
    int x_;
    /**
     * @brief The y-coordinate of the Comment tower.
     */
    int y_;
};

#endif // COMMENT_H
