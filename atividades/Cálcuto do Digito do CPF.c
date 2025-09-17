/*

Cálculo do dígito do CPF

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char input[20];
    printf("Digite o CPF (com ou sem -): ");
    fgets(input, 20, stdin);
    input[strcspn(input, "\n")] = 0;

    char cpf[12] = {0};
    int idx = 0;
    for (int i = 0; input[i]; i++) {
        if (isdigit(input[i])) {
            cpf[idx++] = input[i];
            if (idx == 11) break;
        }
    }

    if (idx != 11) {
        printf("CPF invalido.\n");
        return 1;
    }

    int digits[11];
    for (int i = 0; i < 11; i++) {
        digits[i] = cpf[i] - '0';
    }

    int sum1 = 0;
    for (int i = 0; i < 9; i++) {
        sum1 += digits[i] * (10 - i);
    }
    int dac1 = 11 - (sum1 % 11);
    if (dac1 >= 10) dac1 = 0;

    int sum2 = 0;
    for (int i = 0; i < 10; i++) {
        sum2 += digits[i] * (11 - i);
    }
    int dac2 = 11 - (sum2 % 11);
    if (dac2 >= 10) dac2 = 0;

    if (dac1 == digits[9] && dac2 == digits[10]) {
        printf("CPF valido.\n");
    } else {
        printf("CPF invalido.\n");
    }

    printf("DAC1 calculado: %d\n", dac1);
    printf("DAC2 calculado: %d\n", dac2);

    return 0;
}
