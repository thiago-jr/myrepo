#include <stdio.h>
#include <string.h>

void generarSubcadenas(char cadena[], int n) {
    printf("--- SUBCADENAS (Contiguas) ---\n");
    
    
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            
            for (int k = i; k <= j; k++) {
                putchar(cadena[k]);
            }
            printf("\n");
        }
    }
}

void generarSubsecuencias(char cadena[], int n) {
    printf("\n--- SUBSECUENCIAS (Con o sin saltos) ---\n");
    
    int total = 1 << n;

    for (int i = 0; i < total; i++) {
        
        for (int j = 0; j < n; j++) {
            
            if (i & (1 << j)) {
                putchar(cadena[j]);
            }
        }
        printf("\n");
    }
}

int main() {
    char cadena[20];

    printf("Ingresa una cadena de texto (sin espacios): ");
    scanf(" %19s", cadena);

    int n = strlen(cadena);

    generarSubcadenas(cadena, n);
    generarSubsecuencias(cadena, n);

    return 0;
}