#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef free
#include <stdlib.h>
#endif

/// @brief Информационное поле для элементов списка
typedef struct student {
    char       group[7];  // Шифр группы (6 символов)
    char    surname[16];  // Фамилия студента (15 символов)
    short    birth_year;  // Год рождения студента
    char            man;  // Пол студента (1 - М/П, 0 - Ж/Д)
    uint  skipped_hours;  // Число пропущенный часов
    uint acquired_hours;  // Число оправданных часов
} student;

/// @brief Элемент списка (информационное поле и указатель на следующий элемент)
typedef struct list_item {
    student* inf;
    struct list_item* next;
} list_item;

/// @brief Голова списка (размер списка и указатель на начальный элемент)
typedef struct list_header {
    list_item* first;
    uint length;
} list_header;

list_header* new_list();
student new_student(char _group[7], char _surname[16], short _birth_year, char _man, uint _skipped_hours, uint _acquired_hours);
list_header* add_student(list_header*, student);
student* get_student_by_id(list_header*, uint);
int* get_size();
void create_layout();
void show_base_hint(char*);
void clear_main();
size_t intlen(int);
student* enter_data(student*);
void clear_menu();
void clear_hint();
void update_hint();
void update_menu();
int make_action();
void show_table(list_header*);
void chcp(int);
void remove_student_by_id(list_header*, uint);






