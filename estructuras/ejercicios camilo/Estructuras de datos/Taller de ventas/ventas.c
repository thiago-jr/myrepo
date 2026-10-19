/*
  ventas.c
  Lee ventas.csv (filas = productos, columnas = meses, separado por ;)
  y arma un cubo cubo[año][trimestre][producto] para responder
  4 preguntas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 10000

int main() {

    FILE *archivo;
    char linea[TAM];
    int productos = 0;
    int meses = 0;
    int anos;
    int i, j, a, t, k, p;

    archivo = fopen("ventas.csv", "r");

    /*contar las filas (productos) */
    while (fgets(linea, TAM, archivo) != NULL) {
        if (linea[0] != '\n' && linea[0] != '\r') {
            productos++;
        }
    }

    /*contar columnas en la primera linea */
    rewind(archivo);
    fgets(linea, TAM, archivo);
    for (i = 0; linea[i] != '\0'; i++) {
        if (linea[i] == ';') {
            meses++;
        }
    }
    meses++;   /* la ultima columna no tiene ; */

    anos = meses / 12;
    if (meses % 12 != 0) {
        anos++;
    }

    printf("Productos: %d\n", productos);
    printf("Meses: %d\n", meses);
    printf("Años: %d\n\n", anos);

    /* matriz[producto][mes] con memo dinamica */
    int **matriz = (int **) malloc(productos * sizeof(int *));
    for (i = 0; i < productos; i++) {
        matriz[i] = (int *) calloc(meses, sizeof(int));
    }

    /* tercera pasada: llenar la matriz */
    rewind(archivo);
    i = 0;
    while (fgets(linea, TAM, archivo) != NULL && i < productos) {
        if (linea[0] == '\n' || linea[0] == '\r') {
            continue;
        }

        char *dato = strtok(linea, ";");
        j = 0;
        while (dato != NULL && j < meses) {
            matriz[i][j] = atoi(dato);
            j++;
            dato = strtok(NULL, ";");
        }
        i++;
    }
    fclose(archivo);

    /* cubo[año][trimestre][producto]
       cada trimestre suma sus 3 meses */
    int ***cubo = (int ***) malloc(anos * sizeof(int **));
    for (a = 0; a < anos; a++) {
        cubo[a] = (int **) malloc(4 * sizeof(int *));
        for (t = 0; t < 4; t++) {
            cubo[a][t] = (int *) calloc(productos, sizeof(int));
        }
    }

    for (a = 0; a < anos; a++) {
        for (t = 0; t < 4; t++) {
            for (k = 0; k < 3; k++) {
                int col = a * 12 + t * 3 + k;   /* mes dentro de la matriz */
                for (p = 0; p < productos; p++) {
                    if (col < meses) {
                        cubo[a][t][p] = cubo[a][t][p] + matriz[p][col];
                    }
                }
            }
        }
    }

    /* 1. producto mas vendido en cada año y trimestre */
    printf("1. Producto mas vendido por año y trimestre\n");
    for (a = 0; a < anos; a++) {
        for (t = 0; t < 4; t++) {
            int mayor = cubo[a][t][0];
            int prod = 0;
            for (p = 1; p < productos; p++) {
                if (cubo[a][t][p] > mayor) {
                    mayor = cubo[a][t][p];
                    prod = p;
                }
            }
            printf("Año %d, trimestre %d: producto %d con %d unidades\n",
                   a + 1, t + 1, prod + 1, mayor);
        }
    }
    printf("\n");

    /* 2. año y trimestre con mayor promedio de venta por producto */
    float mejorProm = 0;
    int mejorA = 0, mejorT = 0;
    for (a = 0; a < anos; a++) {
        for (t = 0; t < 4; t++) {
            int suma = 0;
            for (p = 0; p < productos; p++) {
                suma = suma + cubo[a][t][p];
            }
            float prom = (float) suma / productos;
            if (prom > mejorProm) {
                mejorProm = prom;
                mejorA = a;
                mejorT = t;
            }
        }
    }
    printf("2. Mayor promedio de venta por producto\n");
    printf("Año %d, trimestre %d, promedio %.2f\n\n",
           mejorA + 1, mejorT + 1, mejorProm);

    /* 3. año con mayor volumen de ventas */
    int *total = (int *) calloc(anos, sizeof(int));
    for (a = 0; a < anos; a++) {
        for (t = 0; t < 4; t++) {
            for (p = 0; p < productos; p++) {
                total[a] = total[a] + cubo[a][t][p];
            }
        }
    }
    int mejorAnio = 0;
    for (a = 1; a < anos; a++) {
        if (total[a] > total[mejorAnio]) {
            mejorAnio = a;
        }
    }
    printf("3. Año con mayor volumen de ventas\n");
    printf("Año %d con %d unidades\n\n", mejorAnio + 1, total[mejorAnio]);

    /* 4. año de mayor venta del producto n en el mes m */
    int n, m;
    printf("4. Mayor venta de un producto en un mes\n");
    printf("Producto (1 a %d): ", productos);
    scanf("%d", &n);
    printf("Mes (1 a 12): ");
    scanf("%d", &m);

    if (n < 1 || n > productos || m < 1 || m > 12) {
        printf("Datos fuera de rango\n");
    } else {
        int trim = (m - 1) / 3;   /* meses 1,2,3 son trimestre 0 */
        int mejor = 0;
        for (a = 1; a < anos; a++) {
            if (cubo[a][trim][n - 1] > cubo[mejor][trim][n - 1]) {
                mejor = a;
            }
        }
        printf("El producto %d en el mes %d (trimestre %d) vendio mas en el año %d\n",
               n, m, trim + 1, mejor + 1);
        printf("Unidades del trimestre: %d\n", cubo[mejor][trim][n - 1]);
    }

    /* liberar memoria */
    for (i = 0; i < productos; i++) {
        free(matriz[i]);
    }
    free(matriz);
    free(total);
    for (a = 0; a < anos; a++) {
        for (t = 0; t < 4; t++) {
            free(cubo[a][t]);
        }
        free(cubo[a]);
    }
    free(cubo);

    return 0;
}