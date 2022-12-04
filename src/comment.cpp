#include "comment.h"
#include <algorithm>

Comment::Comment(int x, int y, int duration, QWidget* parent) : Path(x, y, CommentType, 0, parent), duration_(duration) {}

void Comment::changeHealth(int value) {
    int afterChange = duration_ + value;
    duration_ = std::max(afterChange, 0);
}
