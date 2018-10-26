//
// Created by determination on 2018/10/26.
//

#include "sys_driver.h"

static char *const DB_URL = "./students.json";

void StudentInfoSystemDriver_MS(void) {
    ShowMenu_MS(stdout);
    fprintf(stdout, "\n\n请输入您需要的操作\n\n");
    ArrayListPtr students = NULL;
    int item = '\n';
    do {
        if (item != '\n')
            IgnoreInputN();

        switch (item) {
            case 'a':
                students = RunOpenDBModel();
                break;
            case 'b':
                RunInputStudentInfoModel(students);
                break;
            case 'c':
                ShowInfo_MS(students, stdout);
                break;
            case 'd':
                RunListPageModel(students);
                break;
            case 'e':
                RunFindBySnoModel(students);
                break;
            case 'f':
                RunFindByNameModel(students);
                break;
            case 'g':
                RunSortBySnoModel(students);
                break;
            case 'h':
                RunSortByNameModel(students);
                break;
            case 'i':
                RunUpdateStudentModel(students);
                break;
            case 'j':
                RunDeleteStudentModel(students);
                break;
            case 'k':
                SyncInfo_MS(students, DB_URL);
                break;
            default:
                if (item != '\n')
                    fprintf(stdout, "输入不合法！\n");
        }

        fprintf(stdout, "admin@student-ms:$ ");

    } while ((item = getchar()) != 'q');

    SyncInfo_MS(students, DB_URL);
}

void RunDeleteStudentModel(ArrayListPtr students) {
    int item = 'n';
    int sno;
    StudentPtr_MS studentTemp = newEmptyStudent_MS();
    do {
        fprintf(stdout, "请输入学号：");
        if (item == 'y') IgnoreInputN();

        fscanf(stdin, "%d", &sno);
        IgnoreInputN();
        StudentPtr_MS student = FindBySno_MS(students, sno);
        if (student) {
            ToString_MS(student, stdout);

            fprintf(stdout, "是否进行删除?(y/n)：");
            if (getchar() == 'y') {
                IgnoreInputN();

                String delRs = DeleteStudentBySno_MS(students, sno, studentTemp) ? "删除成功" : "删除失败";
                fprintf(stdout, "%s\n", delRs);
            }

        } else fprintf(stdout, "学号为%d的学生信息不存在\n", sno);

        fprintf(stdout, "是否继续输入?(y/n)：");
    } while ((item = getchar()) == 'y');
    IgnoreInputN();
}

void RunUpdateStudentModel(ArrayListPtr students) {
    int item = 'n';
    int sno;
    do {
        fprintf(stdout, "请输入学号：");
        if (item == 'y') IgnoreInputN();

        fscanf(stdin, "%d", &sno);
        IgnoreInputN();
        StudentPtr_MS student = FindBySno_MS(students, sno);
        if (student) {
            ToString_MS(student, stdout);

            fprintf(stdout, "请输入需要修改的学生信息：[姓名 年龄 出生年月 地址 电话 邮箱]\n");
            fscanf(stdin, "%s", student->name);
            fscanf(stdin, "%d", &student->age);
            fscanf(stdin, "%s", student->year);
            fscanf(stdin, "%s", student->address);
            fscanf(stdin, "%s", student->phone);
            fscanf(stdin, "%s", student->email);

            String rsStr = UpdateStudentBySno_MS(students, sno, student) ? "更新成功" : "更新失败";
            fprintf(stdout, "%s\n", rsStr);
        } else fprintf(stdout, "学号为%d的学生信息不存在\n", sno);

        fprintf(stdout, "是否继续输入?(y/n)：");
    } while ((item = getchar()) == 'y');
    IgnoreInputN();
}

void IgnoreInputN() { while (getchar() != '\n') continue; }

ArrayListPtr RunOpenDBModel() {
    ArrayListPtr students = OpenStudentDB_MS(DB_URL);
    if (students) printf("打开完成\n");
    else {
        printf("打开数据库失败，正在退出系统...\n");
        exit(EXIT_FAILURE);
    }
    return students;
}

