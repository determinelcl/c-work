//
// Created by determination on 2018/9/23.
//

#include "classroom.h"


void OutputToFile_CR(Classroom *classroom, char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("打开指定文件错误");
        return;
    }

    PrintInfo_CR(classroom, file);

    if (fclose(file) != 0)
        printf("关闭文件失败！");
}

void PrintInfo_CR(const Classroom *classroom, FILE *output) {
    fprintf(output, "============班级信息============\n");

    Classroom *pos = classroom->next;
    int counter = 1;
    while (pos) {
        fprintf(output, "班级：\t%d\n", counter++);
        fprintf(output, "学历：\t%s\n", ToEduString_CR(pos->education));
        fprintf(output, "人数：\t%d\n", pos->num);
        fprintf(output, "\n\n");
        pos = pos->next;
    }
}

char *ToEduString_CR(Education education) {
    switch (education) {
        case DOCTOR:
            return "博士";
        case MASTER:
            return "硕士";
        case UNDERGRADUATE:
            return "本科生";
        default:
            return "未知";
    }
}

Classroom *SwitchToLinkedList(Classroom *classroom) {
    Classroom *head = malloc(sizeof(Classroom));
    assert(head);

    for (int i = 0; i < N_CR; i++) {
        if (i + 1 < N_CR)
            classroom[i].next = &classroom[i + 1];
        else
            classroom[i].next = NULL;
    }

    head->next = &classroom[0];
    return head;
}

void InitInfo_CR(Classroom *classroom) {
    Classroom temCell[N_CR] = {
            {DOCTOR,        5,  NULL},
            {MASTER,        20, NULL},
            {UNDERGRADUATE, 40, NULL}
    };

    for (int i = 0; i < N_CR; ++i) {
        classroom[i] = temCell[i];
    }
}


#ifdef CLASSROOM_TEST

void TestClassroom() {
    Classroom classroom[N_CR];
    InitInfo_CR(classroom);
    Classroom *classroomPtr = SwitchToLinkedList(classroom);
    PrintInfo_CR(classroomPtr, stdout);
    OutputToFile_CR(classroomPtr, "./classout.dat");
}

#endif