#include <stdio.h>
#include <string.h>
#include "auth.h"

#define PASSWORD "admin123"
#define MAX_ATTEMPTS 3

int authenticate(void) {
    char input[100];
    int attempts = 0;
    
    printf("=== АУТЕНТИФИКАЦИЯ ===\n");
    printf("Пароль по умолчанию: admin123\n\n");
    
    while (attempts < MAX_ATTEMPTS) {
        printf("Введите пароль: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, PASSWORD) == 0) {
            printf("\n✅ Доступ разрешен!\n\n");
            return 1;
        }
        
        attempts++;
        printf("❌ Неверный пароль. Осталось попыток: %d\n\n", MAX_ATTEMPTS - attempts);
    }
    
    printf("❌ Доступ запрещен!\n");
    return 0;
}
