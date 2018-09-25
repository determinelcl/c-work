//
// Created by determination on 2018/9/23.
//

#ifndef C_WORK_CLASSROOM_H
#define C_WORK_CLASSROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef enum {
    DOCTOR,         // 博士
    MASTER,         // 硕士
    UNDERGRADUATE   // 本科生
} Education;

typedef struct NodeRecord {
    Education education;        // 学历
    int num;                    // 人数
    struct NodeRecord *next;    // 指向链表的下一个节点
} Classroom;

#define CLASSROOM_TEST 1    // 班级信息测试用例的开关
#define N_CR 3  // 班级信息数组的大小

/**
 * 初始化班级信息
 *
 * @param classroom 存放班级信息的数组
 */
void InitInfo_CR(Classroom *classroom);

/**
 * 将班级信息的数组转换为带头节点的班级链表
 *
 * @param classroom 班级信息数组
 * @return 班级信息的链表
 */
Classroom *SwitchToLinkedList(Classroom *classroom);

/**
 * 输出班级信息
 *
 * @param classroom 班级信息的链表
 * @param output 输出流
 */
void PrintInfo_CR(const Classroom *classroom, FILE *output);

/**
 * 根据学历的枚举值转换为字符串
 *
 * @param education 学历枚举
 * @return 对应学历的字符串
 */
char *ToEduString_CR(Education education);

/**
 * 将班级信息输出到指定的文件中
 *
 * @param classroom 班级信息的链表
 * @param filename 指定的文件名称
 */
void OutputToFile_CR(Classroom *classroom, char *filename);

/**
 * 班级信息测试用例
 */
#ifdef CLASSROOM_TEST

void TestClassroom(void);

#endif

#endif //C_WORK_CLASSROOM_H
