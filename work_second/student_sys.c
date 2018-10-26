//
// Created by determination on 2018/10/22.
//

#include "student_sys.h"

#define OPEN_DB_FIELD -1
#define DB_NOT_OPEN -2
#define READ_STR_LEN 100
#define STUDENTS_MS "Students"

/**
 * 加载Json数据到内存
 *
 * @param json 要加载的Json的数据
 * @return 加载的数据的集合
 */
ArrayListPtr LoadJsonToTheMemory(String json);

/**
 * 将Json中匹配的String字段的数据赋值给相应的String字段
 *
 * @param field 接受赋值的字段
 * @param json 数据载体结构对象
 */
void AssignStrValue_MS(String field, const cJSON *json);

/**
 * 将Json中匹配的Integer字段的数据赋值给相应的Integer字段
 *
 * @param field 接受赋值的字段
 * @param json 数据载体结构对象
 */
void AssignIntValue_MS(Integer *field, const cJSON *json);

/**
 * 用于判断集合是否为空，如果为NULL则说明是未从数据库文件中加载数据
 *
 * @param students 全体学生集合
 */
void AssertDBOpened_MS(ArrayListPtr students);

/**
 * 打印表头
 *
 * @param output 输出流
 */
void ShowTableHeader_MS(FILE *output);

/**
 * 输出表的信息
 *
 * @param students 全体学生信息集合
 * @param index 输出的第几个学生信息的索引值
 * @param output 输出流
 */
void ShowTableBodyInfo_MS(ArrayListPtr students, int index, FILE *output);

/**
 * 根据学号查询学生信息在集合中的索引位置
 *
 * @param students 全体学生集合
 * @param sno 学生学号
 * @return 学生信息在集合中的索引位置
 */
int IndexOf_MS(ArrayListPtr students, Integer sno);

/**
 * 根据学生字段来查询学生信息的通用函数
 *
 * @param students 全体的学生集合
 * @param field 比较字段的函数指针
 * @return 查询到的学生结构对象
 */
StudentPtr_MS FindByField_MS(ArrayListPtr students, void *field);

ArrayListPtr OpenStudentDB_MS(String url) {
    FILE *db = fopen(url, "r");

    if (!db) {
        printf("数据库打开失败！正在退出系统...\n");
        exit(OPEN_DB_FIELD);
    }

    fseek(db, 0L, SEEK_END); /* 定位到文件末尾 */
    long fileLen = ftell(db);
    fseek(db, 0L, SEEK_SET);

    String json = NewString(fileLen + 1);
    String temp = NewString(fileLen + 1);
    while (fgets(temp, READ_STR_LEN, db))
        strncat(json, temp, strlen(temp));

    if (fclose(db) != 0)
        printf("数据库文件挂起失败！可能会使您的数据遭到破坏\n");

    return LoadJsonToTheMemory(json);
}

ArrayListPtr LoadJsonToTheMemory(String json) {
    ArrayListPtr arrayList = newArrayList();

    cJSON *root = cJSON_Parse(json);
    if (!root) return arrayList;

    cJSON *studentsJson = cJSON_GetObjectItem(root, STUDENTS_MS);
    if (!studentsJson) return arrayList;

    for (int i = 0; i < cJSON_GetArraySize(studentsJson); i++) {
        cJSON *tempJson = cJSON_GetArrayItem(studentsJson, i);
        if (!tempJson) continue;

        // 根据相应的元数据字段获取对应的数据载体结构对象
        cJSON *sno = cJSON_GetObjectItem(tempJson, SNO_MS);
        cJSON *name = cJSON_GetObjectItem(tempJson, NAME_MS);
        cJSON *age = cJSON_GetObjectItem(tempJson, AGE_MS);
        cJSON *year = cJSON_GetObjectItem(tempJson, YEAR_MS);
        cJSON *address = cJSON_GetObjectItem(tempJson, ADDRESS_MS);
        cJSON *phone = cJSON_GetObjectItem(tempJson, PHONE_MS);
        cJSON *email = cJSON_GetObjectItem(tempJson, EMAIL_MS);

        // 将获取到的数据载体结构对象中的数据赋值给相应的字段
        StudentPtr_MS studentTemp = newEmptyStudent_MS();
        AssignIntValue_MS(&studentTemp->no, sno);
        AssignStrValue_MS(studentTemp->name, name);
        AssignIntValue_MS(&studentTemp->age, age);
        AssignStrValue_MS(studentTemp->year, year);
        AssignStrValue_MS(studentTemp->address, address);
        AssignStrValue_MS(studentTemp->phone, phone);
        AssignStrValue_MS(studentTemp->email, email);

        // 将数据结构对象放入到内存当中
        add_AL(arrayList, studentTemp);
    }

    cJSON_Delete(root);


    return arrayList;
}

void AssignIntValue_MS(Integer *field, const cJSON *json) {
    if (json) *field = json->valueint;
}

void AssignStrValue_MS(String field, const cJSON *json) {
    if (json != NULL)
        memcpy(field, json->valuestring, strlen(json->valuestring));
}

