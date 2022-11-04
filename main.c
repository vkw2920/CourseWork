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

const char* groups[10] = {
    "ИС-1",
    "ИС-2",
    "ИС-3",
    "ПИ-1",
    "Арх-1",
    "ФКиС-1",
    "ПОН-2",
    "ПОД-1",
    "Фил-1",
    "ИБ-1",
};

const char* surnames[30] = {
    "Контарев",
    "Калашникова",
    "Мочалин",
    "Пышногуб",
    "Яресько",
    "Ольховская",
    "Степанишина",
    "Семёнов",
    "Кулагина",
    "Тарароева",
    "Талан",
    "Билетников",
    "Шаврин",
    "Архипов",
    "Зайцев",
    "Силаев",
    "Аракелян",
    "Дикун",
    "Остапович",
    "Звада",
    "Кабалинов",
    "Куприенко",
    "Константинов",
    "Долгодуш",
    "Шашенко",
    "Шевченко",
    "Шарафан",
    "Замяткин",
    "Ульянов",
    "Сахаровская",
};

void list_apped(list_header* _lh) {
    for (uint _i = 0; _i < 1000; _i++) {
        uint skips = rand()%999999999;
        add_student(_lh, new_student(groups[rand()%10], surnames[rand()%30], 2000+(rand()%20), rand()%2, skips, rand()%skips));
    }
}

int main() {
    // _getch();
    create_layout();
    list_header* lh = new_list();
    if (lh) {
        list_apped(lh);

        show_table(lh);
    }
    return 0;
}
