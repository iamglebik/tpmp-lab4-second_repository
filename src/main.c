#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "auth.h"
#include "menu.h"

int main() {
    
    if (!authenticate()) {
        return 1;
    }
    
    Database *db = init_database("resources/autopark.db");
    if (!db || !db->db) {
        printf("Ошибка подключения к базе данных\n");
        if (db) close_database(db);
        return 1;
    }
    
    printf("Подключение к БД успешно\n");
    
    int choice;
    do {
        show_main_menu();
        printf("\nВыберите действие: ");
        scanf("%d", &choice);
        getchar();
    } while (handle_menu_choice(db, choice) != 0);
    
    close_database(db);
    printf("\nДо свидания!\n");
    return 0;
}
