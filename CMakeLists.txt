cmake_minimum_required(VERSION 3.12)
project(c_work C)

include_directories(work_first)
set(CMAKE_C_STANDARD 11)

set(WORK_FIRST_DIR  work_first/staff.h      work_first/staff.c
                    work_first/classroom.h  work_first/classroom.c)

add_executable(c_work main.c ${WORK_FIRST_DIR})