#include <stdio.h>
#include <assert.h>
#include <string.h>

int validate_password(const char *input) {
    const char *correct = "admin123";
    return strcmp(input, correct) == 0;
}

void test_valid_password() {
    assert(validate_password("admin123") == 1);
    printf("✅ test_valid_password passed\n");
}

void test_invalid_password() {
    assert(validate_password("wrong") == 0);
    printf("✅ test_invalid_password passed\n");
}

void test_empty_password() {
    assert(validate_password("") == 0);
    printf("✅ test_empty_password passed\n");
}

int main() {
    printf("\n=== TESTING MAIN ===\n");
    test_valid_password();
    test_invalid_password();
    test_empty_password();
    printf("All main tests passed!\n");
    return 0;
}