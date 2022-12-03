#include "cs_student.h"
#include "tower.h"

CS_Student::CS_Student(int row, int column, QWidget *parent) : Tower(row, column, parent) {

}

// return the cost of CS_Student tower,
// current set cost is temporarily and might be subjected to change
int CS_Student::cost() {
    return 20;
}
