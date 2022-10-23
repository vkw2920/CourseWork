#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef free
#include <stdlib.h>
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
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef strlen
#include <string.h>
#endif

#include "functions.h"

// Отступы слева и сверху для таблички
#define lp 3
#define tp 2

#define getch_thread_timeout 10

COORD screen_pos = { 0, 0 },    // Нулевые координаты
    screen_end_pos = { 0, 0 },  // Координаты конца экрана (определяется при запуске или перерисовке)
    border_pos = { lp, tp },    // Координаты начала рамки
    menu_pos = { 4, 3 },        // Координаты блока меню
    menu_size = { 0, 0 },       // Размеры блока меню (определяется при запуске или перерисовке)
    main_pos = { 44, 3 },       // Координаты главного блока (блока с данными) (определяется при запуске или перерисовке)
    main_size = { 0, 0 },       // Размеры главного блока (определяется при запуске или перерисовке)
    hint_pos = { 4, 0 },        // Координаты блока подсказок (определяется при запуске или перерисовке)
    hint_size = { 0, 0 };       // Размеры блока подсказок (определяется при запуске или перерисовке)

/// @brief Алиас для упрощения перемещения курсора в консоли
/// @param _pos позиция курсора в структуре COORD
void SCP(COORD _pos) { SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _pos); }

/// @brief Алиас для изменения кодировки вывода
/// @param _cp Номер кодировки
void chcp(int _cp) { SetConsoleOutputCP(_cp); }

/// @brief Функция для получения размеров консоли
/// @return Массив из трёх чисел {код_выхода, число_столбцов, число_строк}
int* get_size() {
    int* res = (int*)calloc(3, sizeof(int));
    if (!res) return NULL;
    HANDLE hWndConsole;
    if (hWndConsole = GetStdHandle(-12)) {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo)) {
            res[1] = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
            res[2] = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
        }
        else res[0] = GetLastError();
    }
    else res[0] = GetLastError();
    return res;
}

/// @brief Функция для создания списка
/// @return Указатель на голову нового списка (или NULL в случае ошибки)
list_header* new_list() {
    list_header* res = (list_header*)calloc(1, sizeof(list_header));
    return res;
}

/// @brief Удаление всего списка без предупреждения!
/// @param _lh Указатель на голову удаляемого списка
void del_list(list_header* _lh) {
    while (get_student_by_id(_lh, 0)) remove_student_by_id(_lh, 0);
    free(_lh);
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
        tmp->next->inf = (student*)calloc(1, sizeof(student));
        *(tmp->next->inf) = _st;
        tmp->next->next = NULL;
        _lh->length++;
    }
    else {
        _lh->first = (list_item*)calloc(1, sizeof(list_item));
        if (!(_lh->first)) return _lh;
        _lh->first->inf = (student*)calloc(1, sizeof(student));
        *(_lh->first->inf) = _st;
        _lh->first->next = NULL;
    }
    return _lh;
}

/// @brief Получение информационного поля по его порядковому номеру (аналлогия с массивами)
/// @param _lh Указатель на голову списка
/// @param _id ID требуемого элемента
/// @return Информационное поле в случае успеха, нулевую структуру в случае ошибки
student* get_student_by_id(list_header* _lh, uint _id) {
    if (!_lh) return NULL;
    list_item* tmp = _lh->first;
    if (!tmp) return NULL;
    for (uint _i = 0; _i <= _id && tmp; _i++, tmp = tmp->next) {
        if (_i == _id) return tmp->inf;
    }
    return NULL;
}

