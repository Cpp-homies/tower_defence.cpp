/**
 * @file path.h
 * @authors rv (harvey.lim@aalto.fi)
 * @brief File containing the path tile class definition and its members.
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PATH_H
#define PATH_H

#include <QWidget>
#include "square.h"

enum PathType {
    Straight, Turn, TSplit, XSplit, Start, End, CommentType
};

/**
 * @brief The Path class. Represents the path tiles on which enemies travel on.
 */
class Path : public Square {
public:
    Path(int x, int y, PathType type, int rotation, Path* old = nullptr, QWidget* parent = nullptr);
    PathType getType();
    int getRotation();
private:
    /**
     * @brief A boolean stating if this path has a Comment tower on it.
     */
    bool isComment_;
    /**
     * @brief If this path contains a comment tower, this will hold the path tile the comment tower replaced when it was built.
     */
    Path* old_;
    /**
     * @brief Denotes which type of path this is.
     */
    PathType type_;
    /**
     * @brief The rotational orientation of the path, in degrees.
     */
    int rotation_;
};

#endif // PATH_H
