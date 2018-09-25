//
// Created by determination on 2018/9/23.
//

#ifndef C_WORK_STAFF_H
#define C_WORK_STAFF_H

#include <stdio.h>
#include <stdlib.h>

#define STAFF_TEST 1

#define SIZE 20 // 员工姓名允许的最大长度+1
#define N 4     // 全体员工的数量

typedef enum {
    A, W            // 工种类别
} WorkerType;

typedef union {
    WorkerType type;    // 职务
    int numOfWorkshop;  // 车间编号
} PostOrWorkShop;

typedef struct {
    int num;        // 人员工号
    char name[SIZE];// 姓名
    float salary;   // 工资
    float bonus;    // 奖金
    float total;    // 总收入
    WorkerType type;// 工种类别
    PostOrWorkShop info;// 职务或者车间编号信息
} Worker;

/**
 * 初始化员工信息
 *
 * @param staff 员工结构体指针
 */
void InitStaffInfo(Worker *staff);

/**
 * 输出员工信息
 *
 * @param staff 员工信息
 * @param size 要输出员工信息的数量
 * @param output 将员工信息输出的输出流
 */
void PrintInfo_STAFF(const Worker *staff, int size, FILE *output);

/**
 * 检索奖金大于等于50的员工
 *
 * @param staff 全部员工
 * @param counter 用于计算符合检索条件的员工的数量
 * @return  符合检索条件的员工
 */
Worker *Seek_STAFF(const Worker *staff, int *counter);

/**
 * 计算员工总收入
 *
 * @param staff 全部员工
 */
void Sum_STAFF(Worker *staff);

/**
 * 对全体员工根据员工的总收入进行排序
 *
 * @param staff 全体员工
 */
void Sort_STAFF(Worker *staff);

/**
 * 将排好序的员工信息输出到文件之中
 *
 * @param staff 全体员工
 * @param fileName 输出的文件名称
 */
void outputToFile_STAFF(const Worker *staff, const char *fileName);

/**
 * 单元测试，需要定义 STAFF_TEST 宏
 */
#ifdef STAFF_TEST

void TestStaff(void);

#endif

#endif //C_WORK_STAFF_H
