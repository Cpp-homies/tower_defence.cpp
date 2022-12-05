#include "comment.h"
#include "mainview.h"
#include <algorithm>

extern MainView * view;

Comment::Comment(int x, int y, int duration, Path* old, QWidget* parent)
    : Path(x, y, CommentType, 0, old, parent), duration_(duration), old_(old) {

    // set tower graphics
    QString ogImagePath_ = ":/images/Comment.png";
    towerImg = view->getGame()->addPixmap(QPixmap(ogImagePath_));
    towerImg->setPos(towerCenter() - QPoint(towerImg->boundingRect().width()/2, towerImg->boundingRect().height()/2) );

}

void Comment::changeHealth(int value) {
    int afterChange = duration_ + value;
    duration_ = std::max(afterChange, 0);
}


QPointF Comment::towerCenter() {
    QPoint towerPos = view->getGame()->getSquarePos(y_, x_).toPoint();
    QPointF center(towerPos.x() + this->pixmap().width()/2,
                   towerPos.y() + this->pixmap().height()/2);

    return center;
}
