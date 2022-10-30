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

// ������� ����� � ������ ��� ��������
#define lp 3
#define tp 2

#define getch_thread_timeout 10

COORD screen_pos = { 0, 0 };      // ������� ����������
COORD screen_end_pos = { 0, 0 };  // ���������� ����� ������ (������������ ��� ������� ��� �����������)
COORD border_pos = { lp, tp };    // ���������� ������ �����
COORD menu_pos = { 4, 3 };        // ���������� ����� ����
COORD menu_size = { 0, 0 };       // ������� ����� ���� (������������ ��� ������� ��� �����������)
COORD main_pos = { 44, 3 };       // ���������� �������� ����� (����� � �������) (������������ ��� ������� ��� �����������)
COORD main_size = { 0, 0 };       // ������� �������� ����� (������������ ��� ������� ��� �����������)
COORD hint_pos = { 4, 0 };        // ���������� ����� ��������� (������������ ��� ������� ��� �����������)
COORD hint_size = { 0, 0 };       // ������� ����� ��������� (������������ ��� ������� ��� �����������)

/// @brief ����� ��� ��������� ����������� ������� � �������
/// @param _pos ������� ������� � ��������� COORD
void SCP(COORD _pos) { SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _pos); }

/// @brief ����� ��� ��������� ��������� ������
/// @param _cp ����� ���������
void chcp(int _cp) { SetConsoleOutputCP(_cp); }

/// @brief ������� ��� �������� ������
/// @return ��������� �� ������ ������ ������ (��� NULL � ������ ������)
list_header* new_list() {
    list_header* res = (list_header*)calloc(1, sizeof(list_header));
    return res;
}

/// @brief �������� ����� ������ ��� ��������������!
/// @param _lh ��������� �� ������ ���������� ������
void del_list(list_header* _lh) {
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
student* get_student_by_id(list_header* _lh, uint _id) {
    if (!_lh) return NULL;
    list_item* tmp = _lh->first;
    if (!tmp) return NULL;
    for (uint _i = 0; _i <= _id && tmp; _i++, tmp = tmp->next) {
        if (_i == _id) return tmp->inf;
    }
    return NULL;
}

/// @brief �������� �������� �� ��� ����������� ������
/// @param _lh ��������� �� ������ ������
/// @param _id ID ���������� ��������
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

/// @brief ������� ��� �������� (��� �����������) ��������� ����������
void create_layout() {
    system("cls");
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(866);
    CONSOLE_CURSOR_INFO inf;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    inf.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &inf);
    CONSOLE_SCREEN_BUFFER_INFO buff_info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buff_info)) {
        screen_end_pos = buff_info.dwSize;
    } else {
        exit(1);
    }
    if (screen_end_pos.X < 80 || screen_end_pos.Y < 30) {
        printf("ERROR: You need to open console in fullscreen, because your console size is too small\n");
        getchar();
    }
    COORD border_lt = { lp, tp };
    uint window_w = screen_end_pos.X - lp * 2;
    uint window_h = screen_end_pos.Y - tp * 2;
    main_size.X = window_w - 42;
    main_size.Y = window_h - 5;
    hint_size.Y = 2;
    hint_size.X = window_w - 2;
    menu_pos.X = border_lt.X + 1;
    menu_pos.Y = border_lt.Y + 1;
    menu_size.X = 39;
    menu_size.Y = window_h - 5;
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

    // ������� ���
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_pos);
    for (uint _i = 0; _i < screen_end_pos.X * 2; _i++) printf("\x1b[3%cm%c", '1' + (rand() % 7), '0' + (rand() % 2));
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

    for (uint _i = 0; _i < screen_end_pos.X * 2; _i++) printf("\x1b[3%cm%c", '1' + (rand() % 7), '0' + (rand() % 2));
    printf("\x1b[0m");
    SetConsoleOutputCP(old_cp);
}

/// @brief ������� ��� ������ ��������� � �������� �������
void show_base_hint(char* _hint) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), hint_pos);
    int old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(1251);
    printf("%s", _hint);
    SetConsoleOutputCP(old_cp);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), screen_end_pos);
}

/// @brief ������� ��� ������ ��������� � �������� �������
/// @param _hint ��-������, ������� ����� ������� (����������� ���������� �������� \0)
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

/// @brief ������� ��� ������� ������� ������� (������� � �������)
void clear_main() {
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
    return strlen(r);
}

