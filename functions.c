#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef free
#include <stdlib.h>
#endif
#ifndef strcpy
#include <string.h>
#endif
#ifndef get_size
#include "menu.h"
#endif
#ifndef HANDLE
#include <windows.h>
#endif
#ifndef printf
#include <stdio.h>
#endif
#ifndef uint
#define uint unsigned int
#endif

// Отступы слева и сверху для таблички
#define lp 3
#define tp 2

#define getch_thread_timeout 10

COORD screen_pos = {0, 0},      // Нулевые координаты
      screen_end_pos = {0, 0},  // Координаты конца экрана (определяется при запуске или перерисовке)
      border_pos = {lp, tp},    // Координаты начала рамки
      menu_pos = {4, 3},        // Координаты блока меню
      menu_size = {0, 0},       // Размеры блока меню (определяется при запуске или перерисовке)
      main_pos = {44, 3},       // Координаты главного блока (блока с данными) (определяется при запуске или перерисовке)
      main_size = {0, 0},       // Размеры главного блока (определяется при запуске или перерисовке)
      hint_pos = {4, 0},        // Координаты блока подсказок (определяется при запуске или перерисовке)
      hint_size = {0, 0};       // Размеры блока подсказок (определяется при запуске или перерисовке)

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
    student inf;
    struct list_item* next;
} list_item;

/// @brief Голова списка (размер списка и указатель на начальный элемент)
typedef struct list_header {
    list_item* first;
    uint length;
} list_header;

void SCP(COORD _pos) {SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _pos);}

/// @brief Функция для создания списка
/// @return Указатель на голову нового списка (или NULL в случае ошибки)
list_header* new_list() {
    return (list_header*)calloc(1, sizeof(list_header));
}

/// @brief Функция для создания структуры информационного поля
/// @param _group Шифр группы (6 символов + '\0')
/// @param _surname Фамилия студента (15 символов + '\0')
/// @param _birth_year Год рождения студента
/// @param _man Пол студента (1 - М, 0 - Ж)
/// @param _skipped_hours Количество пропущенных часов
/// @param _acquired_hours Количество оправданных часов
/// @return Структуру для последуюшего добавления в список
student new_student(char _group[7], char _surname[16], short _birth_year, char _man, uint _skipped_hours, uint _acquired_hours) {
    student res = { 0 };
    strcpy(res.group, _group);
    strcpy(res.surname, _surname);
    res.birth_year = _birth_year;
    res.man = _man;
    res.skipped_hours = _skipped_hours;
    res.acquired_hours = _acquired_hours;
    return res;
}

/// @brief Функция для добавления нового элемента в список
/// @param _lh Указатель на голову списка
/// @param _st Структура (созданная через new_student())
/// @return Указаель на голову списка (не отличается от передаваемого)
list_header* add_student(list_header* _lh, student _st) {
    if (_lh->first) {
        list_item* tmp = _lh->first;
        while (tmp->next) tmp = tmp->next;
        tmp->next = (list_item*)calloc(1, sizeof(list_item));
        if (!(tmp->next)) return _lh;
        tmp->next->inf = _st;
        tmp->next->next = NULL;
        _lh->length++;
    } else {
        _lh->first = (list_item*)calloc(1, sizeof(list_item));
        if (!(_lh->first)) return _lh;
        _lh->first->inf = _st;
        _lh->first->next = NULL;
    }
    return _lh;
}

/// @brief Получение информационного поля по его порядковому номеру (аналлогия с массивами)
/// @param _lh Указатель на голову списка
/// @param _id ID требуемого элемента
/// @return Информационное поле в случае успеха, нулевую структуру в случае ошибки
student get_student_by_id(list_header* _lh, uint _id) {
    student res = { 0 };
    if (!_lh) return res;
    list_item* tmp = _lh->first;
    if (!tmp) return res;
    for (uint _i = 0; _i <= _id && tmp->next; _i++, tmp = tmp->next) {
        if (_i == _id) return tmp->inf;
    }
    return res;
}

