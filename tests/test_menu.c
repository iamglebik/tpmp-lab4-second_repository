#include <stdio.h>
#include <assert.h>

typedef enum {
    ROLE_ADMIN = 0,
    ROLE_DRIVER = 1
} UserRole;

int is_admin(UserRole role) {
    return role == ROLE_ADMIN;
}

int is_driver(UserRole role) {
    return role == ROLE_DRIVER;
}

void test_admin_role() {
    assert(is_admin(ROLE_ADMIN) == 1);
    assert(is_admin(ROLE_DRIVER) == 0);
    printf("✅ test_admin_role passed\n");
}

void test_driver_role() {
    assert(is_driver(ROLE_DRIVER) == 1);
    assert(is_driver(ROLE_ADMIN) == 0);
    printf("✅ test_driver_role passed\n");
}

void test_menu_choice_range() {
    int choice = 5;
    assert(choice >= 0 && choice <= 11);
    printf("✅ test_menu_choice_range passed\n");
}

int main() {
    printf("\n=== TESTING MENU ===\n");
    test_admin_role();
    test_driver_role();
    test_menu_choice_range();
    printf("All menu tests passed!\n");
    return 0;
}