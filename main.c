#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <panel.h>
#include <stdlib.h>

#ifndef uint
#define uint unsigned int
#endif

#if !defined(student_info) || !defined(student_item) || !defined(list_header)
#include "types.h"
#endif

#ifndef select_from_menu
#include "menu.h"
#endif

int main() {
    initscr();          // init NCurses screen
    start_color();      // Будем работать с цветами
    noecho();           // No print input chars
    keypad(stdscr, 1);  // Allow to retrieve Functional keys
    curs_set(0);        // Hide cursor

    border('|', '|', '-', '-', '+', '+', '+', '+');

    const char* menu[5] = {
        "Create list",
        "Add new student",
        "List all students",
        "Edit info by surname",
        "Exit program"
    };

    select_from_menu(stdscr, menu, 5, 4);

    getch();
    
    endwin();
    return 0;
}