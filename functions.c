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

// ������� ����� � ������ ��� ��������
#define lp 3
#define tp 2

COORD screen_pos = {0, 0},      // ������� ����������
      screen_end_pos = {0, 0},  // ���������� ����� ������
      border_pos = {lp, tp},    // ���������� ������ �����
      menu_pos = {4, 3},        // ���������� ����� ����
      main_pos = {44, 3},       // ���������� �������� ����� (����� � �������)
      hint_pos = {4, 0};        // ���������� ����� ��������� (������������ ��� ������� ��� �����������)

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
    student inf;
    struct list_item* next;
} list_item;

/// @brief ������ ������ (������ ������ � ��������� �� ��������� �������)
typedef struct list_header {
    list_item* first;
    uint length;
} list_header;

/// @brief ������� ��� �������� ������
/// @return ��������� �� ������ ������ ������ (��� NULL � ������ ������)
list_header* new_list() {
    return (list_header*)calloc(1, sizeof(list_header));
}

/// @brief ������� ��� �������� ��������� ��������������� ����
/// @param _group ���� ������ (6 �������� + '\0')
/// @param _surname ������� �������� (15 �������� + '\0')
/// @param _birth_year ��� �������� ��������
/// @param _man ��� �������� (1 - �, 0 - �)
/// @param _skipped_hours ���������� ����������� �����
/// @param _acquired_hours ���������� ����������� �����
/// @return ��������� ��� ������������ ���������� � ������
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

/// @brief ������� ��� ���������� ������ �������� � ������
/// @param _lh ��������� �� ������ ������
/// @param _st ��������� (��������� ����� new_student())
/// @return �������� �� ������ ������ (�� ���������� �� �������������)
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

/// @brief ��������� ��������������� ���� �� ��� ����������� ������ (��������� � ���������)
/// @param _lh ��������� �� ������ ������
/// @param _id ID ���������� ��������
/// @return �������������� ���� � ������ ������, ������� ��������� � ������ ������
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

/// @brief ������� ��� �������� (��� �����������) ��������� ����������
void create_layout() {
    system("cls");
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(866);
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

    // ������� ���
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

/// @brief ������� ��� ������ ��������� � �������� ������� (�������� �������)
void show_base_hint() {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), hint_pos);
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(1251);
    printf("��� ������ ������� ���������. ������� ��� �������� ������������ ������������ �� ������.");
    SetConsoleOutputCP(old_cp);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_end_pos);
}

/// @brief ������� ��� ������� ������� ������� (������� � �������)
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

/// @brief ������� ��� ���������� ���������� �������� � �����
/// @param _i �����, ���������� �������� � ������� ���� ������
/// @return ���������� �������� � ���������� �����
int intlen(long long int _i) {
    char* r = (char*)calloc(21, sizeof(char));
    itoa(_i, r, 10);
    return strlen(r);
}

