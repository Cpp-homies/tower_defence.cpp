#ifndef CS_STUDENT_H
#define CS_STUDENT_H

#include "tower.h"

class CS_Student : public Tower {
public:
    CS_Student();
    CS_Student(int row, int column, QWidget *parent=nullptr);
};

#endif // CS_STUDENT_H
