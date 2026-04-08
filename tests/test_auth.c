#include <stdio.h>
#include <string.h>
#include <assert.h>


int mock_authenticate(const char *password) {
    const char *correct = "admin123";
    if (strcmp(password, correct) == 0) {
        return 1;
    }
    return 0;
}

void test_correct_password() {
    assert(mock_authenticate("admin123") == 1);
    printf("✅ test_correct_password passed\n");
}

void test_wrong_password() {
    assert(mock_authenticate("wrong") == 0);
    printf("✅ test_wrong_password passed\n");
}

void test_empty_password() {
    assert(mock_authenticate("") == 0);
    printf("✅ test_empty_password passed\n");
}

int main() {
    printf("\n=== TESTING AUTH ===\n");
    test_correct_password();
    test_wrong_password();
    test_empty_password();
    printf("All auth tests passed!\n");
    return 0;
}