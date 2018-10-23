//
// Created by determination on 2018/10/22.
//

#ifndef C_WORK_STUDENT_WS_H
#define C_WORK_STUDENT_WS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define EMPTY_SNO -1


typedef char *String;
typedef int Integer;

/**
 * 学生结构体
 */
typedef struct Student_MS {
    Integer no;         // 学号
    String name;        // 姓名
    Integer age;        // 年龄
    String year;        // 出生年月
    String address;     // 地址
    String phone;       // 电话
    String email;       // 邮箱
} Student_MS, *StudentPtr_MS;

/**
 * 结构体元数据定义
 */
#define SNO_MS      "no"
#define NAME_MS     "name"
#define AGE_MS      "age"
#define YEAR_MS     "year"
#define ADDRESS_MS  "address"
#define PHONE_MS    "phone"
#define EMAIL_MS    "email"

/**
 * 新建一个学生结构体对象
 *
 * @return 学生结构体对象
 */
StudentPtr_MS newEmptyStudent_MS(void);

/**
 * 创建学生结构体
 *
 * @param sno 学号
 * @param name 姓名
 * @param age 年龄
 * @param year 出生年月
 * @param address 地址
 * @param phone 电话
 * @param email 邮箱
 * @return 学生结构体
 */
StudentPtr_MS newStudent_MS(Integer sno, String name,
                            Integer age, String year, String address, String phone, String email);

/**
 * 显示学生结构体信息
 *
 * @param student 要显示的结构体信息
 * @param output 指定输出流
 */
void ToString_MS(StudentPtr_MS student, FILE *output);


#endif //C_WORK_STUDENT_WS_H
