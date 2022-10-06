#ifndef uint
#define uint unsigned int
#endif

typedef struct student_info {
    char group[7];      // Шифр группы
    char surname[16];   // Фамилия студента
    short birth_year;   // Год рождения студента
    char male;          // (as byte) 1 - male, 0 - female
    int skipped_hours;  // Количество пропущенных часов
    int aquired_hours;  // Количество оправданных часов
} student_info;

typedef struct student_item {
    student_info data;
    struct student_item* next;
} student_item;

typedef struct list_header {
    student_item* first;
    uint len;
} list_header;