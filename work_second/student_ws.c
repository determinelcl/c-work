//
// Created by determination on 2018/10/22.
//

#include "student_ws.h"


StudentPtr_MS newEmptyStudent_MS() {
    StudentPtr_MS student = malloc(sizeof(Student_MS));
    student->no = EMPTY_SNO;
    student->name = "";
    student->age = 0;
    student->year = "";
    student->address = "";
    student->phone = "";
    student->email = "";
    return student;
}

StudentPtr_MS newStudent_MS(Integer sno, String name,
                            Integer age, String year, String address, String phone, String email) {

    StudentPtr_MS student = malloc(sizeof(Student_MS));
    student->no = sno;
    student->name = name;
    student->age = age;
    student->year = year;
    student->address = address;
    student->phone = phone;
    student->email = email;
    return student;
}

void ToString_MS(StudentPtr_MS student, FILE *output) {
    fprintf(output, "学号：\t\t%d\n", student->no);
    fprintf(output, "姓名：\t\t%s\n", student->name);
    fprintf(output, "年龄：\t\t%d\n", student->age);
    fprintf(output, "出生年月：\t%s\n", student->year);
    fprintf(output, "地址：\t\t%s\n", student->address);
    fprintf(output, "电话：\t\t%s\n", student->phone);
    fprintf(output, "邮箱：\t\t%s\n", student->email);
}