#ifndef PATH_H
#define PATH_H

#include <QWidget>
#include "square.h"

enum PathType {
    Start, Middle, End
};

enum PathConfig {
    Straight, Turn, TSplit, XSplit
};

class Path : public Square {
public:
    Path(int x, int y, PathType type, PathConfig config, int rotation, QWidget* parent);
private:

};

#endif // PATH_H
