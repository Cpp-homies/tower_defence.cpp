#ifndef CS_STUDENT_H
#define CS_STUDENT_H

#include "tower.h"

/**
 * @brief A CS Student tower type. Can attack enemies in range. Inherited from Tower class.
 */
class CS_Student : public Tower {
public:
    CS_Student();
    CS_Student(int row, int column, QWidget *parent=nullptr);
    virtual bool upgrade();

};

#endif // CS_STUDENT_H
