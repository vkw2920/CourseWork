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

// ������� ����� � ������ ��� ��������
#define lp 6
#define tp 3

// ������� ��� ���� ��������������
#define elp 40

#define getch_thread_timeout 10

COORD screen_pos = { 0, 0 };      // ������� ����������
COORD screen_end_pos = { 0, 0 };  // ���������� ����� ������ (������������ ��� ������� ��� �����������)
COORD border_pos = { lp, tp };    // ���������� ������ �����
COORD menu_pos = { 4, 3 };        // ���������� ����� ����
COORD menu_size = { 0, 0 };       // ������� ����� ���� (������������ ��� ������� ��� �����������)
COORD main_pos = { 31 + lp, 1 + tp };       // ���������� �������� ����� (����� � �������) (������������ ��� ������� ��� �����������)
COORD main_size = { 0, 0 };       // ������� �������� ����� (������������ ��� ������� ��� �����������)
COORD hint_pos = { 4, 0 };        // ���������� ����� ��������� (������������ ��� ������� ��� �����������)
COORD hint_size = { 0, 0 };       // ������� ����� ��������� (������������ ��� ������� ��� �����������)

int menu_selected = 0;
char _sort_direction = 1;

/// @brief ������� ��� �������������� ������ � ������ ������� (������ ��� ��������)
/// @param _str ��������� �� ������ ������ ������ (��� ������ ��-������)
/// @return ��� �� ���������, ����� ��������� ������
inline char* strlower(char* _str) {
    if (!_str) return _str;
    size_t len = strlen(_str);
    for (uint _i = 0; _i < len; _i++) {
        if (_str[_i] >= 65 && _str[_i] <= 90) {
            _str[_i] += 32;
        }
    }
    return _str;
}

/// @brief ����� ��� ��������� ����������� ������� � �������
/// @param _pos ������� ������� � ��������� COORD
inline void SCP(COORD _pos) { SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _pos); }

/// @brief ����� ��� ��������� ��������� ������
/// @param _cp ����� ���������
inline void chcp(int _cp) { SetConsoleOutputCP(_cp); }

/// @brief ������� ��� �������� ������
/// @return ��������� �� ������ ������ ������ (��� NULL � ������ ������)
list_header* new_list() {
    list_header* res = (list_header*)calloc(1, sizeof(list_header));
    return res;
}

/// @brief �������� ����� ������ ��� ��������������!
/// @param _lh ��������� �� ������ ���������� ������
inline void del_list(list_header* _lh) {
    while (get_student_by_id(_lh, 0)) remove_student_by_id(_lh, 0);
    free(_lh);
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

/// @brief ��������� ��������������� ���� �� ��� ����������� ������ (��������� � ���������)
/// @param _lh ��������� �� ������ ������
/// @param _id ID ���������� ��������
/// @return �������������� ���� � ������ ������, ������� ��������� � ������ ������
list_item* get_student_by_id(list_header* _lh, uint _id) {
    if (_id < 0 || !_lh || !(_lh->first))
        return NULL;

    list_item* tmp = _lh->first;

    for (uint _i = 0; _i <= _id && tmp; _i++, tmp = tmp->next)
        if (_i == _id)
            return tmp;

    return NULL;
}

/// @brief �������� �������� �� ��� ����������� ������
/// @param _lh ��������� �� ������ ������
/// @param _id ID ���������� ��������
inline void remove_student_by_id(list_header* _lh, uint _id) {
    if (!_lh) return;
    list_item* tmp = _lh->first;
    if (!tmp) return;
    if (_id == 0) {
        list_item* tmp2 = _lh->first;
        _lh->first = tmp2->next;
        free(tmp2);
    }
    else {
        for (uint _i = _id-1; _i > 0; _i--, tmp = tmp->next);
        list_item* tmp2 = tmp->next;
        tmp->next = tmp2->next;
        free(tmp2);
    }
    _lh->length--;
}

/// @brief ������� ��� �������� (��� �����������) ��������� ����������
inline void create_layout() {
    system("cls");
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(866);
    CONSOLE_CURSOR_INFO inf;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    inf.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    CONSOLE_SCREEN_BUFFER_INFO buff_info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff_info)) screen_end_pos = buff_info.dwSize;
    else exit(1);
    if (screen_end_pos.X < 80 || screen_end_pos.Y < 30) {
        printf("ERROR: You need to open console in fullscreen, because your console size is too small\n");
        getchar();
    }
    COORD border_lt = { lp, tp };
    uint window_w = screen_end_pos.X - lp * 2;
    int window_h = screen_end_pos.Y - tp * 2;
    if (window_h > 999) {
        exit(screen_end_pos.Y);
    }
    main_size.X = window_w - 32;
    main_size.Y = window_h - 5;
    hint_size.Y = 2;
    hint_size.X = window_w - 2;
    menu_pos.X = border_lt.X + 1;
    menu_pos.Y = border_lt.Y + 1;
    menu_size.X = 29;
    menu_size.Y = window_h - 5;
    hint_pos.X = menu_pos.X;
    hint_pos.Y = menu_pos.Y + menu_size.Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_lt);
    printf("\xC9");
    for (uint _i = 0; _i < menu_size.X; _i++) printf("\xCD");
    printf("\xCB");
    for (uint _i = 0; _i < main_size.X; _i++) printf("\xCD");
    printf("\xBB");
    SetConsoleOutputCP(old_cp);
    COORD border_l = border_lt;
    for (uint _i = 0; _i < menu_size.Y; _i++) {
        border_l.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_l);
        printf("\xBA");
        for (uint _j = 0; _j < menu_size.X; _j++) printf("\x20");
        printf("\xBA");
        for (uint _j = 0; _j < main_size.X; _j++) printf("\x20");
        printf("\xBA");
    }
    border_l.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_l);
    printf("\xCC");
    for (uint _i = 0; _i < menu_size.X; _i++) printf("\xCD");
    printf("\xCA");
    for (uint _i = 0; _i < main_size.X; _i++) printf("\xCD");
    printf("\xB9");
    for (uint _i = 0; _i < 2; _i++) {
        border_l.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_l);
        printf("\xBA");
        for (uint _j = 0; _j < menu_size.X; _j++) printf("\x20");
        printf("\x20");
        for (uint _j = 0; _j < main_size.X; _j++) printf("\x20");
        printf("\xBA");
    }
    hint_pos.Y = border_l.Y - 1;
    border_l.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), border_l);
    printf("\xC8");
    for (uint _i = 0; _i < menu_size.X; _i++) printf("\xCD");
    printf("\xCD");
    for (uint _i = 0; _i < main_size.X; _i++) printf("\xCD");
    printf("\xBC");
    SetConsoleOutputCP(old_cp);

    // ������� ���
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_pos);
    for (uint _i = 0; _i < screen_end_pos.X * tp; _i++) printf("\x1b[3%cm%c", '1' + (rand() % 7), '0' + (rand() % 2));
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
    screen_pos.X = screen_pos.Y = 0;

    for (uint _i = 0; _i < screen_end_pos.X * tp; _i++) printf("\x1b[3%cm%c", '1' + (rand() % 7), '0' + (rand() % 2));
    printf("\x1b[0m");
    SetConsoleOutputCP(old_cp);
    show_base_hint("\x1b[33m��������������\x1b[0m: ������������� ������������� �������� ����� �������� � ������� � ������ ���������.");
}

