#ifndef PATH_H
#define PATH_H

#include <QWidget>
#include "square.h"

enum PathType {
    Straight, Turn, TSplit, XSplit, Start, End
};

class Path : public Square {
public:
    Path(int x, int y, PathType type, int rotation, QWidget* parent);
private:

};

#endif // PATH_H
