cmake_minimum_required(VERSION 3.12)
project(c_work C)

set(CMAKE_C_STANDARD 11)
include_directories(work_first work_second lib)

set(LIB_DIR         lib/array_list.h            lib/array_list.c
                    lib/linked_list.h           lib/linked_list.c
                    lib/cJSON.h                 lib/cJSON.c )

set(WORK_FIRST_DIR  work_first/staff.h          work_first/staff.c
                    work_first/classroom.h      work_first/classroom.c)

set(WORK_SECOND_DIR work_second/student_sys.h   work_second/student_sys.c
                    work_second/student_ws.h    work_second/student_ws.c
                    work_second/sys_driver.c    work_second/sys_driver.h)

add_executable(c_work main.c ${WORK_FIRST_DIR} ${WORK_SECOND_DIR} ${LIB_DIR})