/// @brief ������� ��� ����� ��� ��������� ������
/// @param _data ��������� �� ���������� �������������� ���� ��� NULL ��� �������� ������ 
/// @return ��������� �� �������������� ���� � ����������� �� ������������ �������
student* enter_data(student* _d) {
    if (!_d) _d = (student*)calloc(1, sizeof(student));
    student* _data = (student*)calloc(1, sizeof(student));
    *_data = *_d;
    COORD local_pos = main_pos;
    local_pos.X += 2;
    local_pos.Y++;
    uint local_h = screen_end_pos.Y - tp * 2 - 7,
        local_w = screen_end_pos.X - lp * 2 - 46;
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
    printf("%20.20s\x1b[0m\x20%s", "������", _data->group);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - strlen(_data->group); _i++) printf("_");
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
    printf("%20.20s\x1b[0m\x20%s", "�������", _data->surname);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
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
    printf("%20.20s\x1b[0m\x20%d", "��� ��������", _data->birth_year);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
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
    printf("%20.20s\x1b[0m\x20", "���");
    printf((_data->man) ? "�������" : "�������");
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
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
    printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->skipped_hours);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
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
    printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
    SetConsoleOutputCP(866);
    for (uint _i = 0; _i < local_w - 25 - intlen(_data->acquired_hours); _i++) printf("_");
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
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[0]);
            if (s == 0) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "������", _data->group);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->group); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[1]);
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "�������", _data->surname);
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
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[0]);
            if (s == 0) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "������", _data->group);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->group); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[1]);
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "�������", _data->surname);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[2]);
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��� ��������", _data->birth_year);
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
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[1]);
            if (s == 1) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%s", "�������", _data->surname);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - strlen(_data->surname); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[2]);
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��� ��������", _data->birth_year);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20", "���");
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
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[2]);
            if (s == 2) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��� ��������", _data->birth_year);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->birth_year); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20", "���");
            printf((_data->man) ? "�������" : "�������");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->skipped_hours);
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
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[3]);
            if (s == 3) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20", "���");
            printf((_data->man) ? "�������" : "�������");
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 32; _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
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
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[4]);
            if (s == 4) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->skipped_hours);
            SetConsoleOutputCP(866);
            for (uint _i = 0; _i < local_w - 25 - intlen(_data->skipped_hours); _i++) printf("_");
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
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
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
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
                clear_main();
                return _d;
            }
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), item_pos[5]);
            if (s == 5) printf("\x1b[47;30m");
            SetConsoleOutputCP(old_cp);
            printf("%20.20s\x1b[0m\x20%d", "��������� �����", _data->acquired_hours);
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
    clear_main();
    return _data;
}

