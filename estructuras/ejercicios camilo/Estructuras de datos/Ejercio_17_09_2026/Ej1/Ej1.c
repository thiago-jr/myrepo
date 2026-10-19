/* 1. Escriba un programa en C que mantenga una lista enlazada siempre ordenada de menor a mayor. 
Se recibe como entrada una lista de números en punto 
flotante en precisión simple que finaliza con la entrada del carácter #. 
La inserción de los elementos en la lista se hace a través de la función orderedInsertion. */

#include <stdio.h>
#include <stdlib.h>

struct Nodo {
    float dato;
    struct Nodo *siguiente;
};

// Una cabeza global por tiempo
struct Nodo *cabeza = NULL;

void orderedInsertion(float numero) {
    struct Nodo *nuevo = malloc(sizeof(struct Nodo));

    nuevo->dato = numero;
    nuevo->siguiente = NULL;

    if (cabeza == NULL || numero < cabeza->dato) {
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
    else {
        struct Nodo *actual = cabeza;

        while (actual->siguiente != NULL &&
               actual->siguiente->dato < numero) {
            actual = actual->siguiente;
        }

        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }
}

void mostrarLista() {
    struct Nodo *actual = cabeza;

    while (actual != NULL) {
        printf("%f -> ", actual->dato);
        actual = actual->siguiente;
    }

    printf("NULL\n");
}

int main() {
    char entrada[50];

    printf("Numeros (escribir # para terminar):\n");

    while (scanf("%49s", entrada) == 1) {

        if (entrada[0] == '#') {
            break;
        }

        orderedInsertion(atof(entrada));
    }

    printf("\nlista ordenada de menor a mayor es:\n");

    mostrarLista();

    return 0;
}