void InputStudentInfo_MS(ArrayListPtr students, FILE *input) {
    AssertDBOpened_MS(students);
    StudentPtr_MS student = newEmptyStudent_MS();

    // 从相应的输入流中录入学生信息
    fscanf(input, "%d", &student->no);
    fscanf(input, "%s", student->name);
    fscanf(input, "%d", &student->age);
    fscanf(input, "%s", student->year);
    fscanf(input, "%s", student->address);
    fscanf(input, "%s", student->phone);
    fscanf(input, "%s", student->email);

    while (fgetc(input) != '\n') continue;

    if (FindBySno_MS(students, student->no)) {
        printf("录入信息失败，学号为%d的学生已经存在！\n", student->no);
        return;
    }
    add_AL(students, student);
}

void AssertDBOpened_MS(ArrayListPtr students) {
    if (!students)
        fprintf(stderr, "数据库未打开，无法录入信息：%d\n", DB_NOT_OPEN);
}


void ShowInfo_MS(ArrayListPtr students, FILE *output) {
    AssertDBOpened_MS(students);

    // 打印表头
    ShowTableHeader_MS(output);

    for (int i = 1; i <= size_AL(students); i++)
        ShowTableBodyInfo_MS(students, i, output);

    fprintf(output, "\n\n");
}

void ShowTableBodyInfo_MS(ArrayListPtr students, int index, FILE *output) {
    StudentPtr_MS studentTemp = get_AL(students, index);
    if (!studentTemp) {
        fprintf(output, "第%d条数据出现了异常\n", index);
        return;
    }

    fprintf(output, "%-8d%-20s%-10d%-15s%-10s%-20s%-28s\n",
            studentTemp->no, studentTemp->name, studentTemp->age, studentTemp->year,
            studentTemp->address, studentTemp->phone, studentTemp->email);
}

void ShowTableHeader_MS(FILE *output) {
    fprintf(output, "%-11s%-22s%-13s%-22s%-12s%-23s%-10s\n",
            "学号", "姓名", "年龄", "出生年月日", "地址", "电话", "邮箱");
}


Page_MS ShowInfoPage_MS(ArrayListPtr students, Page_MS page, FILE *output) {
    AssertDBOpened_MS(students);
    ShowTableHeader_MS(output);

    // 计算输出列表开始输出的索引位置
    int startPos = (page.currPage - 1) * page.limit;

    // 计算出当前页要输出的学生信息数量
    int studentsSize = size_AL(students);
    int stdSize = studentsSize - startPos;
    int pageLimit = page.limit;
    if (stdSize > pageLimit) stdSize = pageLimit;

    // 输出当前页的信息
    int count = 0;
    for (int i = startPos + 1; count < stdSize; i++, count++)
        ShowTableBodyInfo_MS(students, i, output);

    // 计算所有的页数
    int totalPage = studentsSize / pageLimit;
    if (studentsSize % pageLimit > 0) totalPage++;

    fprintf(output, "%38s %d / %d, 当前页%d条信息\n\n", "", page.currPage, totalPage, count);

    // 返回一个更新的page对象
    return (Page_MS) {
            (page.currPage), totalPage, page.limit,
            count, page.currPage == 1, page.currPage == totalPage
    };
}

StudentPtr_MS FindBySno_MS(ArrayListPtr students, Integer sno) {
    AssertDBOpened_MS(students);

    for (int i = 1; i <= size_AL(students); i++) {
        StudentPtr_MS studentTemp = get_AL(students, i);
        if (!studentTemp) continue;

        if (studentTemp->no == sno)
            return studentTemp;
    }

    return NULL;
}

ArrayListPtr FindByName_MS(ArrayListPtr students, String name) {
    AssertDBOpened_MS(students);
    ArrayListPtr arrayList = newArrayList();

    regex_t regex;
    regmatch_t regMatch[1];
    const char *pattern = name;
    for (int i = 1; i <= size_AL(students); i++) {
        StudentPtr_MS studentTemp = get_AL(students, i);
        if (!studentTemp) continue;

        regcomp(&regex, pattern, REG_ICASE);
        if (regexec(&regex, studentTemp->name, 1, regMatch, 0) == 0)
            add_AL(arrayList, studentTemp);
    }
    return arrayList;
}

ArrayListPtr SortBySno_MS(ArrayListPtr students, Order_MS order) {
    AssertDBOpened_MS(students);
    ArrayListPtr sortedList = newArrayList();
    union_AL(sortedList, students);

    for (int i = 1; i <= size_AL(sortedList); ++i) {
        for (int j = i; j <= size_AL(sortedList); ++j) {
            StudentPtr_MS curr = get_AL(sortedList, i);
            StudentPtr_MS next = get_AL(sortedList, j);

            bool compareRs = curr->no > next->no;
            if (order == ASC) compareRs = curr->no < next->no;

            if (compareRs) continue;

            replace_AL(sortedList, i, next);
            replace_AL(sortedList, j, curr);
        }
    }
    return sortedList;
}

