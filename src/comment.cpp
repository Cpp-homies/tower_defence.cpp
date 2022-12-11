/**
 * @file comment.cpp
 * @authors rv (harvey.lim@aalto.fi)
 * @brief File containing the comment tower class methods.
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "comment.h"
#include "mainview.h"
#include <algorithm>

extern MainView * view;
/**
 * @brief Comment::Comment Constructor function for Comment towers.
 * @param x The x-coordinate of the comment to be created.
 * @param y The y-coordinate of the comment to be created.
 * @param duration The amount of time enemies need to break down the Comment tower.
 * @param old The path which the comment tower will be placed over.
 * @param parent The parent widget of the Comment tower
 */
Comment::Comment(int x, int y, int duration, Path* old, QWidget* parent)
    : Path(x, y, CommentType, 0, old, parent), duration_(duration), old_(old), x_(x), y_(y) {

    // set tower graphics
    QString ogImagePath_ = ":/images/Comment.png";
    towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );
    breakdownTimer_ = new QTimer(this);
    breakdownTimer_->setSingleShot(true);
    connect(breakdownTimer_, &QTimer::timeout, this, &Comment::breakComment);
}

/**
 * @brief Comment::breakComment Deletes the Comment tower.
 */
void Comment::breakComment() {
    view->getGame()->deleteComment(y_, x_);
}

/**
 * @brief Comment::getOld Returns the old path which was replaced when the Comment tower was built.
 * @return The path the Comment tower previously replaced.
 */
Path* Comment::getOld() {
    return old_;
}

/**
 * @brief Comment::~Comment The destructor for the Comment tower.
 */
Comment::~Comment() {
    view->getGame()->removeItem(towerImg);
    delete towerImg;
    breakdownTimer_->deleteLater();
}

/**
 * @brief Comment::towerCenter Gets the absolute position of the Comment tower.
 * @return The absolute position of the Comment tower.
 */
QPointF Comment::towerCenter() {
    QPoint towerPos = view->getGame()->getSquarePos(y_, x_).toPoint();
    QPointF center(towerPos.x() + this->pixmap().width()/2,
                   towerPos.y() + this->pixmap().height()/2);

    return center;
}

/**
 * @brief Comment::startTimer Starts the breakdown timer of the Comment tower.
 */
void Comment::startTimer() {
    if (!breakdownTimer_->isActive()) {
        breakdownTimer_->start(duration_);
    }
}
