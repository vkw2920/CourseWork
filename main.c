#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#ifndef uint
#define uint unsigned int
#endif

#ifndef student
#include "functions.h"
#endif

#define menu_items_count 5

int main() {
    // _getch();
    create_layout();
    show_base_hint("\x1b[33m��������������\x1b[0m: ������������� ������������� �������� ����� �������� � ������� � ������ ���������.");
    list_header* lh = new_list();
    if (lh) {
        for (uint _i = 0; _i < 10; _i++) {
            add_student(lh, new_student("��-3", "��������", 2004, 1, 5, 4));
            add_student(lh, new_student("��-1", "�����������", 2002, 0, 10, 7));
            add_student(lh, new_student("��-3", "�������", 2002, 1, 10, 7));
            add_student(lh, new_student("��-3", "��������", 2001, 1, 10, 7));
            add_student(lh, new_student("��-3", "�������", 2003, 0, 10, 7));
            add_student(lh, new_student("��-2", "����������", 2003, 0, 10, 7));
            add_student(lh, new_student("��-1", "�����������", 2003, 0, 10, 7));
            add_student(lh, new_student("��-1", "��������", 2003, 0, 10, 7));
            add_student(lh, new_student("��-3", "���������", 2002, 0, 10, 7));
            add_student(lh, new_student("��-3", "�����", 2002, 0, 10, 7));
            add_student(lh, new_student("���-1", "����������", 2004, 1, 199, 7));
        }

        show_table(lh);
    }
    return 0;

    // const char* menu[menu_items_count] = {
    //     "������ ������",
    //     "� ���������",
    //     "�����"
    // };

    // list_header* lst = NULL;
    // char selected = 0;

    // while ((selected = select_from_menu(menu, menu_items_count, menu_items_count - 1)) != menu_items_count - 1) {
    //     switch (selected)
    //     {
    //     case 0:
    //         lst = new_list();
    //         break;

    //     default:
    //         // wclear(stdscr);
    //         getchar();
    //     }
    // }

    // return 0;
}