#include <stdio.h>

int main() {
    float numeros[10];
    float temporal;
    int i, j;

    for (i = 0; i < 10; i++) {
        printf("Numero: ");
        scanf("%f", &numeros[i]);
    }

    for (i = 0; i < 9; i++) {
        for (j = i + 1; j < 10; j++) {
            //Actual mayor que el siguiente, intercambiar
            if (numeros[i] > numeros[j]) {
                temporal = numeros[i];
                numeros[i] = numeros[j];
                numeros[j] = temporal;
            }
        }
    }

    //Ordenado, [0] es el menor y [9] es el mayor.
    //[1] es el segundo menor y [8] el segundo mayor.
    printf("\nSegundo menor es: %.2f\n", numeros[1]);
    printf("Segundo mayor es: %.2f\n", numeros[8]);

    return 0;
}