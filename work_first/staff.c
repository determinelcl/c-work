//
// Created by determination on 2018/9/23.
//

#include "staff.h"

void outputToFile_STAFF(const Worker *staff, const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if (!file) {
        printf("打开指定文件错误");
        return;
    }

    PrintInfo_STAFF(staff, N, file);

    if (fclose(file) != 0)
        printf("关闭文件失败！");
}

void Sort_STAFF(Worker *staff) {
    for (int i = 0; i < N; ++i) {
        for (int j = i; j < N; ++j) {
            if (staff[i].total > staff[j].total) continue;

            Worker staffTemp = staff[i];
            staff[i] = staff[j];
            staff[j] = staffTemp;
        }
    }
}

void Sum_STAFF(Worker *staff) {
    for (int i = 0; i < N; ++i) {
        staff[i].total = staff[i].salary + staff[i].bonus;
    }
}

Worker *Seek_STAFF(const Worker *staff, int *counter) {

    Worker *result = NULL;

    for (int i = 0; i < N; ++i) {
        if (staff[i].bonus < 50) continue;
        result = realloc(result, ((*counter) + 1) * sizeof(Worker));
        result[(*counter)++] = staff[i];
    }

    return result;
}

void PrintInfo_STAFF(const Worker *staff, int size, FILE *output) {
    fprintf(output, "=============人员信息=============\n");
    for (int i = 0; i < size; i++) {
        fprintf(output, "人员工号：\t%d\n", staff[i].num);
        fprintf(output, "人员姓名：\t%s\n", staff[i].name);
        fprintf(output, "人员工资：\t%.2f\n", staff[i].salary);
        fprintf(output, "人员奖金：\t%.2f\n", staff[i].bonus);

        staff[i].total == -1 ?
                fprintf(output, "人员总收入：\t%s\n", "未计算")
                             :
                fprintf(output, "人员总收入：\t%.2f\n", staff[i].total);

        fprintf(output, "人员类别：\t%s\n", staff[i].type == A ? "高级管理者" : "工人");

        staff[i].type == A ?
                fprintf(output, "人员职务：\t高级管理者\n")
                           :
                fprintf(output, "车间编号：\t%d\n", staff[i].info.numOfWorkshop);
        fprintf(output, "\n\n");
    }
}

void InitStaffInfo(Worker *staff) {
    Worker tempInfo[N] = {
            {1, "Zhanglin", 4200, 600, -1, A, A},
            {2, "Lifeng",   2300, 40,  -1, W, 1002},
            {3, "King",     5200, 700, -1, A, A},
            {4, "Jack",     3300, 60,  -1, W, 1003}
    };

    for (int i = 0; i < N; i++) {
        staff[i] = tempInfo[i];
    }
}

#ifdef STAFF_TEST

void TestStaff() {
    Worker staff[N];

    // 初始化并输出
    InitStaffInfo(staff);
    PrintInfo_STAFF(staff, N, stdout);

    // 根据条件检索员工信息并输出
    int counter = 0;
    Worker *seek = Seek_STAFF(staff, &counter);
    PrintInfo_STAFF(seek, counter, stdout);

    // 计算员工总收入并输出
    Sum_STAFF(staff);
    PrintInfo_STAFF(staff, N, stdout);

    // 根据员工总收入并输出
    Sort_STAFF(staff);
    PrintInfo_STAFF(staff, N, stdout);

    // 将员工信息输出到指定的文件中
    outputToFile_STAFF(staff, "./Perout.dat");
}

#endif