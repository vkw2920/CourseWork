#ifndef uchar
#define uchar unsigned char
#endif

// Функция для открытия окна для выбора действия с использованием стрелок
char select_from_menu(
    const char** _menu_items,  // Массив строк. Наименования пунктов меню
    const char _menu_items_count,    // Количество пунктов меню
    char _exit_code            // Код, который надо вернуть для выхода из программы
);

int test_input();

void create_background();

int* get_size();
