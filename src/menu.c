[08.04.2026 22:24] одна клетка мозга (и та чужая): #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

void show_admin_menu(void) {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║        ГЛАВНОЕ МЕНЮ (АДМИН)          ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. Показать всех водителей          ║\n");
    printf("║  2. Показать все автомобили          ║\n");
    printf("║  3. Показать все заказы              ║\n");
    printf("║  4. Добавить водителя                ║\n");
    printf("║  5. Добавить автомобиль              ║\n");
    printf("║  6. Добавить заказ                   ║\n");
    printf("║  7. Отчет по водителям (зарплата)    ║\n");
    printf("║  8. Водитель с мин. поездок          ║\n");
    printf("║  9. Авто с макс. пробегом            ║\n");
    printf("║  0. Выход                            ║\n");
    printf("╚══════════════════════════════════════╝\n");
}

void show_driver_menu(void) {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║       ГЛАВНОЕ МЕНЮ (ВОДИТЕЛЬ)        ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. Мои заказы                       ║\n");
    printf("║  2. Моя статистика                   ║\n");
    printf("║  3. Моя зарплата за период           ║\n");
    printf("║  0. Выход                            ║\n");
    printf("╚══════════════════════════════════════╝\n");
}

void show_main_menu(Session *session, Database *db) {
    (void)db;
    if (session->role == ROLE_ADMIN) {
        show_admin_menu();
    } else {
        show_driver_menu();
    }
}

void input_car_form(Car *car) {
    printf("\n--- ДОБАВЛЕНИЕ АВТОМОБИЛЯ ---\n");
    printf("Госномер: ");
    scanf("%s", car->number);
    printf("Марка: ");
    scanf("%s", car->brand);
    printf("Пробег при покупке (км): ");
    scanf("%lf", &car->initial_mileage);
    printf("Грузоподъемность (тонн): ");
    scanf("%lf", &car->capacity);
    car->id = 0;
}

void input_driver_form(Driver *driver) {
    printf("\n--- ДОБАВЛЕНИЕ ВОДИТЕЛЯ ---\n");
    printf("Табельный номер: ");
    scanf("%d", &driver->personal_id);
    printf("ФИО: ");
    scanf(" %[^\n]", driver->full_name);
    printf("Категория (A,B,C,D,E): ");
    scanf("%s", driver->category);
    printf("Стаж (лет): ");
    scanf("%d", &driver->experience);
    printf("Адрес: ");
    scanf(" %[^\n]", driver->address);
    printf("Год рождения: ");
    scanf("%d", &driver->birth_year);
    driver->id = 0;
}

void input_order_form(Order *order) {
    printf("\n--- ДОБАВЛЕНИЕ ЗАКАЗА ---\n");
    printf("Дата (ГГГГ-ММ-ДД): ");
    scanf("%s", order->date);
    printf("ID водителя: ");
    scanf("%d", &order->driver_id);
    printf("ID автомобиля: ");
    scanf("%d", &order->car_id);
    printf("Километраж (км): ");
    scanf("%lf", &order->distance);
    printf("Масса груза (тонн): ");
    scanf("%lf", &order->cargo_weight);
    printf("Стоимость перевозки (руб): ");
    scanf("%lf", &order->total_cost);
    order->id = 0;
}
[08.04.2026 22:24] одна клетка мозга (и та чужая): int handle_menu_choice(Session *session, Database *db, int choice) {
    Car car;
    Driver driver;
    Order order;
    char start[11], end[11];
    int id;
    
    if (session->role == ROLE_ADMIN) {
        switch (choice) {
            case 1:
                get_all_drivers(db);
                break;
            case 2:
                get_all_cars(db);
                break;
            case 3:
                get_all_orders(db);
                break;
            case 4:
                input_driver_form(&driver);
                add_driver(db, &driver);
                break;
            case 5:
                input_car_form(&car);
                add_car(db, &car);
                break;
            case 6:
                input_order_form(&order);
                add_order(db, &order);
                break;
            case 7:
                report_all_drivers_summary(db);
                break;
            case 8:
                find_min_trips_driver(db);
                break;
            case 9:
                find_max_mileage_car(db);
                break;
            case 0:
                return 0;
            default:
                printf("Неверный выбор!\n");
        }
    } else {
        switch (choice) {
            case 1:
                printf("\n--- МОИ ЗАКАЗЫ ---\n");
                {
                    char sql[256];
                    snprintf(sql, sizeof(sql), 
                        "SELECT * FROM orders WHERE drivers_id = %d;", session->driver_id);
                    execute_query(db, sql);
                }
                break;
            case 2:
                printf("\n--- МОЯ СТАТИСТИКА ---\n");
                {
                    char sql[256];
                    snprintf(sql, sizeof(sql),
                        "SELECT COUNT(*) as trips, SUM(cargo_mass) as cargo, SUM(transport_cost) * 0.2 as earnings "
                        "FROM orders WHERE drivers_id = %d;", session->driver_id);
                    execute_query(db, sql);
                }
                break;
            case 3:
                printf("Введите начальную дату (ГГГГ-ММ-ДД): ");
                scanf("%s", start);
                printf("Введите конечную дату (ГГГГ-ММ-ДД): ");
                scanf("%s", end);
                calc_driver_earnings_period(db, session->driver_id, start, end);
                break;
            case 0:
                return 0;
            default:
                printf("Неверный выбор!\n");
        }
    }
    return 1;
}
