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

void list_apped(list_header* _lh) {
    for (uint _i = 0; _i < 30; _i++) {
        add_student(_lh, new_student("ИС-3", "Контарев", 2004, 1, 5, 4));
        add_student(_lh, new_student("ИС-1", "Калашникова", 2002, 0, 10, 7));
        add_student(_lh, new_student("ИС-3", "Мочалин", 2002, 1, 10, 7));
        add_student(_lh, new_student("ИС-3", "Пышногуб", 2001, 1, 10, 7));
        add_student(_lh, new_student("ИС-3", "Яресько", 2003, 0, 10, 7));
        add_student(_lh, new_student("ИС-2", "Ольховская", 2003, 0, 10, 7));
        add_student(_lh, new_student("ИС-1", "Степанишина", 2003, 0, 10, 7));
        add_student(_lh, new_student("ИС-1", "Кулагина", 2003, 0, 10, 7));
        add_student(_lh, new_student("ИС-3", "Тарароева", 2002, 0, 10, 7));
        add_student(_lh, new_student("ИС-3", "Талан", 2002, 0, 10, 7));
        add_student(_lh, new_student("Арх-1", "Билетников", 2004, 1, 199, 7));
    }
}

int main() {
    // _getch();
    create_layout();
    show_base_hint("\x1b[33mПРЕДУПРЕЖДЕНИЕ\x1b[0m: Использование кириллических символов может привести к ошибкам в работе программы.");
    list_header* lh = new_list();
    if (lh) {
        list_apped(lh);

        show_table(lh);
    }
    return 0;
}
