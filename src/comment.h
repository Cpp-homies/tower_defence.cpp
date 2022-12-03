#ifndef COMMENT_H
#define COMMENT_H

#include "path.h"

class Comment : public Path {
public:
    Comment(int x, int y, int duration, QWidget* parent);
    void changeHealth(int value);
private:
    int duration_;
};

#endif // COMMENT_H
