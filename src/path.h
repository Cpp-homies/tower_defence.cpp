#ifndef PATH_H
#define PATH_H

#include <QWidget>
#include "square.h"

enum PathType {
    Straight, Turn, TSplit, XSplit, Start, End, CommentType
};

class Path : public Square {
public:
    Path(int x, int y, PathType type, int rotation, Path* old = nullptr, QWidget* parent = nullptr);
    PathType getType();
    int getRotation();
private:
    bool isComment_;
    Path* old_;
    PathType type_;
    int rotation_;
};

#endif // PATH_H
