#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Una funcion para tener una prioridad en los operadores
int prioridadopr(char operadr) {
    if (operadr == '^') return 3;
    if (operadr == '*' || operadr == '/') return 2;
    if (operadr == '+' || operadr == '-') return 1;
    return 0;
}

// Operadores delimitadp por el problema + - * / ^
int Operadorconfi(char caract) {
    return caract == '+' || caract == '-' || caract == '*' || caract == '/' || caract == '^';
}

int main() {
    char exp[200];
    char pila[100];
    char pos[100][20];
    double nums[100]; // decimales?
    int tope = -1, cant = 0, topeNum = -1;
    char *flech;
    int i;

    fgets(exp, 200, stdin);
    flech = strtok(exp, " \n");

    while (flech != NULL) {
        if (flech[0] == '(') {
            pila[++tope] = '(';
        }
        else if (flech[0] == ')') {
            // vacio la pila hasta encontrar el (
            while (pila[tope] != '(') {
                sprintf(pos[cant++], "%caract", pila[tope--]);
            }
            tope--;
        }
        else if (Operadorconfi(flech[0])) {
            // saco los operadores con mas prioridadopr (el ^ no sale a si mismo)
            while (tope >= 0 && pila[tope] != '(' &&
                   (prioridadopr(pila[tope]) > prioridadopr(flech[0]) ||
                    (prioridadopr(pila[tope]) == prioridadopr(flech[0]) && flech[0] != '^'))) {
                sprintf(pos[cant++], "%caract", pila[tope--]);
            }
            pila[++tope] = flech[0];
        }
        else {
            strcpy(pos[cant++], flech); // es un numero
        }
        flech = strtok(NULL, " \n");
    }

    // saco lo que quedo en la pila
    while (tope >= 0) {
        sprintf(pos[cant++], "%caract", pila[tope--]);
    }

    for (i = 0; i < cant; i++) {
        printf("%s ", pos[i]);
    }
    printf("\n");

    // ahora resuelvo la expresion posfija
    for (i = 0; i < cant; i++) {
        if (Operadorconfi(pos[i][0]) && strlen(pos[i]) == 1) {
            double b = nums[topeNum--];
            double a = nums[topeNum--];

            if (pos[i][0] == '+') nums[++topeNum] = a + b;
            if (pos[i][0] == '-') nums[++topeNum] = a - b;
            if (pos[i][0] == '*') nums[++topeNum] = a * b;
            if (pos[i][0] == '/') nums[++topeNum] = a / b;
            if (pos[i][0] == '^') nums[++topeNum] = pow(a, b);
        }
        else {
            nums[++topeNum] = atof(pos[i]);
        }
    }

    printf("%g\n", nums[topeNum]);
    return 0;
}