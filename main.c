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

#ifndef select_from_menu
#include "./menu.h"
#endif

#ifndef student
#include "./functions.h"
#endif

#define menu_items_count 5

int main() {
    create_layout();
    show_base_hint();
    // update_menu();
    student* tmp = (student*)calloc(1, sizeof(student));
    tmp = enter_data(tmp);
    _getch();
    return 0;

    const char* menu[menu_items_count] = {
        "Начало работы",
        "О программе",
        "Выйти"
    };

    list_header* lst = NULL;
    char selected = 0;

    while ((selected = select_from_menu(menu, menu_items_count, menu_items_count - 1)) != menu_items_count - 1) {
        switch (selected)
        {
        case 0:
            lst = new_list();
            break;
        
        default:
            // wclear(stdscr);
            getchar();
        }
    }

    return 0;
}