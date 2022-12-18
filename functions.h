#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef free
#include <stdlib.h>
#endif

#ifndef _getch
#include <conio.h>
#endif

/// @brief �������������� ���� ��� ��������� ������
typedef struct student {
    char       group[7];  // ���� ������ (6 ��������)
    char    surname[16];  // ������� �������� (15 ��������)
    short    birth_year;  // ��� �������� ��������
    char            man;  // ��� �������� (1 - �/�, 0 - �/�)
    uint  skipped_hours;  // ����� ����������� �����
    uint acquired_hours;  // ����� ����������� �����
} student;

/// @brief ������� ������ (�������������� ���� � ��������� �� ��������� �������)
typedef struct list_item {
    student* inf;            // ��������� �� �������������� ����
    struct list_item* next;  // ��������� �� ��������� �������
} list_item;

/// @brief ������ ������ (������ ������ � ��������� �� ��������� �������)
typedef struct list_header {
    list_item* first;  // ��������� �� ��������� ������� ������
    int length;        // ����� ������
} list_header;

list_header* new_list();
void del_list(list_header*);
student new_student(char _group[7], char _surname[16], short _birth_year, char _man, uint _skipped_hours, uint _acquired_hours);
list_header* add_student(list_header*, student);
list_item* get_student_by_id(list_header*, uint);
void create_layout();
student* enter_data(student*);
void show_table_with_title(list_header*, char*);
void show_table(list_header*);
void chcp(int);
void remove_student_by_id(list_header*, uint);
void menu(char, list_header*);
void sort_list(list_header*, char, char);
void read_list_from_file(list_header*);
void save_list_to_file(list_header*);
void export_list_to_file(list_header*);
void show_base_hint(char*);
void show_second_hint(char*);
char* strlower(char*);
size_t intlen(int);
void clear_second_hint();
void SCP(COORD);
void chcp(int);