ArrayListPtr SortByName_MS(ArrayListPtr students, Order_MS order) {
    AssertDBOpened_MS(students);
    ArrayListPtr sortedList = newArrayList();
    union_AL(sortedList, students);

    for (int i = 1; i <= size_AL(sortedList); i++) {
        for (int j = i + 1; j <= size_AL(sortedList); j++) {
            StudentPtr_MS curr = get_AL(sortedList, i);
            StudentPtr_MS next = get_AL(sortedList, j);

            // 比较两个字符串的大小
            int compareNameRs = strcmp(curr->name, next->name);
            bool compareRs = compareNameRs > 0;
            if (order == ASC) compareRs = compareNameRs < 0;

            if (compareRs) continue;
            replace_AL(sortedList, i, next);
            replace_AL(sortedList, j, curr);
        }
    }
    return sortedList;
}

int IndexOf_MS(ArrayListPtr students, Integer sno) {
    for (int i = 1; i <= size_AL(students); ++i) {
        StudentPtr_MS studentTemp = get_AL(students, i);
        if (!studentTemp || studentTemp->no != sno) continue;
        return i;
    }
    return -1;
}

bool UpdateStudentBySno_MS(ArrayListPtr students, Integer sno, StudentPtr_MS student) {
    AssertDBOpened_MS(students);
    if (!student)return false;

    student->no = sno;
    int index = IndexOf_MS(students, sno);
    if (index != -1) {
        replace_AL(students, index, student);
        return true;
    }

    printf("指定学号为%d的学生不存在!\n", sno);
    return false;
}

bool DeleteStudentBySno_MS(ArrayListPtr students, Integer sno, StudentPtr_MS deleteData) {
    AssertDBOpened_MS(students);

    int index = IndexOf_MS(students, sno);

    if (index != -1) {
        remove_AL(students, index, (DataType_AL *) &deleteData);
        return true;
    }

    printf("指定学号为%d的学生不存在!\n", sno);
    return false;
}

bool SyncInfo_MS(ArrayListPtr students, String url) {
    if (!students) return false;

    FILE *db = fopen(url, "w+");
    if (!db) {
        fprintf(stderr, "数据库文件链接失败！\n");
        return false;
    }

    // 创建Json对象的根对象
    cJSON *root = cJSON_CreateObject();
    cJSON *rows = cJSON_CreateArray();
    cJSON_AddItemToObject(root, STUDENTS_MS, rows);

    cJSON *row;
    for (int i = 1; i <= students->size; i++) {
        row = cJSON_CreateObject();
        cJSON_AddItemToArray(rows, row);

        // 将结构数据写入到Json对象中
        StudentPtr_MS studentTemp = get_AL(students, i);
        cJSON_AddItemToObject(row, SNO_MS, cJSON_CreateNumber(studentTemp->no));
        cJSON_AddItemToObject(row, NAME_MS, cJSON_CreateString(studentTemp->name));
        cJSON_AddItemToObject(row, AGE_MS, cJSON_CreateNumber(studentTemp->age));
        cJSON_AddItemToObject(row, YEAR_MS, cJSON_CreateString(studentTemp->year));
        cJSON_AddItemToObject(row, ADDRESS_MS, cJSON_CreateString(studentTemp->address));
        cJSON_AddItemToObject(row, PHONE_MS, cJSON_CreateString(studentTemp->phone));
        cJSON_AddItemToObject(row, EMAIL_MS, cJSON_CreateString(studentTemp->email));
    }

    // 向文件之中写入数据文件
    String jsonResult = cJSON_Print(root);
    // 这里在写入文件之前应该对文件进行压缩




    fprintf(db, "%s", jsonResult);

    if (fclose(db) != 0) {
        fprintf(stderr, "数据库链接未成功关闭，有可能会对您的数据造成威胁\n");
    }

    return true;
}

void ShowMenu_MS(FILE *output) {
    fprintf(output, "===========================================================\n");
    fprintf(output, "|                  欢迎使用学生信息系统                      |\n");
    fprintf(output, "|              a）链接数据库                                |\n");
    fprintf(output, "|              b）录入学生信息                              |\n");
    fprintf(output, "|              c）浏览全部学生信息                           |\n");
    fprintf(output, "|              d）分页浏览学生信息                           |\n");
    fprintf(output, "|              e）根据学号查询学生信息                       |\n");
    fprintf(output, "|              f）根据姓名查询学生信息                       |\n");
    fprintf(output, "|              g）根据学号对学生信息进行排序                  |\n");
    fprintf(output, "|              h）根据姓名对学生信息进行排序                  |\n");
    fprintf(output, "|              i）根据学号更新学生信息                       |\n");
    fprintf(output, "|              j）根据学号删除学生信息                       |\n");
    fprintf(output, "|              k）同步数据库数据信息                         |\n");
    fprintf(output, "|                                                         |\n");
    fprintf(output, "| 序号使用相应功能，'q'退出系统                               |\n");
    fprintf(output, "===========================================================\n");
}