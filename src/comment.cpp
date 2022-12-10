#include "comment.h"
#include "mainview.h"
#include <algorithm>

extern MainView * view;

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

void Comment::breakComment() {
    view->getGame()->deleteComment(y_, x_);
}

void Comment::changeHealth(int value) {
    int afterChange = duration_ + value;
    duration_ = std::max(afterChange, 0);
}

Path* Comment::getOld() {
    return old_;
}

Comment::~Comment() {
    view->getGame()->removeItem(towerImg);
    delete towerImg;
    breakdownTimer_->deleteLater();
}

QPointF Comment::towerCenter() {
    QPoint towerPos = view->getGame()->getSquarePos(y_, x_).toPoint();
    QPointF center(towerPos.x() + this->pixmap().width()/2,
                   towerPos.y() + this->pixmap().height()/2);

    return center;
}

void Comment::startTimer() {
    if (!breakdownTimer_->isActive()) {
        breakdownTimer_->start(duration_);
    }
}
