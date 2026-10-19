#include <stdio.h>

int main() {
    char caracteres[10];
    int frecuencias[256] = {0}; // Arreglo de 256 posiciones lleno de ceros
    int i, j, max_frec, indice_max;

    for (i = 0; i < 10; i++) {
        printf("Ingrese un caracter: ");
        scanf(" %c", &caracteres[i]); 
        
        frecuencias[(int)caracteres[i]]++; 
    }

    printf("\nLos 3 caracteres con mayor frecuencia son:\n");

    for (i = 0; i < 3; i++) {
        max_frec = 0;
        indice_max = 0;

        for (j = 0; j < 256; j++) {
            if (frecuencias[j] > max_frec) {
                max_frec = frecuencias[j];
                indice_max = j;
            }
        }

        if (max_frec > 0) {
            printf("%d. El caracter '%c' aparecio %d veces\n", i + 1, indice_max, max_frec);

            frecuencias[indice_max] = 0; 
        }
    }

    return 0;
}