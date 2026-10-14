#include <stdio.h>
#include <stdlib.h>

/* ===================== TAD Cola dinámica ===================== */
typedef struct NodoC {
    int dato;
    struct NodoC *sig;
} NodoC;

struct Cola {
    NodoC *frente;
    NodoC *fin;
    int tam;
};
typedef struct Cola *cola;

cola crearCola(void) {
    cola c = malloc(sizeof(struct Cola));
    if (!c) exit(EXIT_FAILURE);
    c->frente = c->fin = NULL;
    c->tam = 0;
    return c;
}

int isEmptyCola(cola c) { return c->tam == 0; }
int obtenerTamano(cola c) { return c->tam; }

void enqueue(cola c, int x) {
    NodoC *n = malloc(sizeof(NodoC));
    if (!n) exit(EXIT_FAILURE);
    n->dato = x;
    n->sig = NULL;
    if (c->fin) c->fin->sig = n;
    else c->frente = n;
    c->fin = n;
    c->tam++;
}

int dequeue(cola c) {               /* precondición: cola no vacía */
    NodoC *n = c->frente;
    int x = n->dato;
    c->frente = n->sig;
    if (!c->frente) c->fin = NULL;
    free(n);
    c->tam--;
    return x;
}

void destruirCola(cola c) {
    while (!isEmptyCola(c)) dequeue(c);
    free(c);
}

/* ===================== TAD Pila dinámica ===================== */
typedef struct NodoP {
    int dato;
    struct NodoP *sig;
} NodoP;

struct Pila {
    NodoP *tope;
    int tam;
};
typedef struct Pila *pila;

pila crearPila(void) {
    pila p = malloc(sizeof(struct Pila));
    if (!p) exit(EXIT_FAILURE);
    p->tope = NULL;
    p->tam = 0;
    return p;
}

int isEmptyPila(pila p) { return p->tam == 0; }

void push(pila p, int x) {
    NodoP *n = malloc(sizeof(NodoP));
    if (!n) exit(EXIT_FAILURE);
    n->dato = x;
    n->sig = p->tope;
    p->tope = n;
    p->tam++;
}

int pop(pila p) {                   /* precondición: pila no vacía */
    NodoP *n = p->tope;
    int x = n->dato;
    p->tope = n->sig;
    free(n);
    p->tam--;
    return x;
}

void destruirPila(pila p) {
    while (!isEmptyPila(p)) pop(p);
    free(p);
}

/* ===================== Función solicitada ===================== */
/* Imprime la ventana del más reciente al más antiguo sin alterar la cola. */
void imprimirVentanaInvertida(cola c) {
    pila invertida = crearPila();   /* tope = elemento más reciente */
    pila restaurar = crearPila();   /* tope = elemento más antiguo  */
    int primero = 1;

    /* 1) Cola -> Pila: el último en entrar queda arriba */
    while (!isEmptyCola(c))
        push(invertida, dequeue(c));

    /* 2) Imprimir (LIFO) y guardar en la pila de restauración */
    while (!isEmptyPila(invertida)) {
        int x = pop(invertida);
        if (!primero) printf(" ");
        printf("%d", x);
        primero = 0;
        push(restaurar, x);
    }

    /* 3) Restaurar la cola en su orden original (antiguo -> reciente) */
    while (!isEmptyPila(restaurar))
        enqueue(c, pop(restaurar));

    destruirPila(invertida);
    destruirPila(restaurar);
}

/* ============================ main ============================ */
int main(void) {
    int K, x;

    if (scanf("%d", &K) != 1 || K <= 1) {
        fprintf(stderr, "K debe ser un entero mayor que 1.\n");
        return 1;
    }

    cola ventana = crearCola();

    while (scanf("%d", &x) == 1 && x != -1) {
        if (obtenerTamano(ventana) == K)
            dequeue(ventana);
        enqueue(ventana, x);

        printf("%d -> ", x);
        imprimirVentanaInvertida(ventana);
        printf("\n");
    }

    destruirCola(ventana);
    return 0;
}
