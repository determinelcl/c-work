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

/**
 * 运行列表分页显示模块
 *
 * @param students 全体学生集合
 */
void RunListPageModel(ArrayListPtr students);

/**
 * 运行根据学生学号查询信息模块
 *
 * @param students 全体学生集合
 */
void RunFindBySnoModel(ArrayListPtr students);

/**
 * 运行根据学生姓名查询信息模块
 *
 * @param students 全体学生集合
 */
void RunFindByNameModel(ArrayListPtr students);

/**
 * 运行根据学生姓名进行排序模块
 *
 * @param students 全体学生集合
 */
void RunSortByNameModel(ArrayListPtr students);

/**
 * 运行根据学生学号进行排序模块
 *
 * @param students 全体学生集合
 */
void RunSortBySnoModel(ArrayListPtr students);

/**
 * 运行学生信息录入模块
 *
 * @param students 全体学生集合
 */
void RunInputStudentInfoModel(ArrayListPtr students);

/**
 * 连接数据库
 *
 * @return 数据库的全部信息集合
 */
ArrayListPtr RunOpenDBModel(void);

/**
 * 工具函数：用于忽略输入缓冲区的换行字符
 */
void IgnoreInputN(void);

/**
 * 运行更新学生信息模块
 *
 * @param students 全体学生集合
 */
void RunUpdateStudentModel(ArrayListPtr students);

/**
 * 运行删除学生信息模块
 *
 * @param students 全体学生集合
 */
void RunDeleteStudentModel(ArrayListPtr students);

#endif //C_WORK_STUDENT_DRIVER_SYS_H
