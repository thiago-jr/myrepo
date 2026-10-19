#include <stdio.h>

int main() {
    int numeros[30];
    int frecuencia[6] = {0};
    int i;

    printf("30 numeros del 1 al 5:\n");
    for (i = 0; i < 30; i++) {
        scanf("%d", &numeros[i]);
        frecuencia[numeros[i]]++;
    }

    printf("Numeros ingresados:\n");
    for (i = 0; i < 30; i++) {
        printf("%d ", numeros[i]);
    }

    printf("Frecuencia:\n");
    for (i = 1; i <= 5; i++) {
        printf("%d -> %d\n", i, frecuencia[i]);
    }

    return 0;
}