/// @brief Удаление элемнета по его порядковому номеру
/// @param _lh Указатель на голову списка
/// @param _id ID удаляемого элемента
void remove_student_by_id(list_header* _lh, uint _id) {
    if (!_lh) return;
    list_item* tmp = _lh->first;
    if (!tmp) return;
    for (uint _i = 0; _i <= _id+1 && tmp->next; _i++, tmp = tmp->next) {
        if (_i == _id+1) {
            list_item* tmp2 = tmp->next;
            tmp->next = tmp->next->next;
            free(tmp2);
            _lh->length--;
        }
    }
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
    COORD border_lt = { lp, tp };
    uint window_w = sz[1] - lp * 2;
    uint window_h = sz[2] - tp * 2;
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
    for (uint _i = 0; _i < sz[1] * 2; _i++) printf("\x1b[3%cm%c", '1' + (rand() % 7), '0' + (rand() % 2));
    screen_pos.Y += tp;
    for (uint _i = 0; _i < window_h; _i++) {
        screen_pos.X = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_pos);
        for (uint _j = 0; _j < lp; _j++) printf("\x1b[3%cm%c", '1' + (rand() % 7), '0' + (rand() % 2));
        screen_pos.X = lp + window_w;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_pos);
        for (uint _j = 0; _j < lp; _j++) printf("\x1b[3%cm%c", '1' + (rand() % 7), '0' + (rand() % 2));
        screen_pos.Y++;
    }

    for (uint _i = 0; _i < sz[1] * 2; _i++) printf("\x1b[3%cm%c", '1' + (rand() % 7), '0' + (rand() % 2));
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

/// @brief Функция для печати подсказки в заданной области
/// @param _hint Си-строка, которую нужно вывести (обязательно завершение символом \0)
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
    COORD local_pos = main_pos;
    for (uint _i = 0; _i < main_size.Y; _i++, local_pos.Y++) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
        for (uint _j = 0; _j < main_size.X; _j++) printf("\x20");
    }
}

/// @brief Функция для вычисления количества символов в числе
/// @param _i Число, количество символов в котором надо узнать
/// @return Количество символов в переданном числе
size_t intlen(int _i) {
    char* r = (char*)calloc(21, sizeof(char));
    _itoa(_i, r, 10);
    return strlen(r);
}