/// @brief ������� ��� ������ ��������� � �������� �������
inline void show_base_hint(char* _hint) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), hint_pos);
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(1251);
    printf("%s", _hint);
    SetConsoleOutputCP(old_cp);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_end_pos);
}

/// @brief ������� ��� ������ ��������� � �������� �������
/// @param _hint ��-������, ������� ����� ������� (����������� ���������� �������� \0)
inline void show_second_hint(char* _hint) {
    hint_pos.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), hint_pos);
    hint_pos.Y--;
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(1251);
    printf("%s", _hint);
    SetConsoleOutputCP(old_cp);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_end_pos);
}

/// @brief ������� ��� ������� ������� ������� (������� � �������)
inline void clear_main() {
    COORD local_pos = main_pos;
    for (uint _i = 0; _i < main_size.Y; _i++, local_pos.Y++) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
        for (uint _j = 0; _j < main_size.X; _j++) printf("\x20");
    }
}

/// @brief ������� ��� ���������� ���������� �������� � �����
/// @param _i �����, ���������� �������� � ������� ���� ������
/// @return ���������� �������� � ���������� �����
size_t intlen(int _i) {
    char* r = (char*)calloc(21, sizeof(char));
    _itoa(_i, r, 10);
    size_t res = strlen(r);
    free(r);
    return res;
}

/// @brief ������� ��� ������������ ������� intlen(int)
void intlen_test() {
    printf("Testing: intlen(2004) = %d\n", intlen(2004));
    printf("Testing: intlen(204) = %d\n", intlen(204));
    printf("Testing: intlen(24) = %d\n", intlen(24));
    printf("Testing: intlen(98) = %d\n", intlen(98));
    printf("Testing: intlen(20394) = %d\n", intlen(20394));
}