/// @brief ������� ��� ������ ������� ���������
/// @param _lh ��������� �� ������ ������ ���������
void show_table(list_header* _lh) {
    uint page_size = main_size.Y - 6;
    // � ������ �������� ������������ ������� �� ���� ������������ � ������
    if (main_size.X < 76 || main_size.Y < 9) {
        SCP(main_pos);
        for (uint _i = 0; _i < (main_pos.X - 45) / 2; _i++) printf("\x20");
        printf("������ �����������. ������������ ������������");
        _getch();
        clear_main();
        return;
    }
    // ���� �� �����, ��������� ������
    int s = 0;
    uint cur_page = 0;
    menu(0, _lh);
    show_second_hint("\x1b[47;30m N \x1b[0m ��������   \x1b[47;30m DEL \x1b[0m �������   \x1b[47;30m E / Enter \x1b[0m ��������   \x1b[47;30m Up/Down \x1b[0m ����������� �������   \x1b[47;30m Left/Right \x1b[0m ����� ��������    \x1b[45;30m ^S \x1b[0m ��������� � ����    \x1b[47;30m Tab \x1b[0m ������� � ����    ");
    draw_table:  // ���������� ��� GOTO
    chcp(866);
    COORD local_pos = main_pos;
    local_pos.Y++, local_pos.X += 2;
#pragma region ����� �������
    SCP(local_pos);
    printf("\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC2");  // Horizontal line
    // 46 (R) + 10 (L) + 4 (Padding)
    for (uint _i = 0; _i < main_size.X - 60; _i++) printf("\xC4");
    printf("\xC2\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF");  // TopRight
    local_pos.Y++;
    SCP(local_pos);
    printf("\xB3 ");
    chcp(1251); printf("������ ");
    chcp(866); printf("\xB3 ");
    for (uint _i = 0; _i < (main_size.X - 69) / 2; _i++) printf("\x20");
    chcp(1251); printf("�������");
    chcp(866);
    for (uint _i = 0; _i < (main_size.X - 69) - (main_size.X - 69) / 2; _i++) printf("\x20");
    printf(" \xB3 ");
    chcp(1251); printf("��� �������� ");
    chcp(866); printf("\xB3 ");
    chcp(1251); printf("��� ");
    chcp(866); printf("\xB3 ");
    chcp(1251); printf("��������� ");
    chcp(866); printf("\xB3 ");
    chcp(1251); printf("��������� ");
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
        chcp(1251); printf((tmp->man) ? "�" : "�");
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
    for (uint _i = 0; _i < main_size.X / 2 - (5 + intlen(cur_page+1) + intlen((_lh->length % page_size) ? (_lh->length / page_size + 1) : (_lh->length / page_size))); _i++) printf("\x20");
    printf("Page: %d / %d", cur_page+1, (_lh->length % page_size) ? (_lh->length / page_size + 1) : (_lh->length / page_size));
    for (uint _i = 0; _i < main_size.X / 2 - (5 + intlen(cur_page+1) + intlen((_lh->length % page_size) ? (_lh->length / page_size + 1) : (_lh->length / page_size))); _i++) printf("\x20");
    while (local_pos.Y < main_pos.Y + main_size.Y - 1) {
        local_pos.Y++;
        SCP(local_pos);
        for (uint _i = 0; _i < main_size.X - 4; _i++) printf("\x20");
    }

    while (1) {
        uchar a = _getch();
        COORD _local_pos = menu_pos;
        _local_pos.Y += menu_size.Y - 1;
        SCP(_local_pos);
        printf("Entered %d    ", a);
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
            if (b == 72 && s > cur_page*page_size) {
                s--;
                if (s > _lh->length) s = _lh->length;
                // SCP(menu);
                // uint last_on_page = page_size*(cur_page+1) - 1;
                // uint post = (s < last_on_page) ? s+1 : s;
                // printf("S = %d, LOP = %d, POST = %d        ", s, last_on_page, post);
                for (uint _i = s; _i <= ((s < page_size*(cur_page+1) - 1) ? s+1 : s); _i++) {
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
                    chcp(1251); printf((tmp->man) ? "�" : "�");
                    chcp(866); printf("\x20\x20\xB3\x20");
                    printf("%9d\x20\xB3\x20%-9d\x20\x1b[0m\xB3", tmp->skipped_hours, tmp->acquired_hours);
                }
            } else if (b == 80 && s < _lh->length && s < (cur_page+1)*page_size - 2) {
                s++;
                for (uint _i = (s>cur_page*page_size)?s-1:s; _i <= s; _i++) {
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
                    chcp(1251); printf((tmp->man) ? "�" : "�");
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
        } else if (a == 9) {
            // Tab � switch to menu
            menu(1, _lh);
            show_second_hint("\x1b[47;30m N \x1b[0m ��������   \x1b[47;30m DEL \x1b[0m �������   \x1b[47;30m E / Enter \x1b[0m ��������   \x1b[47;30m Up/Down \x1b[0m ����������� �������   \x1b[47;30m Left/Right \x1b[0m ����� ��������    \x1b[45;30m ^S \x1b[0m ��������� � ����    \x1b[47;30m Tab \x1b[0m ������� � ����    ");
            goto draw_table;
        } else if (a == 19) {
            // Ctrl + S
            //
        }
    }
}

/// @brief ������� ��� ������ ����
/// @param _interactive ����� �� ������������ ����������������� � ���� ��� ������ ������
/// @return ��� ������ (��� ���������������� ������ ������ 0)
int menu(char _interactive, list_header* _lh) {
    int selected = 0;
    #define _menu_items 8
    char _sort_direction = 1;
    COORD local_pos = menu_pos;
    COORD item_pos[8] = {
        {menu_pos.X, menu_pos.Y},                                                         // ����������� ����������
        {menu_pos.X, menu_pos.Y+1},   // ����������� �� ������
        {menu_pos.X, menu_pos.Y+2},   // ����������� �� �������
        {menu_pos.X, menu_pos.Y+3},   // ����������� �� ���� ��������
        {menu_pos.X, menu_pos.Y+4},   // ����������� �� ����
        {menu_pos.X, menu_pos.Y+5},  // ����������� �� ����� ����������� �����
        {menu_pos.X, menu_pos.Y+6},  // ����������� �� ����� ����������� �����
        {menu_pos.X, menu_pos.Y+7},  // ����������� �� ����� ������������� �����
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
        if (selected == 0) printf("\x1b[47;30m");
        printf("%s: %s", item_str[0], ((_sort_direction) ? ("�� �����������") : ("�� ��������")));
        for (uint _i = 0; _i < (menu_size.X - strlen(item_str[0]) - strlen((_sort_direction) ? ("�� �����������") : ("�� ��������")) - 2); _i++) printf("\x20");
        printf("\x1b[0m");
        for (int _i = 1; _i < 8; _i++) {
            SCP(item_pos[_i]);
            if (_i == selected) printf("\x1b[47;30m");
            printf("%s", item_str[_i]);
            for (uint _j = 0; _j < (menu_size.X - strlen(item_str[_i])); _j++) printf("\x20");
            printf("\x1b[0m");
        }
        return 0;
    } else {
        // Print menu with selected
        clear_second_hint();
        show_second_hint("\x1b[47;30m Enter \x1b[0m �������   \x1b[47;30m Up/Down \x1b[0m ����������� �������    \x1b[45;30m Tab \x1b[0m ��������� � �������    ");

        while (1) {
            SCP(item_pos[0]);
            if (selected == 0) printf("\x1b[47;30m");
            printf("%s: %s", item_str[0], ((_sort_direction) ? ("�� �����������") : ("�� ��������")));
            for (uint _i = 0; _i < (menu_size.X - strlen(item_str[0]) - strlen((_sort_direction) ? ("�� �����������") : ("�� ��������")) - 2); _i++) printf("\x20");
            printf("\x1b[0m");
            for (int _i = 1; _i < 8; _i++) {
                SCP(item_pos[_i]);
                if (_i == selected) printf("\x1b[47;30m");
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
                    if (selected > 0) selected--;
                } else if (b == 80) {
                    // ������� ����
                    if (selected < _menu_items - 1) selected++;
                } else if (b == 75 || b == 77) {
                    // ������� ����� ��� ������
                    if (selected == 0) _sort_direction = !_sort_direction;
                }
            } else if (a == 9) {
                // Tab
                SCP(item_pos[selected]);
                printf("%s", item_str[selected]);
                for (uint _j = 0; _j < (menu_size.X - strlen(item_str[selected])); _j++) printf("\x20");
                printf("\x1b[0m");
                return 0;
            } else if (a == 13) {
                // Enter
                if (selected == 0) _sort_direction = !_sort_direction;
                else {
                    sort_list(_lh, selected, _sort_direction);
                    SCP(item_pos[selected]);
                    printf("%s", item_str[selected]);
                    for (uint _j = 0; _j < (menu_size.X - strlen(item_str[selected])); _j++) printf("\x20");
                    printf("\x1b[0m");
                    return 0;
                }
            }
        }
    }
}

