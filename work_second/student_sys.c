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
 * 根据学生名称进行比较
 *
 * @param currName 当前的学生姓名
 * @param nextName 下一个学生的姓名
 * @return currName > nextName  返回 1
 *         currName == nextName 返回 0
 *         currName < nextName  返回-1
 */
int CompareByName_MS(String currName, String nextName);

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

void RunListPageModel(ArrayListPtr students);

void RunFindBySnoModel(const ArrayList *students);

void RunFindByNameModel(ArrayListPtr students);

void RunSortByNameModel(ArrayListPtr students);

void RunSortBySnoModel(ArrayListPtr students);

void RunInputStudentInfoModel(ArrayListPtr students);

ArrayListPtr RunOpenDBModel();

void IgnoreInputN();

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
    fprintf(output, "%10d\t%10s\t%10d\t%10s\t%10s\t%10s\t%10s\n",
            studentTemp->no, studentTemp->name, studentTemp->age, studentTemp->year,
            studentTemp->address, studentTemp->phone, studentTemp->email);
}

void ShowTableHeader_MS(FILE *output) {
    fprintf(output, "%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n",
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
    for (int i = startPos; count < stdSize; i++, count++)
        ShowTableBodyInfo_MS(students, i, output);

    // 计算所有的页数
    int totalPage = studentsSize / pageLimit;
    if (studentsSize % pageLimit > 0) totalPage++;

    fprintf(output, "%60s %d / %d, 当前页%d条信息\n\n", "", page.currPage, totalPage, count);

    return (Page_MS) {
            (page.currPage), totalPage, page.limit,
            count, page.currPage == 1, page.currPage == totalPage
    };
}

StudentPtr_MS FindBySno_MS(ArrayListPtr students, Integer sno) {
    AssertDBOpened_MS(students);

    for (int i = 0; i < size_AL(students); i++) {
        StudentPtr_MS studentTemp = get_AL(students, i);
        if (!studentTemp) continue;


        if (studentTemp->no == sno)
            return studentTemp;
    }

    return NULL;
}

int CompareByName_MS(String currName, String nextName) {
    size_t first = strlen(currName);
    size_t second = strlen(nextName);
    if (first != second) return false;

    return strncmp(currName, nextName, first);
}

StudentPtr_MS FindByName_MS(ArrayListPtr students, String name) {
    AssertDBOpened_MS(students);

    for (int i = 0; i < size_AL(students); i++) {
        StudentPtr_MS studentTemp = get_AL(students, i);
        if (!studentTemp) continue;


        if (CompareByName_MS(studentTemp->name, name) == 0)
            return studentTemp;
    }
    return NULL;
}

void SortBySno_MS(ArrayListPtr students, Order_MS order) {
    for (int i = 1; i <= size_AL(students); ++i) {
        for (int j = i; j <= size_AL(students); ++j) {
            StudentPtr_MS curr = get_AL(students, i);
            StudentPtr_MS next = get_AL(students, j);

            bool compareRs = curr->no > next->no;
            if (order == ASC) compareRs = curr->no < next->no;

            if (compareRs) continue;

            replace_AL(students, i, next);
            replace_AL(students, j, curr);
        }
    }
}

void SortByName_MS(ArrayListPtr students, Order_MS order) {
    for (int i = 1; i <= size_AL(students); ++i) {
        for (int j = i; j <= size_AL(students); ++j) {
            StudentPtr_MS curr = get_AL(students, i);
            StudentPtr_MS next = get_AL(students, j);

            int compareNameRs = CompareByName_MS(curr->name, next->name);
            bool compareRs = compareNameRs == 1;
            if (order == ASC) compareRs = compareNameRs == -1;

            if (compareRs) continue;
            replace_AL(students, i, next);
            replace_AL(students, j, curr);
        }
    }
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

static char *const url = "./students.json";

void StudentInfoSystemDriver_MS(void) {
    ShowMenu_MS(stdout);
    fprintf(stdout, "\n\n请输入您需要的操作\n\n");
    ArrayListPtr students = NULL;
    int item = 0;
    while (true) {
        fprintf(stdout, "admin@student-ms:$ ");
        item = getchar();
        IgnoreInputN();

        if (item == 'q') {
            SyncInfo_MS(students, url);
            break;
        }

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
                break;
            case 'j':
                break;
            case 'k':
                break;
            default:
                fprintf(stdout, "输入不合法\n");
        }
    }
}

void IgnoreInputN() { while (getchar() != '\n') continue; }

ArrayListPtr RunOpenDBModel() {
    ArrayListPtr students = OpenStudentDB_MS(url);
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
}

void RunSortBySnoModel(ArrayListPtr students) {

}

void RunSortByNameModel(ArrayListPtr students) {

}


void RunFindByNameModel(ArrayListPtr students) {

}

void RunFindBySnoModel(const ArrayList *students) {
    int item;
    int sno;
    do {
        fprintf(stdout, "请输入学号：\n");

    } while ((item = fscanf(stdin, "%d", &sno)));
    FindBySno_MS(students, 1);
}

void RunListPageModel(ArrayListPtr students) {
    // 选项初始值用于显示第一页数据
    int item = '0';

    // 初始化page对象
    Page_MS page = {
            1, 0, 10, 0, true, false
    };
    do {
        if (item == 'n') {
            if (page.last) printf("当前页是最后一页!\n");
            else
                page = ShowInfoPage_MS(students, page, stdout);
        } else if (item == 'p') {
            if (page.first) printf("当前页为第一页");
            else
                page = ShowInfoPage_MS(students, page, stdout);
        } else if (item == '0') {
            page.currPage = 1;
            page = ShowInfoPage_MS(students, page, stdout);
        }
        fprintf(stdout, "输入n获取下一页，输入p获取上一页，其他字符退出\n");
    } while ((item = getchar()) == 'n' || item == 'p');
}