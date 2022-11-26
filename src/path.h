#ifndef PATH_H
#define PATH_H

#include "square.h"
#include "comment.h"

enum PathType {
    Start, End, Middle
};

class Path : public Square {
public:
    Path(QPoint pos, PathType type = Middle);
    bool buildComment();
    bool hasComment();
    PathType getType();
    QPoint getPos();
private:
    Comment comment_;
    PathType type_;
};

#endif // PATH_H