/// @brief Функция для ввода или изменения данных
/// @param _data Указатель на изменяемое информационное поле или NULL для создания нового 
/// @return Указатель на информационное поле с полученными от пользователя данными
student* enter_data(student* _d) {
    if (!_d) _d = (student*)calloc(1, sizeof(student));
    student* _data = (student*)calloc(1, sizeof(student));
    *_data = *_d;
    COORD local_pos = main_pos;
    local_pos.X += 2;
    local_pos.Y++;
    int* sz = get_size();
    uint local_h = sz[2] - tp * 2 - 7,
        local_w = sz[1] - lp * 2 - 46;
    if (local_h < 12) { return _data; }
    int s = 0;
    SCP(local_pos);
    COORD item_pos[7] = {
        {local_pos.X + 2, local_pos.Y + ((local_h > 17) ? 2 : 1)},
        {local_pos.X + 2, local_pos.Y + ((local_h > 17) ? 4 : 2)},
        {local_pos.X + 2, local_pos.Y + ((local_h > 17) ? 6 : 3)},
        {local_pos.X + 2, local_pos.Y + ((local_h > 17) ? 8 : 4)},
        {local_pos.X + 2, local_pos.Y + ((local_h > 17) ? 10 : 5)},
        {local_pos.X + 2, local_pos.Y + ((local_h > 17) ? 12 : 6)},
        {local_pos.X, local_pos.Y + ((local_h > 17) ? 14 : 7)}
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
    printf((_data->man) ? "Мужской" : "Женский");
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
    for (uint _i = 0; _i < btn_w / 2; _i++) printf("\x20");
    SetConsoleOutputCP(old_cp);
    if (s == 6) printf("\x1b[47;30m");
    printf("   ОК   \x1b[0m  ");
    if (s == 7) printf("\x1b[47;30m");
    printf("Отмена\x1b[0m");
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < btn_w - btn_w / 2; _i++) printf("\x20");
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
                show_second_hint("Повторите нажатие \"а\" для ввода этой буквы");
                uchar b = _getch();
                show_second_hint("                                          ");
                if (b == 80) s++;  // Down arrow
                else if (b == 224) if (strlen(_data->group) < 6) _data->group[strlen(_data->group)] = 'а';
            }
            else if ((a >= 32 && a <= 126) || (a >= 192 && a <= 255)) {
                // Char key
                if (strlen(_data->group) < 6) _data->group[strlen(_data->group)] = a;
            }
            else if (a == 8) {
                // Backspace key
                _data->group[strlen(_data->group) - 1] = '\0';
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
        }
        else if (s == 1) { // 0 1
            // Редактирование фамилии
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                show_second_hint("Повторите нажатие \"а\" для ввода этой буквы");
                uchar b = _getch();
                show_second_hint("                                          ");
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
                else if (b == 224 && strlen(_data->surname) < 15) _data->surname[strlen(_data->surname)] = 'а';
            }
            else if ((a >= 32 && a <= 126) || (a >= 192 && a <= 255)) {
                // Char key
                if (strlen(_data->surname) < 15) _data->surname[strlen(_data->surname)] = a;
            }
            else if (a == 8) {
                // Backspace key
                _data->surname[strlen(_data->surname) - 1] = '\0';
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
        }
        else if (s == 2) { // 2 4 5
            // Редактирование года рождения
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
            }
            else if (a >= 48 && a <= 57) {
                // Number key
                _data->birth_year *= 10;
                _data->birth_year += a - 48;
            }
            else if (a == 8) {
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
            printf((_data->man) ? "Мужской" : "Женский");
            SetConsoleOutputCP(866);
        }
        else if (s == 3) { // 3
            // Редактирование гендера
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
                else if (b == 75 || b == 77) _data->man = !(_data->man);
            }
            else if (a == 13) _data->man = !(_data->man);
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
            printf((_data->man) ? "Мужской" : "Женский");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "Пропущено часов", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
        }
        else if (s == 4) { // 2 4 5
            // Редактирование количества пропущенных часов
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
            }
            else if (a >= 48 && a <= 57) {
                // Number key
                _data->skipped_hours *= 10;
                _data->skipped_hours += a - 48;
            }
            else if (a == 8) {
                // Backspace key
                _data->skipped_hours -= (_data->skipped_hours) % 10;
                _data->skipped_hours /= 10;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20", "Пол");
            printf((_data->man) ? "Мужской" : "Женский");
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
        }
        else if (s == 5) { // 2 4 5
            // Редактирование количества пропущенных часов
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
            }
            else if (a >= 48 && a <= 57) {
                // Number key
                _data->acquired_hours *= 10;
                _data->acquired_hours += a - 48;
            }
            else if (a == 8) {
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
            for (uint _i = 0; _i < btn_w / 2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ОК   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("Отмена\x1b[0m");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < btn_w - btn_w / 2; _i++) printf("\x20");
            printf("\xB3");
        }
        else if (s == 6) { // OK
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 77) s = 7;  // Right arrow
                else if (b == 72) s = 5;  // Up arrow
            }
            else if (a == 13) {
                // Enter
                clear_main();
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
            for (uint _i = 0; _i < btn_w / 2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ОК   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("Отмена\x1b[0m");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < btn_w - btn_w / 2; _i++) printf("\x20");
            printf("\xB3");
        }
        else if (s == 7) { // Cancel
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 75) s = 6;  // Left arrow
                else if (b == 72) s = 5;  // Up arrow
            }
            else if (a == 13) {
                // Enter
                clear_main();
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
            for (uint _i = 0; _i < btn_w / 2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ОК   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("Отмена\x1b[0m");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < btn_w - btn_w / 2; _i++) printf("\x20");
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
    clear_main();
    return _data;
}

