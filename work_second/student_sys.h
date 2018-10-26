//
// Created by determination on 2018/10/22.
//

#ifndef C_WORK_STUDENT_SYS_H
#define C_WORK_STUDENT_SYS_H
/**
 * 学生信息管理系统
 */

#include "student_ws.h"
#include "array_list.h"
#include "cJSON.h"
#include <string.h>
#include <regex.h>

/**
 * 封装分页显示的结构体
 */
typedef struct Page_MS {
    int currPage;       // 当前页数
    int totalPages;     // 总共多少页
    int limit;          // 每页的信息的数量
    int pageSize;       // 当前页数据的数量
    bool first;         // 是否是第一页
    bool last;          // 是否是最后一页
} Page_MS;


/**
 * 用于学生信息排序方式的枚举
 */
typedef enum Order_MS {
    DESC,       // 降序
    ASC         // 升序
} Order_MS;

/**
 * 打开学生信息数据库
 *
 * @param url 数据库的url，这里使用文件路径：包括相对路径和绝对路径
 * @return  将数据库中的全体学生的数据信息加载到内存当中的集合
 */
ArrayListPtr OpenStudentDB_MS(String url);

/**
 * 录入学生信息
 *
 * @param students 全体学生集合
 * @param input 数据输入源
 */
void InputStudentInfo_MS(ArrayListPtr students, FILE *input);

/**
 * 学生信息浏览
 *
 * @param students 全体学生集合
 * @param output 输出流
 */
void ShowInfo_MS(ArrayListPtr students, FILE *output);

/**
 * 分页进行浏览学生信息
 *
 * @param students 所有的学生的集合
 * @param page 分页参数
 * @param output 输出流
 */
Page_MS ShowInfoPage_MS(ArrayListPtr students, Page_MS page, FILE *output);

/**
 * 根据学号进行查询学生信息
 *
 * @param students 所有学生的集合
 * @param sno 学生学号
 * @return 返回成功查找的学生结构体，如果不存在则返回NULL
 */
StudentPtr_MS FindBySno_MS(ArrayListPtr students, Integer sno);

/**
 * 根据学生姓名进行查询学生信息
 *
 * @param students 所有学生的集合
 * @param name 学生姓名
 * @return 返回成功查找的学生结构体，如果不存在则返回NULL
 */
ArrayListPtr FindByName_MS(ArrayListPtr students, String name);

/**
 * 根据学生编号进行排序
 *
 * @param students 所有学生的集合
 * @param order 排序方式，默认值为升序排序
 * @return 排序完成的新的学生集合
 */
ArrayListPtr SortBySno_MS(ArrayListPtr students, Order_MS order);

/**
 * 根据学生姓名进行排序
 *
 * @param students 所有学生的集合
 * @param order 排序方式，默认值为升序排序
 * @return 排序完成的新的学生集合
 */
ArrayListPtr SortByName_MS(ArrayListPtr students, Order_MS order);

/**
 * 根据学号更新学生信息
 *
 * @param students 全体学生集合
 * @param sno 学生学号
 * @param student 将要更新的学生信息，注：学生学号不可以更新
 * @return 更新成功返回true，否则返回false
 */
bool UpdateStudentBySno_MS(ArrayListPtr students, Integer sno, StudentPtr_MS student);

/**
 * 根据学生学号删除学生信息
 *
 * @param students 全体学生集合
 * @param sno 学生学号
 * @return 成功删除返回true，否则返回false
 */
bool DeleteStudentBySno_MS(ArrayListPtr students, Integer sno, StudentPtr_MS deleteData);

/**
 * 向数据库同步信息，将内存之中的数据写入到数据库
 *
 * @param students 全体学生集合
 */
bool SyncInfo_MS(ArrayListPtr students, String url);

/**
 * 系统化功能菜单
 *
 * @param output 输出流
 */
void ShowMenu_MS(FILE *output);

#endif //C_WORK_STUDENT_SYS_H
