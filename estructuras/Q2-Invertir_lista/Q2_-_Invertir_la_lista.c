
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  float dato;
  struct node *nxt;
} tipoNodo, *nodo;

typedef struct {
  nodo front;
  nodo rear;
} tipoLista, *lista;

typedef struct {
  nodo top;
} tipoPila, *pila;

nodo crearNodo(float);
lista crearLista();
pila crearPila();
void enqueue(float, lista);
float dequeue(lista);
int isEmptyLista(lista);
void destruirLista(lista);
void push(float, pila);
float pop(pila);
int isEmptyPila(pila);
void destruirPila(pila);

int main() {
  lista original = crearLista();
  pila invertida = crearPila();
  float numero;
  int contador = 0;
  scanf("%f", &numero);
  while (contador < 3) {
    if (numero == -1) {
      contador++;
    }
    if (contador < 3) {
      enqueue(numero, original);
      scanf("%f", &numero);
    }
  }
  while (!isEmptyLista(original)) {
    push(dequeue(original), invertida);
  }
  while (!isEmptyPila(invertida)) {
    printf("%g", pop(invertida));
    if (!isEmptyPila(invertida)) {
      printf(" ");
    }
  }
  destruirLista(original);
  destruirPila(invertida);

  return 0;
}

nodo crearNodo(float valor) {
  nodo nuevo = (nodo)malloc(sizeof(tipoNodo));
  nuevo->dato = valor;
  nuevo->nxt = NULL;
  return nuevo;
}

lista crearLista() {
  lista nuevaLista = (lista)malloc(sizeof(tipoLista));
  nuevaLista->front = NULL;
  nuevaLista->rear = NULL;
  return nuevaLista;
}

pila crearPila() {
  pila nuevaPila = (pila)malloc(sizeof(tipoPila));
  nuevaPila->top = NULL;
  return nuevaPila;
}

void enqueue(float valor, lista listaActual) {
  nodo nuevo = crearNodo(valor);
  if (listaActual->rear == NULL) {
    listaActual->front = nuevo;
  } else {
    listaActual->rear->nxt = nuevo;
  }
  listaActual->rear = nuevo;
}

float dequeue(lista listaActual) {
  float valor;
  nodo tope = listaActual->front;
  valor = tope->dato;
  listaActual->front = tope->nxt;
  if (listaActual->front == NULL) {
    listaActual->rear = NULL;
  }
  free(tope);
  return valor;
}

int isEmptyLista(lista listaActual) { return listaActual->front == NULL; }

void destruirLista(lista listaActual) {
  while (!isEmptyLista(listaActual)) {
    dequeue(listaActual);
  }
  free(listaActual);
}

void push(float valor, pila pilaActual) {
  nodo nuevo = crearNodo(valor);
  nuevo->nxt = pilaActual->top;
  pilaActual->top = nuevo;
}

float pop(pila pilaActual) {
  float valor;
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