/// @brief Функция для создания (или перерисовки) элементов интерфейса
void create_layout() {
    system("cls");
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(866);
    CONSOLE_CURSOR_INFO inf;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    inf.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    int* sz = get_size();
    if (!sz && sz[0]) {
        printf("CRITICAL ERROR: Can't get window size\n");
        exit(1);
    }
    if (sz[1] < 80 || sz[2] < 30) {
        printf("ERROR: You need to open console in fullscreen, because your console size is too small\n");
        getchar();
    }
    // printf("Width = %d, height = %d\n", sz[1], sz[2]);
    screen_end_pos.X = sz[1];
    screen_end_pos.Y = sz[2];
    COORD border_lt = {lp, tp};
    uint window_w = sz[1] - lp*2;
    uint window_h = sz[2] - tp*2;
    main_size.X = window_w - 42;
    main_size.Y = window_h - 5;
    hint_size.Y = 2;
    hint_size.X = window_w - 2;
    menu_pos.X = border_lt.X + 1;
    menu_pos.Y = border_lt.Y + 1;
    hint_pos.X = menu_pos.X;
    hint_pos.Y = menu_pos.Y + window_h - 5;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_lt);
    printf("\xC9");
    for (uint _i = 0; _i < 39; _i++) printf("\xCD");
    printf("\xCB");
    for (uint _i = 0; _i < window_w - 42; _i++) printf("\xCD");
    printf("\xBB");
    SetConsoleOutputCP(old_cp);
    COORD border_l = border_lt;
    for (uint _i = 0; _i < window_h - 5; _i++) {
        border_l.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_l);
        printf("\xBA");
        for (uint _i = 0; _i < 39; _i++) printf("\x20");
        printf("\xBA");
        for (uint _i = 0; _i < window_w - 42; _i++) printf("\x20");
        printf("\xBA");
    }
    border_l.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_l);
    printf("\xCC");
    for (uint _i = 0; _i < 39; _i++) printf("\xCD");
    printf("\xCA");
    for (uint _i = 0; _i < window_w - 42; _i++) printf("\xCD");
    printf("\xB9");
    for (uint _i = 0; _i < 2; _i++) {
        border_l.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_l);
        printf("\xBA");
        for (uint _i = 0; _i < 39; _i++) printf("\x20");
        printf("\x20");
        for (uint _i = 0; _i < window_w - 42; _i++) printf("\x20");
        printf("\xBA");
    }
    hint_pos.Y = border_l.Y - 1;
    border_l.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_l);
    printf("\xC8");
    for (uint _i = 0; _i < 39; _i++) printf("\xCD");
    printf("\xCD");
    for (uint _i = 0; _i < window_w - 42; _i++) printf("\xCD");
    printf("\xBC");
    SetConsoleOutputCP(old_cp);

    // Сделать фон
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_pos);
    for (uint _i = 0; _i < sz[1] * 2; _i++) printf("\x1b[3%cm%c", '1'+(rand()%7), '0'+(rand()%2));
    screen_pos.Y += tp;
    for (uint _i = 0; _i < window_h; _i++) {
        screen_pos.X=0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_pos);
        for (uint _j = 0; _j < lp; _j++) printf("\x1b[3%cm%c", '1'+(rand()%7), '0'+(rand()%2));
        screen_pos.X = lp+window_w;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_pos);
        for (uint _j = 0; _j < lp; _j++) printf("\x1b[3%cm%c", '1'+(rand()%7), '0'+(rand()%2));
        screen_pos.Y++;
    }
    
    for (uint _i = 0; _i < sz[1] * 2; _i++) printf("\x1b[3%cm%c", '1'+(rand()%7), '0'+(rand()%2));
    printf("\x1b[0m");
    SetConsoleOutputCP(old_cp);
    free(sz);
}

/// @brief Функция для печати подсказки в заданной области
void show_base_hint(char* _hint) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), hint_pos);
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(1251);
    printf("%s", _hint);
    SetConsoleOutputCP(old_cp);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_end_pos);
}

/// @brief Функция для печати подсказки в заданной области (тестовая функция)
void show_second_hint(char* _hint) {
    hint_pos.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), hint_pos);
    hint_pos.Y--;
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(1251);
    printf("%s", _hint);
    SetConsoleOutputCP(old_cp);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_end_pos);
}

/// @brief Функция для очистки главной области (области с данными)
void clear_main() {
    COORD local_pos = menu_pos;
    int* sz = get_size();
    for (uint _i = 0; _i < sz[2] - tp*2 - 5; _i++) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
        for (uint _j = 0; _j < sz[1] - lp*2 - 42; _j++) printf("\x20");
        local_pos.Y++;
    }
    free(sz);
}

/// @brief Функция для вычисления количества символов в числе
/// @param _i Число, количество символов в котором надо узнать
/// @return Количество символов в переданном числе
int intlen(long long int _i) {
    char* r = (char*)calloc(21, sizeof(char));
    itoa(_i, r, 10);
    return strlen(r);
}

