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

ArrayListPtr OpenStudentDB_MS(String url) {
    FILE *db = fopen(url, "w+");

    if (!db) {
        printf("数据库打开失败！");
        exit(OPEN_DB_FIELD);
    }


    String json = "";
    String temp = "";
    while (fgets(temp, READ_STR_LEN, db))
        strncat(json, temp, strlen(temp));

    if (fclose(db) != 0)
        printf("数据库文件挂起失败！可能会使您的数据遭到破坏");

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
    Integer sno, age;
    String name = NULL, year = NULL,
            address = NULL, phone = NULL, email = NULL;

    // 从相应的输入流中录入学生信息
    fscanf(input, "%d", &sno);
    fscanf(input, "%s", name);
    fscanf(input, "%d", &age);
    fscanf(input, "%s", year);
    fscanf(input, "%s", address);
    fscanf(input, "%s", phone);
    fscanf(input, "%s", email);

    while (fgetc(input) != '\n') continue;
    StudentPtr_MS student = newStudent_MS(sno, name, age, year, address, phone, email);
    add_AL(students, student);
}

void AssertDBOpened_MS(ArrayListPtr students) {
    if (!students)
        fprintf(stderr, "数据库未打开，无法录入信息：%d", DB_NOT_OPEN);
}


void ShowInfo_MS(ArrayListPtr students, FILE *output) {
    AssertDBOpened_MS(students);

    // 打印表头
    ShowTableHeader_MS(output);

    for (int i = 0; i < size_AL(students); i++)
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
    fprintf(output, "%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s",
            "学号", "姓名", "年龄", "出生年月日", "地址", "电话", "邮箱");
}


void ShowInfoPage_MS(ArrayListPtr students, Page_MS page, FILE *output) {
    AssertDBOpened_MS(students);
    ShowTableHeader_MS(output);

    // 计算输出列表开始输出的索引位置
    int startPos = page.current * page.pageSize + 1;


    // 计算出当前页要输出的学生信息数量
    int studentsSize = size_AL(students);
    int stdSize = studentsSize - startPos;
    int pageSize = page.pageSize;
    if (stdSize > pageSize) stdSize = pageSize;

    int count = 0;
    for (int i = startPos; i < stdSize; i++, count++)
        ShowTableBodyInfo_MS(students, i, output);

    fprintf(output, "%60s %d / %d, 当前页%d条信息", "", pageSize, studentsSize / pageSize, count);
    fprintf(output, "\n\n");
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

    printf("指定学号为%d的学生不存在!", sno);
    return false;
}

bool DeleteStudentBySno_MS(ArrayListPtr students, Integer sno, StudentPtr_MS deleteData) {
    AssertDBOpened_MS(students);

    int index = IndexOf_MS(students, sno);

    if (index != -1) {
        remove_AL(students, index, (DataType_AL *) &deleteData);
        return true;
    }

    printf("指定学号为%d的学生不存在!", sno);
    return false;
}

bool SyncInfo_MS(ArrayListPtr students, String url) {
    AssertDBOpened_MS(students);
    FILE *db = fopen(url, "w+");
    if (!db) {
        fprintf(stderr, "数据库文件链接失败！");
        return false;
    }

    // 创建Json对象的根对象
    cJSON *root = cJSON_CreateObject();
    cJSON *rows = cJSON_CreateArray();
    cJSON_AddItemToObject(root, STUDENTS_MS, rows);

    cJSON *row;
    for (int i = 0; i < students->size; ++i) {
        row = cJSON_CreateObject();
        cJSON_AddItemToArray(rows, row);

        // 将结构数据写入到Json对象中
        StudentPtr_MS studentTemp = get_AL(students, i);
        cJSON_AddItemToObject(row, SNO_MS,     cJSON_CreateNumber(studentTemp->no));
        cJSON_AddItemToObject(row, NAME_MS,    cJSON_CreateString(studentTemp->name));
        cJSON_AddItemToObject(row, AGE_MS,     cJSON_CreateNumber(studentTemp->no));
        cJSON_AddItemToObject(row, YEAR_MS,    cJSON_CreateString(studentTemp->year));
        cJSON_AddItemToObject(row, ADDRESS_MS, cJSON_CreateString(studentTemp->address));
        cJSON_AddItemToObject(row, PHONE_MS,   cJSON_CreateString(studentTemp->phone));
        cJSON_AddItemToObject(row, EMAIL_MS,   cJSON_CreateString(studentTemp->email));
    }

    // 向文件之中写入数据文件
    String jsonResult = cJSON_Print(root);
    fprintf(db, "%s", jsonResult);

    if (fclose(db) != 0) {
        fprintf(stderr, "数据库链接未成功关闭，有可能会对您的数据造成威胁");
    }

    return true;
}

void ShowMenu_MS(FILE *output) {
    fprintf(output, "===========================================================\n");
    fprintf(output, "|                  欢迎使用学生信息系统                      |\n");
    fprintf(output, "|              1）链接数据库                                |\n");
    fprintf(output, "|              2）录入学生信息                              |\n");
    fprintf(output, "|              3）浏览全部学生信息                           |\n");
    fprintf(output, "|              4）分页浏览学生信息                           |\n");
    fprintf(output, "|              5）根据学号查询学生信息                       |\n");
    fprintf(output, "|              6）根据姓名查询学生信息                       |\n");
    fprintf(output, "|              7）根据学号对学生信息进行排序                  |\n");
    fprintf(output, "|              8）根据姓名对学生信息进行排序                  |\n");
    fprintf(output, "|              9）根据学号更新学生信息                       |\n");
    fprintf(output, "|             10）根据学号删除学生信息                       |\n");
    fprintf(output, "|             11）同步数据库数据信息                         |\n");
    fprintf(output, "===========================================================\n");
}

void StudentInfoSystemDriver_MS(void) {
    ShowMenu_MS(stdout);
}