#include <stdio.h>

int main() {
    int n, i, j;
    char cadenas[10][21];

    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        scanf("%s", cadenas[i]); 
    }

    //j avanza letra por letra sobre la primera palabra
    for (j = 0; cadenas[0][j] != '\0'; j++) {
        int coinciden = 1; 
        
        //i revisa esa misma letra en las demás palabras
        for (i = 1; i < n; i++) {
            if (cadenas[i][j] != cadenas[0][j]) {
                coinciden = 0; 
                break; // SOLO SI hay una diferencia, dejamos de revisar esta letra
            }
        }

        // Si no coinciden, cortamos la búsqueda principal
        if (coinciden == 0) {
            break; 
        }
    }

    if (j == 0) {
        printf("-\n");
    } else {
        cadenas[0][j] = '\0';
        printf("%s\n", cadenas[0]);
    }

    return 0;
}