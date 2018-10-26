//
// Created by determination on 2018/10/26.
//

#ifndef C_WORK_STUDENT_DRIVER_SYS_H
#define C_WORK_STUDENT_DRIVER_SYS_H

/**
 * 学生信息管理系统
 */

#include "student_sys.h"

/**
 * 学生信息系统驱动程序
 */
void StudentInfoSystemDriver_MS(void);

void RunListPageModel(ArrayListPtr students);

void RunFindBySnoModel(ArrayListPtr students);

void RunFindByNameModel(ArrayListPtr students);

void RunSortByNameModel(ArrayListPtr students);

void RunSortBySnoModel(ArrayListPtr students);

void RunInputStudentInfoModel(ArrayListPtr students);

ArrayListPtr RunOpenDBModel();

void IgnoreInputN();

void RunUpdateStudentModel(ArrayListPtr students);

void RunDeleteStudentModel(ArrayListPtr students);

#endif //C_WORK_STUDENT_DRIVER_SYS_H