/// @brief ������� ��� ����� ��� ��������� ������
/// @param _data ��������� �� ���������� �������������� ���� ��� NULL ��� �������� ������ 
/// @return ��������� �� �������������� ���� � ����������� �� ������������ �������
student* enter_data(student* _data) {
    if (!_data) _data = (student*)calloc(1, sizeof(student));
    COORD local_pos = main_pos;
    local_pos.X+=2;
    local_pos.Y++;
    int* sz = get_size();
    uint local_h = sz[2] - tp*2 - 7,
         local_w = sz[1] - lp*2 - 46;
    if (local_h < 12) {return _data;}
    int s = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
    // int old_cp = GetConsoleOutputCP();
    int old_cp = 1251;
    SetConsoleOutputCP(866);
    printf("\xDA");
    for (uint _i = 0; _i < local_w - 2; _i++) printf("\xC4");
    printf("\xBF");
    if (local_h >= 17) {
        // Big layout

        while (1) {
            // ������ ������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3");
            for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
            printf("\xB3");
            
            // ������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3\x20");
            if (s == 0) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "������", _data->group);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->group); _i++) printf("_");
            printf("\x20\xB3");
            
            // ������ ������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3");
            for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
            printf("\xB3");
            
            // �������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3\x20");
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "�������", _data->surname);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
            printf("\x20\xB3");
            
            // ������ ������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3");
            for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
            printf("\xB3");
            
            // ��� ��������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3\x20");
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��� ��������", _data->birth_year);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
            printf("\x20\xB3");
            
            // ������ ������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3");
            for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
            printf("\xB3");
            
            // ���
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3\x20");
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20", "���");
            printf((_data->man)?"�������":"�������");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
            printf("\x20\xB3");
            
            // ������ ������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3");
            for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
            printf("\xB3");
            
            // ����������� ����
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3\x20");
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
            printf("\x20\xB3");
            
            // ������ ������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3");
            for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
            printf("\xB3");
            
            // ����������� ����
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3\x20");
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
            printf("\x20\xB3");
            
            // ������ ������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3");
            for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
            printf("\xB3");

            // ������ ��������� � ��������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3");
            uint btn_w = local_w - 18;
            for (uint _i = 0; _i < btn_w/2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ��   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("������\x1b[0m");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < btn_w - btn_w/2; _i++) printf("\x20");
            printf("\xB3");
            
            // ������ ������
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xB3");
            for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
            printf("\xB3");
            
            // ���������� (������ �������)
            local_pos.Y++;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
            printf("\xC0");
            for (uint _i = 0; _i < local_w - 2; _i++) printf("\xC4");
            printf("\xD9");

            if (s == 2) { // 2 4 5
                // �������������� ���� ��������
                uchar a = _getch();
                COORD local2_pos = menu_pos;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local2_pos);
                printf("a = %d", a);
                if (a == 224) {
                    // Arrow key
                    uchar b = _getch();
                    if (b == 80) s++;  // Down arrow
                    else if (b == 72) s--; // Up arrow
                } else if (a >= 48 && a <= 57) {
                    // Number key
                    _data->birth_year *= 10;
                    _data->birth_year += a - 48;
                } else if (a == 8) {
                    // Backspace key
                    _data->birth_year -= (_data->birth_year) % 10;
                    _data->birth_year /= 10;
                }
            } else if (s == 4) { // 2 4 5
                // �������������� ���������� ����������� �����
                uchar a = _getch();
                COORD local2_pos = menu_pos;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local2_pos);
                printf("a = %d", a);
                if (a == 224) {
                    // Arrow key
                    uchar b = _getch();
                    if (b == 80) s++;  // Down arrow
                    else if (b == 72) s--; // Up arrow
                } else if (a >= 48 && a <= 57) {
                    // Number key
                    _data->skipped_hours *= 10;
                    _data->skipped_hours += a - 48;
                } else if (a == 8) {
                    // Backspace key
                    _data->skipped_hours -= (_data->skipped_hours) % 10;
                    _data->skipped_hours /= 10;
                }
            } else if (s == 5) { // 2 4 5
                // �������������� ���������� ����������� �����
                uchar a = _getch();
                COORD local2_pos = menu_pos;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local2_pos);
                printf("a = %d", a);
                if (a == 224) {
                    // Arrow key
                    uchar b = _getch();
                    if (b == 80) s++;  // Down arrow
                    else if (b == 72) s--; // Up arrow
                } else if (a >= 48 && a <= 57) {
                    // Number key
                    _data->acquired_hours *= 10;
                    _data->acquired_hours += a - 48;
                } else if (a == 8) {
                    // Backspace key
                    _data->acquired_hours -= (_data->acquired_hours) % 10;
                    _data->acquired_hours /= 10;
                }
            } else {
                _getch();
            }
            local_pos.Y = main_pos.Y + 1;
        }

    } else {
        // Small layout
    }
    local_pos.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
    printf("\xC0");
    for (uint _i = 0; _i < local_w - 2; _i++) printf("\xC4");
    printf("\xD9");
    SetConsoleOutputCP(old_cp);
    return _data;
}

/// @brief ������� ��� ������� ������� � ����
void clear_menu() {
    //
}

/// @brief ������� ��� ������� ������� � �����������
void clear_hint() {
    //
}

/// @brief ������� ��� ���������� ������� � �����������
void update_hint() {
    //
}

/// @brief ������� ��� ���������� ������� � ����
void update_menu() {
    //
}

/// @brief �������, ����������� ��������
/// @return 1 ��� ����������� ������, 0 ��� ���������� ��������� (-1, ���� ����������� �� ����������)
int make_action() {
    return -1;
}












