void RunInputStudentInfoModel(ArrayListPtr students) {
    do {
        fprintf(stdout, "请输入学生信息：[学号 姓名 年龄 出生年月 地址 电话 邮箱]\n");
        InputStudentInfo_MS(students, stdin);
        fprintf(stdout, "输入完成\n是否继续输入?(y/n)：");
    } while (getchar() == 'y');
    IgnoreInputN();
}

void RunSortBySnoModel(ArrayListPtr students) {
    int item;
    ArrayListPtr sortedList;
    fprintf(stdout, "按照学号排序，a：升序，d：降序，其他字符退出\n");
    do {
        fprintf(stdout, "sort@model:$ \n");

        item = getchar();
        IgnoreInputN();

        if (item != 'a' && item != 'd') break;

        if (item == 'a') {
            fprintf(stdout, "按照学生学号进行升序排序，排序结果：\n");
            sortedList = SortBySno_MS(students, ASC);
        } else {
            fprintf(stdout, "按照学生学号进降序排序，排序结果：\n");
            sortedList = SortBySno_MS(students, DESC);
        }

        ShowInfo_MS(sortedList, stdout);
    } while (true);
    IgnoreInputN();
}

void RunSortByNameModel(ArrayListPtr students) {
    int item;
    fprintf(stdout, "按照学生姓名排序，a：升序，d：降序，其他字符退出\n");
    ArrayListPtr sortedList;
    do {
        fprintf(stdout, "sort@model:$ \n");
        item = getchar();
        IgnoreInputN();

        if (item != 'a' && item != 'd') break;

        if (item == 'a') {
            fprintf(stdout, "按照学生姓名进行升序排序，排序结果：\n");
            sortedList = SortBySno_MS(students, ASC);
        } else {
            fprintf(stdout, "按照学生姓名进行降序排序，排序结果：\n");
            sortedList = SortBySno_MS(students, DESC);
        }

        ShowInfo_MS(sortedList, stdout);
    } while (true);
    IgnoreInputN();
}


void RunFindByNameModel(ArrayListPtr students) {
    int item = 'n';
    String name = NewString(20);
    do {
        fprintf(stdout, "请输入姓名：");
        if (item == 'y') IgnoreInputN();

        fscanf(stdin, "%s", name);
        IgnoreInputN();
        StudentPtr_MS student = FindByName_MS(students, name);
        if (student)
            ToString_MS(student, stdout);
        else fprintf(stdout, "姓名为%s的学生信息不存在\n", name);

        fprintf(stdout, "是否继续输入?(y/n)：");
    } while ((item = getchar()) == 'y');
    IgnoreInputN();
}

void RunFindBySnoModel(ArrayListPtr students) {
    int item = 'n';
    int sno;
    do {
        fprintf(stdout, "请输入学号：");
        if (item == 'y') IgnoreInputN();

        fscanf(stdin, "%d", &sno);
        IgnoreInputN();
        StudentPtr_MS student = FindBySno_MS(students, sno);
        if (student)
            ToString_MS(student, stdout);
        else fprintf(stdout, "学号为%d的学生信息不存在\n", sno);

        fprintf(stdout, "是否继续输入?(y/n)：");
    } while ((item = getchar()) == 'y');
    IgnoreInputN();
}

void RunListPageModel(ArrayListPtr students) {
    // 选项初始值用于显示第一页数据
    int item = '0';

    // 初始化page对象
    Page_MS page = {
            1, 0, 10, 0, true, false
    };
    do {
        // 浏览下一页信息
        if (item == 'n') {
            IgnoreInputN();
            if (page.last) printf("当前页是最后一页!\n");
            else {
                page.currPage++;
                page = ShowInfoPage_MS(students, page, stdout);
            }
        } else if (item == 'p') {   // 浏览上一页信息
            IgnoreInputN();
            if (page.first) printf("当前页为第一页!\n");
            else {
                page.currPage--;
                page = ShowInfoPage_MS(students, page, stdout);
            }
        } else if (item == '0') {   // 用作初始化页面的输出
            page.currPage = 1;
            page = ShowInfoPage_MS(students, page, stdout);
        }
        if (item != '\n')
            fprintf(stdout, "输入n获取下一页，输入p获取上一页，其他字符退出\n");

        fprintf(stdout, "page@model:$ ");
    } while ((item = getchar()) == 'n' || item == 'p' || item == '\n');
    IgnoreInputN();
}