/// @brief Функция для вывода таблицы студентов
/// @param _lh Указатель на голову списка студентов
void show_table(list_header* _lh) {
    uint page_size = main_size.Y - 6;
    // В случае нехватки пространства сообщим об этом пользователю и выйдем
    if (main_size.X < 76 || main_size.Y < 9) {
        SCP(main_pos);
        for (uint _i = 0; _i < (main_pos.X - 45) / 2; _i++) printf("\x20");
        printf("Ошибка отображения. Недостаточно пространства");
        _getch();
        clear_main();
        return;
    }
    // Если не вышли, продолжим работу
    int s = 0;
    uint cur_page = 0;
    draw_table:  // Специально для GOTO
    COORD local_pos = main_pos;
    local_pos.Y++, local_pos.X += 2;
#pragma region Шапка таблицы
    SCP(local_pos);
    printf("\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC2");  // Horizontal line
    // 46 (R) + 10 (L) + 4 (Padding)
    for (uint _i = 0; _i < main_size.X - 60; _i++) printf("\xC4");
    printf("\xC2\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF");  // TopRight
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3 ");
    chcp(1251); printf("Группа ");
    chcp(866); printf("\xB3 ");
    for (uint _i = 0; _i < (main_size.X - 69) / 2; _i++) printf("\x20");
    chcp(1251); printf("Фамилия");
    chcp(866);
    for (uint _i = 0; _i < (main_size.X - 69) - (main_size.X - 69) / 2; _i++) printf("\x20");
    printf(" \xB3 ");
    chcp(1251); printf("Год рождения ");
    chcp(866); printf("\xB3 ");
    chcp(1251); printf("Пол ");
    chcp(866); printf("\xB3 ");
    chcp(1251); printf("Пропущено ");
    chcp(866); printf("\xB3 ");
    chcp(1251); printf("Оправдано ");
    chcp(866); printf("\xB3");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xC3\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC5");
    for (uint _i = 0; _i < main_size.X - 60; _i++) printf("\xC4");
    printf("\xC5\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xB4");
#pragma endregion

    local_pos.Y++;
    for (int _i = cur_page * page_size; _i <= _lh->length && _i <= ((page_size)*(cur_page+1) - 2); _i++, local_pos.Y++) {
        student* tmp = get_student_by_id(_lh, _i);
        SCP(local_pos);
        printf((s == _i)?"\xB3\x1b[47;30m ":"\xB3 ");
        chcp(1251); printf("%6s", tmp->group);
        chcp(866); printf("\x20\xB3\x20");
        chcp(1251); printf("%s", tmp->surname);
        uint spaces_count = main_size.X - 62 - strlen(tmp->surname);
        for (uint _i = 0; _i < spaces_count; _i++)
            printf(" ");
        chcp(866); printf("\x20\xB3\x20");
        for (uint _i = 0; _i < (12 - intlen(tmp->birth_year)) / 2; _i++) printf(" ");
        chcp(1251); printf("%4d", tmp->birth_year);
        for (uint _i = 0; _i < (12 - intlen(tmp->birth_year)) - (12 - intlen(tmp->birth_year)) / 2; _i++) printf(" ");
        chcp(866); printf("\x20\xB3\x20\x20");
        chcp(1251); printf((tmp->man) ? "М" : "Ж");
        chcp(866); printf("\x20\x20\xB3\x20");
        printf("%9d\x20\xB3\x20%-9d\x20\x1b[0m\xB3", tmp->skipped_hours, tmp->acquired_hours);
        // _getch();
    }
    SCP(local_pos);
    printf("\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC1");
    for (uint _i = 0; _i < main_size.X - 60; _i++) printf("\xC4");
    printf("\xC1\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9");
    local_pos.Y++;
    SCP(local_pos);
    for (uint _i = 0; _i < main_size.X / 2 - 6; _i++) printf("\x20");
    printf("Page: %d / %d", cur_page+1, (_lh->length % page_size) ? (_lh->length / page_size + 1) : (_lh->length / page_size));
    for (uint _i = 0; _i < main_size.X / 2 - 8; _i++) printf("\x20");
    while (local_pos.Y < main_pos.Y + main_size.Y - 1) {
        local_pos.Y++;
        SCP(local_pos);
        for (uint _i = 0; _i < main_size.X - 4; _i++) printf("\x20");
    }

    while (1) {
        uchar a = _getch();
        SCP(menu_pos);
        printf("Entered %d     ", a);
        if (a == 'e' || a == 243 || a == 13) {
            student* editable = get_student_by_id(_lh, s);
            *(editable) = *(enter_data(editable));
            goto draw_table;
        } else if (a == 'n' || a == 226) {
            add_student(_lh, *(enter_data(NULL)));
            goto draw_table;
        } else if (a == 224) {
            // Arrow key
            uchar b = _getch();
            COORD menu = menu_pos;
            menu.Y++;
            SCP(menu);
            printf("Entered %d     ", b);
            if (b == 72 && s > cur_page*page_size) {
                s--;
                uint prev = (s>cur_page*page_size)?s-1:s;
                uint post = (s<(cur_page+1)*page_size)?s+2:s;
                for (uint _i = prev; _i < post; _i++) {
                    local_pos.Y = main_pos.Y + 4 + (_i % page_size);
                    student* tmp = get_student_by_id(_lh, _i);
                    if (!tmp) continue;
                    SCP(local_pos);
                    printf((s == _i)?"\xB3\x1b[47;30m ":"\xB3 ");
                    chcp(1251); printf("%6s", tmp->group);
                    chcp(866); printf("\x20\xB3\x20");
                    chcp(1251); printf("%s", tmp->surname);
                    uint spaces_count = main_size.X - 62 - strlen(tmp->surname);
                    for (uint _i = 0; _i < spaces_count; _i++)
                        printf(" ");
                    chcp(866); printf("\x20\xB3\x20");
                    for (uint _i = 0; _i < (12 - intlen(tmp->birth_year)) / 2; _i++) printf(" ");
                    chcp(1251); printf("%4d", tmp->birth_year);
                    for (uint _i = 0; _i < (12 - intlen(tmp->birth_year)) - (12 - intlen(tmp->birth_year)) / 2; _i++) printf(" ");
                    chcp(866); printf("\x20\xB3\x20\x20");
                    chcp(1251); printf((tmp->man) ? "М" : "Ж");
                    chcp(866); printf("\x20\x20\xB3\x20");
                    printf("%9d\x20\xB3\x20%-9d\x20\x1b[0m\xB3", tmp->skipped_hours, tmp->acquired_hours);
                }
            } else if (b == 80 && s < _lh->length && s < (cur_page+1)*page_size - 2) {
                s++;
                uint prev = (s>cur_page*page_size)?s-1:s;
                uint post = (s<(cur_page+1)*page_size+1)?s+1:s;
                for (uint _i = prev; _i < post; _i++) {
                    local_pos.Y = main_pos.Y + 4 + (_i % page_size);
                    student* tmp = get_student_by_id(_lh, _i);
                    if (!tmp) continue;
                    SCP(local_pos);
                    printf((s == _i)?"\xB3\x1b[47;30m ":"\xB3 ");
                    chcp(1251); printf("%6s", tmp->group);
                    chcp(866); printf("\x20\xB3\x20");
                    chcp(1251); printf("%s", tmp->surname);
                    uint spaces_count = main_size.X - 62 - strlen(tmp->surname);
                    for (uint _i = 0; _i < spaces_count; _i++)
                        printf(" ");
                    chcp(866); printf("\x20\xB3\x20");
                    for (uint _i = 0; _i < (12 - intlen(tmp->birth_year)) / 2; _i++) printf(" ");
                    chcp(1251); printf("%4d", tmp->birth_year);
                    for (uint _i = 0; _i < (12 - intlen(tmp->birth_year)) - (12 - intlen(tmp->birth_year)) / 2; _i++) printf(" ");
                    chcp(866); printf("\x20\xB3\x20\x20");
                    chcp(1251); printf((tmp->man) ? "М" : "Ж");
                    chcp(866); printf("\x20\x20\xB3\x20");
                    printf("%9d\x20\xB3\x20%-9d\x20\x1b[0m\xB3", tmp->skipped_hours, tmp->acquired_hours);
                }
            } else if (b == 83) {
                remove_student_by_id(_lh, s-2);
                goto draw_table;
            } else if (b == 75 && cur_page > 0) {
                cur_page--;
                s -= page_size;
                goto draw_table;
            } else if (b == 77 && cur_page < _lh->length / page_size) {
                s += page_size;
                cur_page++;
                goto draw_table;
            }
        } else if (a == 27) {
            del_list(_lh);
            return;
        } else if (a == 18) {
            goto draw_table;
        }
    }
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
    COORD local_pos = hint_pos;
    for (uint _i = 0; _i < 2; _i++, local_pos.Y++) {
        SCP(local_pos);
        for (uint _j = 0; _j < hint_size.X; _j++) printf("\x20");
    }
}

/// @brief Функция, выполняющая действие
/// @return 1 для продолжения работы, 0 для завершения программы (-1, если функцияонал не реализован)
int make_action() {
    return -1;
}












