/// @brief ������� ��� ���������� ������
/// @param _lh ��������� �� ������ ������
/// @param _field ����� ����, �� �������� ����������� (1 � ������, ... , 7 � ����� ������������� �����)
/// @param _direction ����������� ���������� (1 � �� �����������, 0 � �� ��������)
void sort_list(list_header* _lh, char _field, char _direction) {
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
void clear_menu() {
    COORD local_pos = menu_pos;
    for (uint _i = 0; _i < menu_size.Y; _i++, local_pos.Y++) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), local_pos);
        for (uint _j = 0; _j < menu_size.X; _j++) printf("\x20");
    }
}

/// @brief ������� ��� ������� ������� � �����������
void clear_hint() {
    COORD local_pos = hint_pos;
    for (uint _i = 0; _i < 2; _i++, local_pos.Y++) {
        SCP(local_pos);
        for (uint _j = 0; _j < hint_size.X; _j++) printf("\x20");
    }
}

/// @brief ������� ��� ������� ������ ������ ������� ���������
void clear_second_hint() {
    COORD local_pos = hint_pos;
    local_pos.Y++;
    SCP(local_pos);
    for (uint _j = 0; _j < hint_size.X; _j++) printf("\x20");
}

/// @brief �������, ����������� ��������
/// @return 1 ��� ����������� ������, 0 ��� ���������� ��������� (-1, ���� ����������� �� ����������)
int make_action() {
    return -1;
}












































