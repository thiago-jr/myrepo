
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int dato;
  struct node *nxt;
} tipoNodo, *nodo;

typedef struct {
  nodo front;
  nodo rear;
  int tamano;
} tipoCola, *cola;

typedef struct {
  nodo top;
} tipoPila, *pila;

nodo crearNodo(int);
cola crearCola();
pila crearPila();
void enqueue(int, cola);
int dequeue(cola);
int isEmptyCola(cola);
int tamanoCola(cola);
void destruirCola(cola);
void push(int, pila);
int pop(pila);
int isEmptyPila(pila);
void destruirPila(pila);
void imprimirVentanaInvertida(cola);

int main() {
  cola ventana = crearCola();
  int k, numero;
  scanf("%d", &k);
  scanf("%d", &numero);
  while (numero != -1) {
    if (tamanoCola(ventana) == k) {
      dequeue(ventana);
    }
    enqueue(numero, ventana);
    printf("Ingresa %d -> Ventana invertida:", numero);
    imprimirVentanaInvertida(ventana);
    printf("\n");
    scanf("%d", &numero);
  }
  destruirCola(ventana);

  return 0;
}

nodo crearNodo(int valor) {
  nodo nuevo = (nodo)malloc(sizeof(tipoNodo));
  nuevo->dato = valor;
  nuevo->nxt = NULL;
  return nuevo;
}

cola crearCola() {
  cola nuevaCola = (cola)malloc(sizeof(tipoCola));
  nuevaCola->front = NULL;
  nuevaCola->rear = NULL;
  nuevaCola->tamano = 0;
  return nuevaCola;
}

pila crearPila() {
  pila nuevaPila = (pila)malloc(sizeof(tipoPila));
  nuevaPila->top = NULL;
  return nuevaPila;
}

void enqueue(int valor, cola colaActual) {
  nodo nuevo = crearNodo(valor);
  if (colaActual->rear == NULL) {
    colaActual->front = nuevo;
  } else {
    colaActual->rear->nxt = nuevo;
  }
  colaActual->rear = nuevo;
  colaActual->tamano++;
}

int dequeue(cola colaActual) {
  int valor;
  nodo tope = colaActual->front;
  valor = tope->dato;
  colaActual->front = tope->nxt;
  if (colaActual->front == NULL) {
    colaActual->rear = NULL;
  }
  colaActual->tamano--;
  free(tope);
  return valor;
}

int isEmptyCola(cola colaActual) { return colaActual->front == NULL; }

int tamanoCola(cola colaActual) { return colaActual->tamano; }

void destruirCola(cola colaActual) {
  while (!isEmptyCola(colaActual)) {
    dequeue(colaActual);
  }
  free(colaActual);
}

void push(int valor, pila pilaActual) {
  nodo nuevo = crearNodo(valor);
  nuevo->nxt = pilaActual->top;
  pilaActual->top = nuevo;
}

int pop(pila pilaActual) {
  int valor;
  nodo tope = pilaActual->top;
  valor = tope->dato;
  pilaActual->top = tope->nxt;
  free(tope);
  return valor;
}

int isEmptyPila(pila pilaActual) { return pilaActual->top == NULL; }

void destruirPila(pila pilaActual) {
  while (!isEmptyPila(pilaActual)) {
    pop(pilaActual);
  }
  free(pilaActual);
}

void imprimirVentanaInvertida(cola c) {
  pila auxiliar = crearPila();
  pila restaurar = crearPila();
  int valor;
  while (!isEmptyCola(c)) {
    push(dequeue(c), auxiliar);
  }
  while (!isEmptyPila(auxiliar)) {
    valor = pop(auxiliar);
    printf(" %d", valor);
    push(valor, restaurar);
  }
  while (!isEmptyPila(restaurar)) {
    enqueue(pop(restaurar), c);
  }
  destruirPila(auxiliar);
  destruirPila(restaurar);
}
