#include <stdio.h>
#include "menu.h"

void show_main_menu(void) {
    printf("\n=== ГЛАВНОЕ МЕНЮ ===\n");
    printf("1. Показать всех водителей\n");
    printf("2. Показать все автомобили\n");
    printf("3. Показать все заказы\n");
    printf("4. Добавить водителя\n");
    printf("5. Добавить автомобиль\n");
    printf("6. Добавить заказ\n");
    printf("0. Выход\n");
}

int handle_menu_choice(Database *db, int choice) {
    switch (choice) {
        case 1:
            printf("\n--- СПИСОК ВОДИТЕЛЕЙ ---\n");
            execute_query(db, "SELECT id, personnel_number, last_name, category, experience, address, birth_year FROM drivers;");
            break;
        case 2:
            printf("\n--- СПИСОК АВТОМОБИЛЕЙ ---\n");
            execute_query(db, "SELECT id, car_number, brand, mileage_on_purchase, capacity FROM cars;");
            break;
        case 3:
            printf("\n--- СПИСОК ЗАКАЗОВ ---\n");
            execute_query(db, "SELECT id, order_date, drivers_id, cars_id, mileage, cargo_mass, transport_cost FROM orders;");
            break;
        case 4:
            printf("Функция добавления водителя в разработке\n");
            break;
        case 5:
            printf("Функция добавления автомобиля в разработке\n");
            break;
        case 6:
            printf("Функция добавления заказа в разработке\n");
            break;
        case 0:
            return 0;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
    }
    return 1;
}
