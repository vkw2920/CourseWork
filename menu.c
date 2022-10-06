#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// #ifndef WINDOW
// #include <ncurses.h>
// #endif

#ifndef PANEL
#include <panel.h>
#endif

#ifndef free
#include <stdlib.h>
#endif

#ifndef uint
#define uint unsigned int
#endif

void test(WINDOW* _screen) {
    box(_screen, 0, 0);
    for (int i = 0; i < 5; i++) {
        char g = getch();
        mvwprintw(_screen, i+1, 1, "Entered: %d", g);
    }
}

int select_from_menu(WINDOW* _screen, const char** _menu_items, char _menu_items_count, char _exit_code) {
    // Высота зависит от количества пунктов
    // Два столбца, каждый пункт по 2 строки
    // Плюс одна строка рамки
    int height = _menu_items_count * 2 + 1;

    // Сделаем связку для изменения цветового оформления выделенного пункта меню
    init_pair(12, COLOR_BLACK, COLOR_WHITE);
    mvwprintw(_screen, 1, 1, "Black = %d, White = %d", COLOR_BLACK, COLOR_WHITE);
    mvwprintw(_screen, 2, 1, "Pair = %d", COLOR_PAIR(12));

    // Сохраним размеры родительского окна
    int scr_w = COLS, scr_h = LINES;
    getmaxyx(_screen, scr_h, scr_w);

    // Минимальная ширина 47 (пункт меню 45 символов + 2 символа границ)
    // Если размеры окна позволяют использовать большее пространство
    // То будем использовать 60% от максимальной ширины
    // Не забудем про выравнивание, чтобы оба столбца были одинаковой ширины
    int width = 47;
    if ((scr_w * 0.6) > width)
        width = (scr_w * 0.6);

    // Сохраним ширину столбца, т.к. она не всегда будет 45 символов
    int col_width = width - 2;

    // И подготовим форматную строку для вывода пунктов меню
    char *format = (char*)calloc(16, sizeof(char));
    sprintf(format, " %c-%d.%ds ", '%', col_width-2, col_width-2);

    // Создадим окно заданных размеров с выравниванием по центру
    // Выравнивание производится указанием координат верхнего левого угла
    // Верхниц левый угол окна консоли является началом координат
    WINDOW* _menu_window = subwin(_screen, height, width, (scr_h - height) / 2, (scr_w - width) / 2);
    PANEL* _menu_panel = new_panel(_menu_window);

    // Нарисуем рамку по границе дочернего окна, чтобы выделить его визуально
    // Как-то нехорошо будет, если менюшка окажется без рамки
    // wborder(_menu_window, '|', '|', '-', '-', '+', '+', '+', '+');
    box(_menu_window, 0, 0);

    // Также стоит разрешить дочернему окну перехватывать нажатия клавиш со стрелками
    keypad(_menu_window, 1);

    // Теперь же реализуем механику самого меню
    int s = 0;  // Переменная с индексом выделенного пункта меню

    // Цикл для вывода меню
    for (char _i = 0; _i < _menu_items_count; _i++) {
        // Если работаем с выделенным пунктом, то следует применить цветовую пару
        if (_i == s) wattron(_menu_window, 3072);

        // Потом вывести наименование пункта
        mvwprintw(_menu_window, (_i * 2) + 1, 1, format, _menu_items[_i]);

        // И убрать выделение, чтобы не закрышивать последующие элементы
        if (_i == s) wattroff(_menu_window, 3072);

        // Также выведем горизонтальную линию после каждого, кроме последнего пункта
        if (_i + 1 < _menu_items_count) mvwhline(_menu_window, (_i * 2) + 2, 1, '-', col_width);
    }
    update_panels();
    doupdate();

    while (1) {
        char a = getch();
        mvwprintw(_screen, 3, 1, "Entered: %d ", a);
        if (a == 2) s++;
        else if (a == 3) s--;
        else if (a == 4) s = 0;
        else if (a == 5) s = _menu_items_count - 1;
        else if (a == 10) return s;
        if (s >= _menu_items_count) s = _menu_items_count - 1;
        else if (s < 0) s = 0;
        mvwprintw(_screen, 4, 1, "Selected: %d ", s);
        
        // Цикл для вывода меню
        for (char _i = 0; _i < _menu_items_count; _i++) {
            // Если работаем с выделенным пунктом, то следует применить цветовую пару
            if (_i == s) wattron(_menu_window, 3072);

            // Потом вывести наименование пункта
            mvwprintw(_menu_window, (_i * 2) + 1, 1, format, _menu_items[_i]);

            // И убрать выделение, чтобы не закрышивать последующие элементы
            if (_i == s) wattroff(_menu_window, 3072);

            // Также выведем горизонтальную линию после каждого, кроме последнего пункта
            if (_i + 1 < _menu_items_count) mvwhline(_menu_window, (_i * 2) + 2, 1, '-', col_width);
        }
        update_panels();
        doupdate();
    }

    // Заморозим менюшку, чтобы увидеть, как выглядит
    getch();

    // Удалим дочернее окно
    delwin(_menu_window);

    // Обновим родительское окно
    wrefresh(_screen);

    // Перерисуем родительское окно
    redrawwin(_screen);

    // Для обновления информации на экране
    doupdate();

    // Вернём код выхода
    return _exit_code;
}

// Функция main предусмотрена для отладки конкретного файла проекта
#ifndef main
int main() {
    initscr();          // init NCurses screen
    start_color();      // Будем работать с цветами
    noecho();           // No print input chars
    keypad(stdscr, 1);  // Allow to retrieve Functional keys
    curs_set(0);        // Hide cursor
    const char* menu[5] = {
        "Create list",
        "Add new student",
        "List all students",
        "Edit info by surname",
        "Exit program"
    };

    // test(stdscr);
    select_from_menu(stdscr, menu, 5, 4);
    getch();
}
#endif