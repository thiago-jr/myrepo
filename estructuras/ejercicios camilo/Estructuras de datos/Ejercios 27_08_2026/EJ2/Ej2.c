#include <stdio.h>

int main() {
    int numeros[20];
    int i, j, contador;

    for (i = 0; i < 20; i++) {
        printf("Ingrese un numero: ");
        scanf("%d", &numeros[i]);
    }

    printf("\nUNA sola ocurrencia:\n");

    for (i = 0; i < 20; i++) {
        contador = 0; // Contador que reinicia para cada número
        
        for (j = 0; j < 20; j++) {
            if (numeros[i] == numeros[j]) {
                contador++; 
            }
        }

        
        if (contador == 1) {
            printf("%d ", numeros[i]);
        }
    }
    
    printf("\n");

    return 0;
}