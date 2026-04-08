#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "menu.h"

int authenticate_with_role(Session *session) {
    char username[50];
    char password[50];
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;
    
    printf("=== АУТЕНТИФИКАЦИЯ ===\n");
    printf("Логин: admin\n");
    printf("Пароль: admin123\n\n");
    
    while (attempts < MAX_ATTEMPTS) {
        printf("Логин: ");
        scanf("%s", username);
        printf("Пароль: ");
        scanf("%s", password);
        
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
            printf("\nДобро пожаловать, Администратор!\n\n");
            session->role = ROLE_ADMIN;
            session->driver_id = 0;
            return 1;
        }
        
        if (strcmp(username, "ivanov") == 0 && strcmp(password, "pass123") == 0) {
            printf("\nДобро пожаловать, водитель!\n\n");
            session->role = ROLE_DRIVER;
            session->driver_id = 1;
            return 1;
        }
        
        attempts++;
        printf("Неверный логин или пароль. Осталось попыток: %d\n\n", MAX_ATTEMPTS - attempts);
    }
    
    printf("Доступ запрещен!\n");
    return 0;
}

int main() {
    Session session;
    Database *db;
    int choice;
    
    if (!authenticate_with_role(&session)) {
        return 1;
    }
    
    db = init_database("resources/autopark.db");
    if (!db || !db->db) {
        printf("Ошибка подключения к базе данных\n");
        if (db) close_database(db);
        return 1;
    }
    
    printf("Подключение к БД успешно\n");
    
    do {
        show_main_menu(&session, db);
        printf("\nВыберите действие: ");
        scanf("%d", &choice);
        getchar();
    } while (handle_menu_choice(&session, db, choice) != 0);
    
    close_database(db);
    printf("\nДо свидания!\n");
    return 0;
}