/// @brief ������� ��� ����� ��� ��������� ������
/// @param _data ��������� �� ���������� �������������� ���� ��� NULL ��� �������� ������ 
/// @return ��������� �� �������������� ���� � ����������� �� ������������ �������
student* enter_data(student* _d) {
    student* _data = (student*)calloc(1, sizeof(student));
    if (_d) *_data = *_d;
    COORD local_pos = main_pos;
    local_pos.X += (main_size.X - 40) / 2;
    local_pos.Y += (main_size.Y > 21) ? (main_size.Y / 2 - 8) : (main_size.Y / 2 - 4);
    uint local_h = main_size.Y - 4,
        local_w = 40;
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
    clear_second_hint();

    if (local_h > 17) {
        // ������ ������
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }

    // ������
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 0) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    printf("%15.15s\x1b[0m\x20%s", "������", _data->group);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 20 - strlen(_data->group); _i++) printf("_");
    printf("\x20\xB3");

    if (local_h > 17) {
        // ������ ������
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }

    // �������
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 1) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    printf("%15.15s\x1b[0m\x20%s", "�������", _data->surname);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 20 - strlen(_data->surname); _i++) printf("_");
    printf("\x20\xB3");

    if (local_h > 17) {
        // ������ ������
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }

    // ��� ��������
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 2) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    if (_data->birth_year) printf("%15.15s\x1b[0m\x20%d", "��� ��������", _data->birth_year);
    else printf("%15.15s\x1b[0m\x20_", "��� ��������");
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 20 - intlen(_data->birth_year); _i++) printf("_");
    printf("\x20\xB3");

    if (local_h > 17) {
        // ������ ������
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }

    // ���
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 3) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    printf("%15.15s\x1b[0m\x20", "���");
    printf((_data->man) ? "�������" : "�������");
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 27; _i++) printf("_");
    printf("\x20\xB3");

    if (local_h > 17) {
        // ������ ������
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }

    // ����������� ����
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 4) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    if (_data->skipped_hours) printf("%15.15s\x1b[0m\x20%d", "��������� �����", _data->skipped_hours);
    else printf("%15.15s\x1b[0m\x20_", "��������� �����");
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 20 - intlen(_data->skipped_hours); _i++) printf("_");
    printf("\x20\xB3");

    if (local_h > 17) {
        // ������ ������
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }

    // ����������� ����
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3\x20");
    if (s == 5) printf("\x1b[47;30m");
    SetConsoleOutputCP(old_cp);
    if (_data->acquired_hours) printf("%15.15s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
    else printf("%15.15s\x1b[0m\x20_", "��������� �����");
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 20 - intlen(_data->acquired_hours); _i++) printf("_");
    printf("\x20\xB3");

    if (local_h > 17) {
        // ������ ������
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }

    // ������ ��������� � ��������
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3");
    uint btn_w = local_w - 18;
    for (uint _i = 0; _i < btn_w / 2; _i++) printf("\x20");
    SetConsoleOutputCP(old_cp);
    if (s == 6) printf("\x1b[47;30m");
    printf("   ��   \x1b[0m  ");
    if (s == 7) printf("\x1b[47;30m");
    printf("������\x1b[0m");
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < btn_w - btn_w / 2; _i++) printf("\x20");
    printf("\xB3");

    if (local_h > 17) {
        // ������ ������
        local_pos.Y++;
        SCP(local_pos);
        printf("\xB3");
        for (uint _i = 0; _i < local_w - 2; _i++) printf("\x20");
        printf("\xB3");
    }

    // ���������� (������ �������)
    local_pos.Y++;
    SCP(local_pos);
    printf("\xC0");
    for (uint _i = 0; _i < local_w - 2; _i++) printf("\xC4");
    printf("\xD9");

    while (1) {
        if (s == 0) { // 0 1
            // �������������� ������
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                show_second_hint("��������� ������� \"�\" ��� ����� ���� �����");
                uchar b = _getch();
                clear_second_hint();
                if (b == 80) s++;  // Down arrow
                else if (b == 224) if (strlen(_data->group) < 6) _data->group[strlen(_data->group)] = '�';
            }
            else if ((a >= 32 && a <= 126) || (a >= 192 && a <= 255)) {
                // Char key
                if (strlen(_data->group) < 6) _data->group[strlen(_data->group)] = a;
            }
            else if (a == 8) {
                // Backspace key
                _data->group[strlen(_data->group) - 1] = '\0';
            }
            else if (a == 27) {
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[0]);
            if (s == 0) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%s", "������", _data->group);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->group); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[1]);
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%s", "�������", _data->surname);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
        }
        else if (s == 1) { // 0 1
            // �������������� �������
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                show_second_hint("��������� ������� \"�\" ��� ����� ���� �����");
                uchar b = _getch();
                show_second_hint("                                          ");
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
                else if (b == 224 && strlen(_data->surname) < 15) _data->surname[strlen(_data->surname)] = '�';
            }
            else if ((a >= 32 && a <= 126) || (a >= 192 && a <= 255)) {
                // Char key
                if (strlen(_data->surname) < 15) _data->surname[strlen(_data->surname)] = a;
            }
            else if (a == 8) {
                // Backspace key
                _data->surname[strlen(_data->surname) - 1] = '\0';
            }
            else if (a == 27) {
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[0]);
            if (s == 0) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%s", "������", _data->group);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->group); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[1]);
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%s", "�������", _data->surname);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[2]);
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��� ��������", _data->birth_year);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
        }
        else if (s == 2) { // 2 4 5
            // �������������� ���� ��������
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
            else if (a == 27) {
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[1]);
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%s", "�������", _data->surname);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[2]);
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��� ��������", _data->birth_year);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20", "���");
            printf((_data->man) ? "�������" : "�������");
            SetConsoleOutputCP(866);
        }
        else if (s == 3) { // 3
            // �������������� �������
            uchar a = _getch();
            if (a == 224) {
                // Arrow key
                uchar b = _getch();
                if (b == 80) s++;  // Down arrow
                else if (b == 72) s--;  // Up arrow
                else if (b == 75 || b == 77) _data->man = !(_data->man);
            }
            else if (a == 13) _data->man = !(_data->man);
            else if (a == 27) {
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[2]);
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��� ��������", _data->birth_year);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20", "���");
            printf((_data->man) ? "�������" : "�������");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��������� �����", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
        }
        else if (s == 4) { // 2 4 5
            // �������������� ���������� ����������� �����
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
            else if (a == 27) {
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20", "���");
            printf((_data->man) ? "�������" : "�������");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��������� �����", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
        }
        else if (s == 5) { // 2 4 5
            // �������������� ���������� ����������� �����
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
            else if (a == 27) {
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��������� �����", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[6]);
            printf("\xB3");
            uint btn_w = local_w - 18;
            for (uint _i = 0; _i < btn_w / 2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ��   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("������\x1b[0m");
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
            else if (a == 27) {
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[6]);
            printf("\xB3");
            uint btn_w = local_w - 18;
            for (uint _i = 0; _i < btn_w / 2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ��   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("������\x1b[0m");
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
                return _d;
            }
            else if (a == 27) {
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%15.15s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[6]);
            printf("\xB3");
            uint btn_w = local_w - 18;
            for (uint _i = 0; _i < btn_w / 2; _i++) printf("\x20");
            SetConsoleOutputCP(old_cp);
            if (s == 6) printf("\x1b[47;30m");
            printf("   ��   \x1b[0m  ");
            if (s == 7) printf("\x1b[47;30m");
            printf("������\x1b[0m");
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
    return _data;
}

/// @brief ������� ��� ������ ������� ���������
/// @param _lh ��������� �� ������ ������ ���������
/// @param _title ��������� �������
void show_table_with_title(list_header* _lh, char* _title) {
    COORD local_pos;
    char table_hints[] = "\x1b[47;30m N \x1b[0m ��������   \x1b[47;30mDEL\x1b[0m �������   \x1b[47;30m E \x1b[0m ��������   \x1b[47;30m ^S/^O \x1b[0m ������ � ������   \x1b[47;30m ^E \x1b[0m ������� � ����   \x1b[47;30mTab\x1b[0m ������� � ����";

    char format[20] = "\0";
    const char* headers[6] = {
        "������",
        "�������",
        "��� ��������",
        "���",
        "���������",
        "���������"
    };

    table_start:

    // �������� ��� �������
    uint table_width = main_size.X - 2;  // ������� ����� � ������ �� �������
    uint table_hight = main_size.Y - 2;  // ����� ��� ��������� � ��������� �������
    uint page_size = table_hight - 4;
    int current_page = 0;
    uint page_count = _lh->length / page_size;
    if (page_count*page_size < _lh->length) page_count++;

    uint group_width = 6;
    uint birthyear_width = 12;
    uint gender_width = 3;
    uint skipped_hours_with = 11;
    uint acuired_hours_width = 11;
    uint surname_width = table_width - 19 - group_width - birthyear_width - gender_width - skipped_hours_with - acuired_hours_width;

    int selected = 0;

    local_pos = main_pos;
    local_pos.X++;
    SCP(local_pos);
    chcp(1251);
    uint title_left_padding = (table_width - strlen(_title)) / 2;
    for (uint _i = 0; _i < title_left_padding; _i++) printf(" ");
    printf("%s", _title);
    for (uint _i = 0; _i < table_width - strlen(_title) - title_left_padding; _i++) printf(" ");

    draw_table:

    menu(0, _lh);
    show_second_hint(table_hints);

    local_pos = main_pos;
    local_pos.X++;
    local_pos.Y++;

    SCP(local_pos);
    chcp(866); printf("\xda\xc4");
    for (uint _i = 0; _i < group_width; _i++) printf("\xc4");
    printf("\xc4\xc2\xc4");
    for (uint _i = 0; _i < surname_width; _i++) printf("\xc4");
    printf("\xc4\xc2\xc4");
    for (uint _i = 0; _i < birthyear_width; _i++) printf("\xc4");
    printf("\xc4\xc2\xc4");
    for (uint _i = 0; _i < gender_width; _i++) printf("\xc4");
    printf("\xc4\xc2\xc4");
    for (uint _i = 0; _i < skipped_hours_with; _i++) printf("\xc4");
    printf("\xc4\xc2\xc4");
    for (uint _i = 0; _i < acuired_hours_width; _i++) printf("\xc4");
    printf("\xc4\xbf");

    local_pos.Y++;
    SCP(local_pos);

    // ����� ������� � ��������� "������"
    printf("\xb3 ");
    chcp(1251); printf("%s", headers[0]);
    chcp(866); printf(" \xb3 ");

    uint surname_header_left_padding = (surname_width - strlen(headers[1])) / 2;
    for (uint _i = 0; _i < surname_header_left_padding; _i++) printf(" ");
    chcp(1251); printf("%s", headers[1]);
    for (uint _i = 0; _i < surname_width - strlen(headers[1]) - surname_header_left_padding; _i++) printf(" ");
    chcp(866); printf(" \xb3 ");
    chcp(1251); printf("%s", headers[2]);
    chcp(866); printf(" \xb3 ");
    chcp(1251); printf("%s", headers[3]);
    chcp(866); printf(" \xb3  ");
    chcp(1251); printf("%s", headers[4]);
    chcp(866); printf("  \xb3  ");
    chcp(1251); printf("%s", headers[5]);
    chcp(866); printf("  \xb3");

    local_pos.Y++;
    SCP(local_pos);
    chcp(866); printf("\xc3\xc4");
    for (uint _i = 0; _i < group_width; _i++) printf("\xc4");
    printf("\xc4\xc1\xc4");
    for (uint _i = 0; _i < surname_width; _i++) printf("\xc4");
    printf("\xc4\xc1\xc4");
    for (uint _i = 0; _i < birthyear_width; _i++) printf("\xc4");
    printf("\xc4\xc1\xc4");
    for (uint _i = 0; _i < gender_width; _i++) printf("\xc4");
    printf("\xc4\xc1\xc4");
    for (uint _i = 0; _i < skipped_hours_with; _i++) printf("\xc4");
    printf("\xc4\xc1\xc4");
    for (uint _i = 0; _i < acuired_hours_width; _i++) printf("\xc4");
    printf("\xc4\xb4");

    uint showed_items = 0;
    list_item* tmp = get_student_by_id(_lh, current_page*page_size);
    for (uint _i = 0; _i < page_size && tmp; _i++, tmp = tmp->next, showed_items++) {
        local_pos.Y++;
        SCP(local_pos);
        printf(((_i+current_page*page_size) == selected)?"\xb3\x1b[47;30m ":"\xb3 ");
        chcp(1251); printf("%6.6s   ", tmp->inf->group);
        printf("%s", tmp->inf->surname);
        for (uint _j = 0; _j < surname_width - strlen(tmp->inf->surname); _j++) printf(" ");
        printf("       %4.4d        %c    %11d   %-11d", tmp->inf->birth_year, (tmp->inf->man)?'�':'�', tmp->inf->skipped_hours, tmp->inf->acquired_hours);
        chcp(866); printf(" \x1b[0m\xb3");
    }

    local_pos.Y++;
    SCP(local_pos);

    printf("\xc0");
    for (uint _i = 0; _i < table_width - 2; _i++) printf("\xc4");
    printf("\xd9");

    local_pos.Y++;
    SCP(local_pos);
    char page_string[26] = "\0";
    if (current_page <= 0) {  // ������ �������� (��� ����� ���������)
        if (page_count == 1) {  // ��� ���� ������ ��������
            sprintf(page_string, "    Page: %d / %d    ", current_page + 1, page_count);
        } else {
            sprintf(page_string, "    Page: %d / %d  ->", current_page + 1, page_count);
        }
    } else {
        if (current_page + 1 == page_count) {
            sprintf(page_string, "<-  Page: %d / %d    ", current_page + 1, page_count);
        } else {
            sprintf(page_string, "<-  Page: %d / %d  ->", current_page + 1, page_count);
        }
    }
    uint page_string_left_padding = (table_width - strlen(page_string)) / 2;
    for (uint _i = 0; _i < page_string_left_padding; _i++) printf(" ");
    printf("%s", page_string);
    for (uint _i = 0; _i < table_width - strlen(page_string) - page_string_left_padding; _i++) printf(" ");
    
    for (uint _i = 0; _i < page_size - showed_items; _i++) {
        local_pos.Y++;
        SCP(local_pos);
        for (uint _j = 0; _j < table_width; _j++) printf(" ");
    }

    // �� � ������ ���������� �������� ���������
    while (1) {
        uchar a = _getch();
        COORD _local_pos = menu_pos;
        _local_pos.Y += menu_size.Y - 1;
        SCP(_local_pos);
        printf("Entered %d    ", a);
        if (a == 'e' || a == 243 || a == 13) {  // Edit
            student* editable = get_student_by_id(_lh, selected)->inf;
            *(editable) = *(enter_data(editable));
            goto draw_table;
        } else if (a == 'n' || a == 226) {  // Add new
            student* _st = enter_data(NULL);
            if (_st) add_student(_lh, *_st);
            goto draw_table;
        } else if (a == 224) {  // Arrow key
            uchar b = _getch();
            if (b == 83) {  // Delete
                remove_student_by_id(_lh, selected);
                if (selected % page_size == 0) {current_page--; selected--;}
                if (current_page < 0) current_page = 0;
                if (selected < 0) selected = 0;
                if (selected > _lh->length) selected--;
                page_count = _lh->length / page_size;
                if (page_count*page_size < _lh->length+1) page_count++;
                if (current_page >= page_count) current_page = page_count - 1;
                goto draw_table;
            } else if (b == 71 && current_page > 0) {  // Home
                selected -= current_page * page_size;
                current_page = 0;
                goto draw_table;
            } else if (b == 79 && (current_page+1)*page_size < _lh->length) {  // End
                selected = (page_count-1)*page_size + selected - current_page*page_size;
                if (selected >= _lh->length) selected = _lh->length;
                current_page = page_count - 1;
                goto draw_table;
            } else if (b == 75 && current_page > 0) {  // Left
                selected -= page_size;
                current_page--;
                goto draw_table;
            } else if (b == 77 && current_page < page_count-1) {  // Right
                selected += page_size;
                current_page++;
                if (selected >= _lh->length) selected = _lh->length;
                goto draw_table;
            } else if (b == 72 && selected > current_page*page_size) {  // Arrow Up
                selected--;
                local_pos.Y = main_pos.Y + 4 + (selected % page_size);
                SCP(local_pos);

                tmp = get_student_by_id(_lh, selected);
                printf("\xb3\x1b[47;30m ");
                chcp(1251); printf("%6.6s   ", tmp->inf->group);
                printf("%s", tmp->inf->surname);
                for (uint _i = 0; _i < surname_width - strlen(tmp->inf->surname); _i++) printf(" ");
                printf("       %4.4d        %c    %11d   %-11d", tmp->inf->birth_year, (tmp->inf->man)?'�':'�', tmp->inf->skipped_hours, tmp->inf->acquired_hours);
                chcp(866); printf(" \x1b[0m\xb3");

                local_pos.Y++;
                SCP(local_pos);
                tmp = tmp->next;
                printf("\xb3\x1b[0m ");
                chcp(1251); printf("%6.6s   ", tmp->inf->group);
                printf("%s", tmp->inf->surname);
                for (uint _i = 0; _i < surname_width - strlen(tmp->inf->surname); _i++) printf(" ");
                printf("       %4.4d        %c    %11d   %-11d", tmp->inf->birth_year, (tmp->inf->man)?'�':'�', tmp->inf->skipped_hours, tmp->inf->acquired_hours);
                chcp(866); printf(" \x1b[0m\xb3");
            } else if (b == 72 && current_page > 0) {  // Arrow Up � To prev page
                selected--;
                current_page--;
                goto draw_table;
            } else if (b == 80 && selected < (current_page+1)*page_size - 1 && selected < _lh->length) {  // Arrow Down
                selected++;
                local_pos.Y = main_pos.Y + 3 + (selected % page_size);
                SCP(local_pos);

                tmp = get_student_by_id(_lh, selected-1);
                printf("\xb3\x1b[0m ");
                chcp(1251); printf("%6.6s   ", tmp->inf->group);
                printf("%s", tmp->inf->surname);
                for (uint _i = 0; _i < surname_width - strlen(tmp->inf->surname); _i++) printf(" ");
                printf("       %4.4d        %c    %11d   %-11d", tmp->inf->birth_year, (tmp->inf->man)?'�':'�', tmp->inf->skipped_hours, tmp->inf->acquired_hours);
                chcp(866); printf(" \x1b[0m\xb3");

                local_pos.Y++;
                SCP(local_pos);
                tmp = tmp->next;
                printf("\xb3\x1b[47;30m ");
                chcp(1251); printf("%6.6s   ", tmp->inf->group);
                printf("%s", tmp->inf->surname);
                for (uint _i = 0; _i < surname_width - strlen(tmp->inf->surname); _i++) printf(" ");
                printf("       %4.4d        %c    %11d   %-11d", tmp->inf->birth_year, (tmp->inf->man)?'�':'�', tmp->inf->skipped_hours, tmp->inf->acquired_hours);
                chcp(866); printf(" \x1b[0m\xb3");
            } else if (b == 80 && current_page < page_count - 1) {  // Arrow Down � To next page
                selected++;
                current_page++;
                goto draw_table;
            }
        } else if (a == 3 || a == 27) {  // Ctrl + C
            del_list(_lh);
            system("cls");
            return;
        } else if (a == 5) {  // Ctrl + E
            // Export to csv or txt file
            export_list_to_file(_lh);
            goto draw_table;
        } else if (a == 9) {  // Tab � switch to menu
            menu(1, _lh);
            goto draw_table;
        } else if (a == 15) {  // Ctrl + O
            read_list_from_file(_lh);
            goto draw_table;
        } else if (a == 18) {  // Ctrl + R
            create_layout();
            goto table_start;
        } else if (a == 19) {  // Ctrl + S
            save_list_to_file(_lh);
            goto draw_table;
        }
        // show_second_hint(table_hints);
    }
}

/// @brief ����� ��� ������ ������� show_table_with_title() � ���������� �� ���������
/// @param _lh ��������� �� ������ ������
inline void show_table(list_header* _lh) { show_table_with_title(_lh, "������ ���������"); }

/// @brief ������� ��� �������� ������ � ���� (��� ����� ������������ ������������� �� ����������)
/// @param _lh ��������� �� ������ ������ ��� ��������
inline void export_list_to_file(list_header* _lh) {
    if (!_lh || !(_lh->first)) return;
    char file_name[47] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    COORD local_pos = main_pos;
    local_pos.Y += (main_size.Y - 4) / 2;
    local_pos.X += (main_size.X - 50) / 2;
    chcp(866);
    SCP(local_pos);
    printf("\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3        ");
    chcp(1251);
    printf("������� ��� ����� ��� ����������");
    chcp(866);
    printf("        \xB3");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3 ");
    chcp(1251);
    printf("%s", file_name);
    for (uint _i = 0; _i < 46 - strlen(file_name); _i++) printf("_");
    chcp(866);
    printf(" \xB3");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9");
    local_pos.Y--;
    while (1) {
        uchar a = _getch();
        if (a == 13) {  // Enter
            char extension[4] = {0};
            for (uint _i = 0; _i < 3; _i++) extension[_i] = file_name[strlen(file_name) - 3 + _i];
            if (!strcmp(strlower(extension), "txt")) {  // Export to TXT file
                FILE* out = fopen(file_name, "wt");
                if (!out) {
                    SCP(local_pos);
                    printf("\xB3 ");
                    chcp(1251);
                    printf("            ������ �������� �����.            ");
                    Sleep(1000);
                    return;
                }
                for (list_item* tmp = _lh->first; tmp; tmp = tmp->next) {
                    fprintf(
                        out,
                        "�������:\n    ������: %s\n    �������: %s\n    ���: %s\n    ��� ��������: %d\n    ��������� �����: %d\n    ��������� �����: %d\n    ������������� �����: %d\n\n",
                        tmp->inf->group,
                        tmp->inf->surname,
                        tmp->inf->man?"�������":"�������",
                        tmp->inf->birth_year,
                        tmp->inf->skipped_hours,
                        tmp->inf->acquired_hours,
                        tmp->inf->skipped_hours - tmp->inf->acquired_hours
                    );
                }
                fclose(out);
            } else if (!strcmp(strlower(extension), "csv")) {  // Export to CSV file
                FILE* out = fopen(file_name, "wt");
                if (!out) {
                    SCP(local_pos);
                    printf("\xB3 ");
                    chcp(1251);
                    printf("            ������ �������� �����.            ");
                    Sleep(1000);
                    return;
                }
                fprintf(out, "\"������\";\"�������\";\"���\";\"��� ��������\";\"��������� �����\";\"��������� �����\";\"����������� �����\";\n");
                for (list_item* tmp = _lh->first; tmp; tmp = tmp->next) {
                    fprintf(
                        out,
                        "\"%s\";\"%s\";\"%s\";%d;%d;%d;%d;\n",
                        tmp->inf->group,
                        tmp->inf->surname,
                        tmp->inf->man?"�������":"�������",
                        tmp->inf->birth_year,
                        tmp->inf->skipped_hours,
                        tmp->inf->acquired_hours,
                        tmp->inf->skipped_hours - tmp->inf->acquired_hours
                    );
                }
                fclose(out);
            } else {  // Unknown extension
                SCP(local_pos);
                printf("\xB3 ");
                chcp(1251);
                printf("        ���������� ����� �� ����������        ");
                Sleep(1000);
            }
            return;
        } else if (a == 224) {  // Filter for arrows and other special keys
            _getch();
        } else if ((a >= 32 && a <= 126) || (a >= 192 && a <= 255)) {  // Any character
            if (strlen(file_name) < 46) file_name[strlen(file_name)] = a;
        } else if (a == 8) {  // Backspace
            file_name[strlen(file_name) - 1] = '\0';
        } else if (a == 27) {  // ESC
            return;
        }
        SCP(local_pos);
        printf("\xB3 ");
        chcp(1251);
        printf("%s", file_name);
        for (uint _i = 0; _i < 46 - strlen(file_name); _i++) printf("_");
        chcp(866);
    }
}

/// @brief ������� ��� ������� ����� ����� �� ������������ � ���������� ������ � ���� ����
/// @param _lh ��������� �� ������ ������ ��� ������
inline void save_list_to_file(list_header* _lh) {
    if (!_lh || !(_lh->first)) return;
    char file_name[47] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    COORD local_pos = main_pos;
    local_pos.Y += (main_size.Y - 4) / 2;
    local_pos.X += (main_size.X - 50) / 2;
    chcp(866);
    SCP(local_pos);
    printf("\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3        ");
    chcp(1251);
    printf("������� ��� ����� ��� ����������");
    chcp(866);
    printf("        \xB3");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3 ");
    chcp(1251);
    printf("%s", file_name);
    for (uint _i = 0; _i < 46 - strlen(file_name); _i++) printf("_");
    chcp(866);
    printf(" \xB3");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9");
    local_pos.Y--;
    while (1) {
        uchar a = _getch();
        if (a == 13) {
            // Enter
            // ������� ���� ��� ������ � ��������� � ���� ������
            FILE* out = fopen(file_name, "ab");
            if (!out) {  // ���� �� ������� ������� (�������) ���� ��� ������
                local_pos.X+=2;
                SCP(local_pos);
                chcp(1251);
                printf("           �� ������� ������� ����.           ");
                Sleep(1000);
            } else {
                fseek(out, 0, SEEK_END);
                if (ftell(out) > 0) {
                    // ���� ���� ��� ���-�� ��������
                    local_pos.X+=2;
                    SCP(local_pos);
                    chcp(1251);
                    printf(" ������� A ��� �������� ��� W ��� ����������. ");
                    uchar b = _getch();
                    while (b != 'w' && b != '�' && b != 'a' && b != '�' && b != 27) b = _getch();
                    if (b == 'w' || b == '�') {
                        freopen(file_name, "wb", out);
                        fseek(out, 0, SEEK_SET);
                        // Rewrite...
                        for (list_item* _i = _lh->first; _i; _i = _i->next) {
                            if (!fwrite(_i->inf, sizeof(student), 1, out)) {
                                SCP(local_pos);
                                chcp(1251);
                                printf("             ������ ������ � ����             ");
                                Sleep(1000);
                                return;
                            }
                        }
                        SCP(local_pos);
                        chcp(1251);
                        printf("            ������ �������� � ����            ");
                        Sleep(1000);
                        return;
                    } else if (b == 'a' || b == '�') {
                        // Write to end
                        for (list_item* _i = _lh->first; _i; _i = _i->next) {
                            if (!fwrite(_i->inf, sizeof(student), 1, out)) {
                                SCP(local_pos);
                                chcp(1251);
                                printf("             ������ ������ � ����             ");
                                Sleep(1000);
                                return;
                            }
                        }
                        SCP(local_pos);
                        chcp(1251);
                        printf("            ������ �������� � ����            ");
                        Sleep(1000);
                        return;
                    } else if (b == 27) {
                        // ESC
                        SCP(local_pos);
                        chcp(1251);
                        printf("                   ��������                   ");
                        Sleep(1000);
                        return;
                    }
                } else {
                    // ���� ���� ���� (��� ������ ������, ��� �� ���� �� �� �����)
                    for (list_item* _i = _lh->first; _i; _i = _i->next) {
                        if (!fwrite(_i->inf, sizeof(student), 1, out)) {
                            SCP(local_pos);
                            chcp(1251);
                            printf("             ������ ������ � ����             ");
                            Sleep(1000);
                            return;
                        }
                    }
                    local_pos.X+=2;
                    SCP(local_pos);
                    chcp(1251);
                    printf("            ������ �������� � ����            ");
                    Sleep(1000);
                }
            }
            return;
        } else if (a == 224) {
            _getch();
        } else if ((a >= 32 && a <= 126) || (a >= 192 && a <= 255)) {
            if (strlen(file_name) < 46) file_name[strlen(file_name)] = a;
        } else if (a == 8) {
            file_name[strlen(file_name) - 1] = '\0';
        } else if (a == 27) {
            // ESC
            return;
        }
        SCP(local_pos);
        printf("\xB3 ");
        chcp(1251);
        printf("%s", file_name);
        for (uint _i = 0; _i < 46 - strlen(file_name); _i++) printf("_");
        chcp(866);
        printf(" \xB3");
    }
}

/// @brief ������� ��� ������ ������ �� �����
/// @param _lh ��������� �� ������ ������, � ������� ����� �������� ������
inline void read_list_from_file(list_header* _lh) {
    if (!_lh) return;
    char file_name[47] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    COORD local_pos = main_pos;
    local_pos.Y += (main_size.Y - 4) / 2;
    local_pos.X += (main_size.X - 50) / 2;
    chcp(866);
    SCP(local_pos);
    printf("\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3        ");
    chcp(1251);
    printf("  ������� ��� ����� ��� ������  ");
    chcp(866);
    printf("        \xB3");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3 ");
    chcp(1251);
    printf("%s", file_name);
    for (uint _i = 0; _i < 46 - strlen(file_name); _i++) printf("_");
    chcp(866);
    printf(" \xB3");
    local_pos.Y++;
    SCP(local_pos);
    printf("\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9");
    local_pos.Y--;
    local_pos.X += 2;
    while (1) {
        uchar a = _getch();
        if (a == 13) {
            // Enter
            // ������� ���� ��� ������ � ��������� � ���� ������
            FILE* in = fopen(file_name, "rb");
            if (!in) {  // ���� �� ������� ������� ���� ��� ������
                SCP(local_pos);
                chcp(1251);
                printf("           �� ������� ������� ����.           ");
                _getch();
            } else {
                fseek(in, 0, SEEK_END);
                if (!ftell(in)) {
                    SCP(local_pos);
                    chcp(1251);
                    printf("                  ���� ����.                  ");
                    _getch();
                    return;
                }
                fseek(in, 0, SEEK_SET);
                while (_lh->first) remove_student_by_id(_lh, 0);
                student tmp = { 0 };
                while (fread(&tmp, sizeof(student), 1, in)) add_student(_lh, tmp);
            }
            return;
        } else if (a == 224) {
            _getch();
        } else if ((a >= 32 && a <= 126) || (a >= 192 && a <= 255)) {
            if (strlen(file_name) < 46) file_name[strlen(file_name)] = a;
        } else if (a == 8) {
            file_name[strlen(file_name) - 1] = '\0';
        } else if (a == 27) {
            // ESC
            return;
        }
        SCP(local_pos);
        chcp(1251);
        printf("%s", file_name);
        for (uint _i = 0; _i < 46 - strlen(file_name); _i++) printf("_");
    }
}

/// @brief ������� ��� ������ ����
/// @param _interactive ����� �� ������������ ����������������� � ���� ��� ������ ������
inline void menu(char _interactive, list_header* _lh) {
    #define _menu_items 8
    COORD local_pos = menu_pos;
    COORD item_pos[8] = {
        {menu_pos.X, menu_pos.Y},     // ����������� ����������
        {menu_pos.X, menu_pos.Y+1},   // ����������� �� ������
        {menu_pos.X, menu_pos.Y+2},   // ����������� �� �������
        {menu_pos.X, menu_pos.Y+3},   // ����������� �� ���� ��������
        {menu_pos.X, menu_pos.Y+4},   // ����������� �� ����
        {menu_pos.X, menu_pos.Y+5},   // ����������� �� ����� ����������� �����
        {menu_pos.X, menu_pos.Y+6},   // ����������� �� ����� ����������� �����
        {menu_pos.X, menu_pos.Y+7},   // ����������� �� ����� ������������� �����
    };
    const char* item_str[_menu_items] = {
        "����������",
        "  �� ������",
        "  �� �������",
        "  �� ���� ��������",
        "  �� ����",
        "  �� ����������� �����",
        "  �� ����������� �����",
        "  �� ������������� �����",
    };
    chcp(1251);
    if (!_interactive) {
        // Print only items, without selecting
        SCP(item_pos[0]);
        printf("%s: %s", item_str[0], ((_sort_direction) ? ("�� �����������") : ("�� ��������")));
        for (uint _i = 0; _i < (menu_size.X - strlen(item_str[0]) - strlen((_sort_direction) ? ("�� �����������") : ("�� ��������")) - 2); _i++) printf("\x20");
        for (int _i = 1; _i < 8; _i++) {
            SCP(item_pos[_i]);
            printf("%s", item_str[_i]);
            for (uint _j = 0; _j < (menu_size.X - strlen(item_str[_i])); _j++) printf("\x20");
        }
        return;
    } else {
        // Print menu with selected
        clear_second_hint();
        show_second_hint("\x1b[47;30m Enter \x1b[0m �������   \x1b[47;30m Up/Down \x1b[0m ����������� �������    \x1b[47;30m Tab \x1b[0m ��������� � �������    ");

        while (1) {
            SCP(item_pos[0]);
            if (menu_selected == 0) printf("\x1b[47;30m");
            printf("%s: %s", item_str[0], ((_sort_direction) ? ("�� �����������") : ("�� ��������")));
            for (uint _i = 0; _i < (menu_size.X - strlen(item_str[0]) - strlen((_sort_direction) ? ("�� �����������") : ("�� ��������")) - 2); _i++) printf("\x20");
            printf("\x1b[0m");
            for (int _i = 1; _i < 8; _i++) {
                SCP(item_pos[_i]);
                if (_i == menu_selected) printf("\x1b[47;30m");
                printf("%s", item_str[_i]);
                for (uint _j = 0; _j < (menu_size.X - strlen(item_str[_i])); _j++) printf("\x20");
                printf("\x1b[0m");
            }
            uchar a = _getch();
            if (a == 224) {
                // ���� ������ �������
                uchar b = _getch();
                if (b == 72) {
                    // ������� �����
                    if (menu_selected > 0) menu_selected--;
                } else if (b == 80) {
                    // ������� ����
                    if (menu_selected < _menu_items - 1) menu_selected++;
                } else if (b == 75 || b == 77) {
                    // ������� ����� ��� ������
                    if (menu_selected == 0) _sort_direction = !_sort_direction;
                }
            } else if (a == 9) {
                // Tab
                SCP(item_pos[menu_selected]);
                if (menu_selected == 0) printf("%s: %s", item_str[0], ((_sort_direction) ? ("�� �����������") : ("�� ��������")));
                else printf("%s", item_str[menu_selected]);
                if (menu_selected != 0) for (uint _j = 0; _j < (menu_size.X - strlen(item_str[menu_selected])); _j++) printf("\x20");
                else for (uint _i = 0; _i < (menu_size.X - strlen(item_str[0]) - strlen((_sort_direction) ? ("�� �����������") : ("�� ��������")) - 2); _i++) printf("\x20");
                printf("\x1b[0m");
                return;
            } else if (a == 13) {
                // Enter
                if (menu_selected == 0) _sort_direction = !_sort_direction;
                else {
                    sort_list(_lh, menu_selected, _sort_direction);
                    menu(0, _lh);
                    return;
                }
            } else if (a == 3) {
                del_list(_lh);
                system("cls");
                exit(0);
            }
        }
    }
}

/// @brief ������� ��� ���������� ������
/// @param _lh ��������� �� ������ ������
/// @param _field ����� ����, �� �������� ����������� (1 � ������, ... , 7 � ����� ������������� �����)
/// @param _direction ����������� ���������� (1 � �� �����������, 0 � �� ��������)
inline void sort_list(list_header* _lh, char _field, char _direction) {
    if (!_lh || !(_lh->first) || !(_lh->first->next)) return;
    if (_direction == 1) {
        switch (_field) {
            case 1:  // ���������� �� ������
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (strcmp(pmin->inf->group, _j->inf->group) > 0) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 2:  // ���������� �� �������
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (strcmp(pmin->inf->surname, _j->inf->surname) > 0) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 3:  // ���������� �� ���� ��������
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (pmin->inf->birth_year > _j->inf->birth_year) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 4:  // ���������� �� ����
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (pmin->inf->man > _j->inf->man) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 5:  // ���������� �� ����������� �����
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (pmin->inf->skipped_hours > _j->inf->skipped_hours) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 6:  // ���������� �� ����������� �����
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (pmin->inf->acquired_hours > _j->inf->acquired_hours) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 7:  // ���������� �� ������������� �����
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if ((pmin->inf->skipped_hours - pmin->inf->acquired_hours) > (_j->inf->skipped_hours - _j->inf->acquired_hours)) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
        }
    }
    else {
        switch (_field) {
            case 1:  // ���������� �� ������
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (strcmp(pmin->inf->group, _j->inf->group) < 0) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 2:  // ���������� �� �������
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (strcmp(pmin->inf->surname, _j->inf->surname) < 0) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 3:  // ���������� �� ���� ��������
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (pmin->inf->birth_year < _j->inf->birth_year) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 4:  // ���������� �� ����
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (pmin->inf->man < _j->inf->man) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 5:  // ���������� �� ����������� �����
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (pmin->inf->skipped_hours < _j->inf->skipped_hours) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 6:  // ���������� �� ����������� �����
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if (pmin->inf->acquired_hours < _j->inf->acquired_hours) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
            case 7:  // ���������� �� ������������� �����
                for (list_item* _i = _lh->first; _i->next; _i = _i->next) {
                    list_item* pmin = _i;
                    for (list_item* _j = _i->next; _j; _j = _j->next) if ((pmin->inf->skipped_hours - pmin->inf->acquired_hours) < (_j->inf->skipped_hours - _j->inf->acquired_hours)) pmin = _j;
                    if (pmin != _i) {
                        student* tmp = _i->inf;
                        _i->inf = pmin->inf;
                        pmin->inf = tmp;
                    }
                }
                break;
        }
    }
}

/// @brief ������� ��� ������� ������� � ����
inline void clear_menu() {
    COORD local_pos = menu_pos;
    for (uint _i = 0; _i < menu_size.Y; _i++, local_pos.Y++) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
        for (uint _j = 0; _j < menu_size.X; _j++) printf("\x20");
    }
}

/// @brief ������� ��� ������� ������� � �����������
inline void clear_hint() {
    COORD local_pos = hint_pos;
    for (uint _i = 0; _i < 2; _i++, local_pos.Y++) {
        SCP(local_pos);
        for (uint _j = 0; _j < hint_size.X; _j++) printf("\x20");
    }
}

/// @brief ������� ��� ������� ������ ������ ������� ���������
inline void clear_second_hint() {
    COORD local_pos = hint_pos;
    local_pos.Y++;
    SCP(local_pos);
    for (uint _j = 0; _j < hint_size.X; _j++) printf("\x20");
}