/// @brief Функция для ввода или изменения данных
/// @param _data Указатель на изменяемое информационное поле или NULL для создания нового 
/// @return Указатель на информационное поле с полученными от пользователя данными
student* enter_data(student* _d) {
    student* _data = (student*)calloc(1, sizeof(student));
    *_data = *_d;
    COORD local_pos = main_pos;
    local_pos.X+=2;
    local_pos.Y++;
    int* sz = get_size();
    uint local_h = sz[2] - tp*2 - 7,
         local_w = sz[1] - lp*2 - 46;
    if (local_h < 12) {return _data;}
    int s = 0;
    SCP(local_pos);
    COORD item_pos[7] = {
        {local_pos.X + 2, local_pos.Y + ((local_h>17)?2:1)},
        {local_pos.X + 2, local_pos.Y + ((local_h>17)?4:2)},
        {local_pos.X + 2, local_pos.Y + ((local_h>17)?6:3)},
        {local_pos.X + 2, local_pos.Y + ((local_h>17)?8:4)},
        {local_pos.X + 2, local_pos.Y + ((local_h>17)?10:5)},
        {local_pos.X + 2, local_pos.Y + ((local_h>17)?12:6)},
        {local_pos.X, local_pos.Y + ((local_h>17)?14:7)}
    };
    // int old_cp = GetConsoleOutputCP();
    int old_cp = 1251;
    SetConsoleCP(old_cp);
    SetConsoleOutputCP(866);
    printf("\xDA");
    for (uint _i = 0; _i < local_w - 2; _i++) printf("\xC4");
    printf("\xBF");

    if (local_h > 17) {
        // Пустая строка
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }
    
    // Группа
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 0) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    printf("%20.20s\x1b[0m\x20%s", "Группа", _data->group);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - strlen(_data->group); _i++) printf("_");
    printf("\x20\xB3");
    
    if (local_h > 17) {
        // Пустая строка
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }
    
    // Фамилия
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 1) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    printf("%20.20s\x1b[0m\x20%s", "Фамилия", _data->surname);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
    printf("\x20\xB3");
    
    if (local_h > 17) {
        // Пустая строка
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }
    
    // Год рождения
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 2) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    printf("%20.20s\x1b[0m\x20%d", "Год рождения", _data->birth_year);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
    printf("\x20\xB3");
    
    if (local_h > 17) {
        // Пустая строка
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }
    
    // Пол
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 3) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    printf("%20.20s\x1b[0m\x20", "Пол");
    printf((_data->man)?"Мужской":"Женский");
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
    printf("\x20\xB3");
    
    if (local_h > 17) {
        // Пустая строка
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }
    
    // Пропущенные часы
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 4) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    printf("%20.20s\x1b[0m\x20%d", "Пропущено часов", _data->skipped_hours);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
    printf("\x20\xB3");
    
    if (local_h > 17) {
        // Пустая строка
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }
    
    // Оправданные часы
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 5) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    printf("%20.20s\x1b[0m\x20%d", "Оправдано часов", _data->acquired_hours);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
    printf("\x20\xB3");
    
    if (local_h > 17) {
        // Пустая строка
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }

    // Кнопки Сохранить и Отменить
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3");
    uint btn_w = local_w - 18;
    for (uint _i = 0; _i < btn_w/2; _i++) printf("\x20");
    SetConsoleOutputCP(old_cp);
    if (s == 6) printf("\x1b[47;30m");
    printf("   ОК   \x1b[0m  ");
    if (s == 7) printf("\x1b[47;30m");
    printf("Отмена\x1b[0m");
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < btn_w - btn_w/2; _i++) printf("\x20");
    printf("\xB3");
    
    if (local_h > 17) {
        // Пустая строка
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }
    
    // Завершение (нижняя граница)
    local_pos.Y++;
    SCP(local_pos);
    printf("\xC0");
    for (uint _i = 0; _i < local_w - 2; _i++) printf("\xC4");
    printf("\xD9");

    while (1) {
        if (s == 0) { // 0 1
            // Редактирование группы
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
            } else if ((a >= 32 && a <= 126) || (a >= 192 && a <= 255)) {
                // Char key
                if (strlen(_data->group) < 6) _data->group[strlen(_data->group)] = a;
            } else if (a == 8) {
                // Backspace key
                _data->group[strlen(_data->group)-1] = '\0';
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[0]);
            if (s == 0) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "Группа", _data->group);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->group); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[1]);
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "Фамилия", _data->surname);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
        } else if (s == 1) { // 0 1
            // Редактирование фамилии
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
            } else if ((a >= 32 && a <= 126) || (a >= 192 && a <= 255)) {
                // Char key
                if (strlen(_data->surname) < 15) _data->surname[strlen(_data->surname)] = a;
            } else if (a == 8) {
                // Backspace key
                _data->surname[strlen(_data->surname)-1] = '\0';
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[0]);
            if (s == 0) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "Группа", _data->group);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->group); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[1]);
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "Фамилия", _data->surname);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[2]);
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Год рождения", _data->birth_year);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
        } else if (s == 2) { // 2 4 5
            // Редактирование года рождения
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
            } else if (a >= 48 && a <= 57) {
                // Number key
                _data->birth_year *= 10;
                _data->birth_year += a - 48;
            } else if (a == 8) {
                // Backspace key
                _data->birth_year -= (_data->birth_year) % 10;
                _data->birth_year /= 10;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[1]);
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "Фамилия", _data->surname);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[2]);
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Год рождения", _data->birth_year);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20", "Пол");
            printf((_data->man)?"Мужской":"Женский");
            SetConsoleOutputCP(866);
        } else if (s == 3) { // 3
            // Редактирование гендера
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
                else if (b == 75 || b == 77) _data->man = !(_data->man);
            } else if (a >= 48 && a <= 57) {
                // Number key
                _data->birth_year *= 10;
                _data->birth_year += a - 48;
            } else if (a == 13) _data->man = !(_data->man);
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[2]);
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Год рождения", _data->birth_year);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20", "Пол");
            printf((_data->man)?"Мужской":"Женский");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Пропущено часов", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
        } else if (s == 4) { // 2 4 5
            // Редактирование количества пропущенных часов
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
            } else if (a >= 48 && a <= 57) {
                // Number key
                _data->skipped_hours *= 10;
                _data->skipped_hours += a - 48;
            } else if (a == 8) {
                // Backspace key
                _data->skipped_hours -= (_data->skipped_hours) % 10;
                _data->skipped_hours /= 10;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20", "Пол");
            printf((_data->man)?"Мужской":"Женский");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Пропущено часов", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Оправдано часов", _data->acquired_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
        } else if (s == 5) { // 2 4 5
            // Редактирование количества пропущенных часов
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
            } else if (a >= 48 && a <= 57) {
                // Number key
                _data->acquired_hours *= 10;
                _data->acquired_hours += a - 48;
            } else if (a == 8) {
                // Backspace key
                _data->acquired_hours -= (_data->acquired_hours) % 10;
                _data->acquired_hours /= 10;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Пропущено часов", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Оправдано часов", _data->acquired_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[6]);
            printf("\xB3");
            uint btn_w = local_w - 18;
            for (uint _i = 0; _i < btn_w/2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ОК   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("Отмена\x1b[0m");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < btn_w - btn_w/2; _i++) printf("\x20");
            printf("\xB3");
        } else if (s == 6) { // OK
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 77) s = 7;  // Right arrow
                else if (b == 72) s = 5;  // Up arrow
            } else if (a == 13) {
                // Enter
                return _data;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Оправдано часов", _data->acquired_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[6]);
            printf("\xB3");
            uint btn_w = local_w - 18;
            for (uint _i = 0; _i < btn_w/2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ОК   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("Отмена\x1b[0m");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < btn_w - btn_w/2; _i++) printf("\x20");
            printf("\xB3");
        } else if (s == 7) { // Cancel
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 75) s = 6;  // Left arrow
                else if (b == 72) s = 5;  // Up arrow
            } else if (a == 13) {
                // Enter
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Оправдано часов", _data->acquired_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[6]);
            printf("\xB3");
            uint btn_w = local_w - 18;
            for (uint _i = 0; _i < btn_w/2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ОК   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("Отмена\x1b[0m");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < btn_w - btn_w/2; _i++) printf("\x20");
            printf("\xB3");
        }
        local_pos.Y = main_pos.Y + 1;
    }
    
    local_pos.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
    printf("\xC0");
    for (uint _i = 0; _i < local_w - 2; _i++) printf("\xC4");
    printf("\xD9");
    SetConsoleOutputCP(old_cp);
    return _data;
}

/// @brief Функция для очистки области с меню
void clear_menu() {
    COORD local_pos = menu_pos;
    for (uint _i = 0; _i < menu_size.Y; _i++, local_pos.Y++) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
        for (uint _j = 0; _j < menu_size.X; _j++) printf("\x20");
    }
}

/// @brief Функция для очистки области с подсказками
void clear_hint() {
    //
}

/// @brief Функция для обновления области с подсказками
void update_hint() {
    //
}

/// @brief Функция для обновления области с меню
void update_menu() {
    //
}

/// @brief Функция, выполняющая действие
/// @return 1 для продолжения работы, 0 для завершения программы (-1, если функцияонал не реализован)
int make_action() {
    return -1;
}












































