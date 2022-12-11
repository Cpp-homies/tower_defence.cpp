/**
 * @file cs_student.h
 * @authors Hung Vu (hung.h.vu@aalto.fi), Saku (saku.kovanen@aalto.fi)
 * @brief 
 * @version 0.1
 * @date 2022-12-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
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
