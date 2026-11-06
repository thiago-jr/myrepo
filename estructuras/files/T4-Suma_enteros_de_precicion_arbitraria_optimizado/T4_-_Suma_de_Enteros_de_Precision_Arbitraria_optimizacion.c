
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  short dato;
  struct node *nxt;
} tipoNodo, *nodo;

typedef struct {
  nodo front;
  nodo rear;
} tipoLista, *lista;

typedef struct {
  nodo top;
} tipoPila, *pila;

nodo crearNodo(short);
lista crearLista();
pila crearPila();
void insertarInicio(short, lista);
void insertarFinal(short, lista);
void leerNumero(lista);
lista sumar(lista, lista);
void imprimirNumero(lista);
void destruirLista(lista);
void push(short, pila);
short pop(pila);
int isEmpty(pila);
void destruirPila(pila);

int main() {
  lista primero = crearLista();
  lista segundo = crearLista();
  lista suma;
  leerNumero(primero);
  leerNumero(segundo);
  suma = sumar(primero, segundo);
  imprimirNumero(suma);
  destruirLista(primero);
  destruirLista(segundo);
  destruirLista(suma);

  return 0;
}

nodo crearNodo(short valor) {
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

void insertarInicio(short valor, lista listaActual) {
  nodo nuevo = crearNodo(valor);
  nuevo->nxt = listaActual->front;
  listaActual->front = nuevo;
  if (listaActual->rear == NULL) {
    listaActual->rear = nuevo;
  }
}

void insertarFinal(short valor, lista listaActual) {
  nodo nuevo = crearNodo(valor);
  if (listaActual->rear == NULL) {
    listaActual->front = nuevo;
  } else {
    listaActual->rear->nxt = nuevo;
  }
  listaActual->rear = nuevo;
}

void leerNumero(lista numero) {
  lista digitos = crearLista();
  nodo actual;
  short bloque, multiplicador;
  int i;
  int caracter = getchar();
  while (caracter != EOF && (caracter < '0' || caracter > '9')) {
    caracter = getchar();
  }
  while (caracter >= '0' && caracter <= '9') {
    insertarInicio((short)(caracter - '0'), digitos);
    caracter = getchar();
  }
  actual = digitos->front;
  while (actual != NULL) {
    bloque = 0;
    multiplicador = 1;
    for (i = 0; i < 4 && actual != NULL; i++) {
      bloque = bloque + actual->dato * multiplicador;
      multiplicador = multiplicador * 10;
      actual = actual->nxt;
    }
    insertarFinal(bloque, numero);
  }
  destruirLista(digitos);
}

lista sumar(lista primero, lista segundo) {
  lista resultado = crearLista();
  nodo a = primero->front;
  nodo b = segundo->front;
  short acarreo = 0;
  short suma;
  while (a != NULL || b != NULL || acarreo > 0) {
    suma = acarreo;
    if (a != NULL) {
      suma = suma + a->dato;
      a = a->nxt;
    }
    if (b != NULL) {
      suma = suma + b->dato;
      b = b->nxt;
    }
    insertarFinal((short)(suma % 10000), resultado);
    acarreo = (short)(suma / 10000);
  }
  return resultado;
}

void imprimirNumero(lista numero) {
  pila auxiliar = crearPila();
  nodo actual = numero->front;
  short bloque;
  int imprimiendo = 0;
  while (actual != NULL) {
    push(actual->dato, auxiliar);
    actual = actual->nxt;
  }
  while (!isEmpty(auxiliar)) {
    bloque = pop(auxiliar);
    if (imprimiendo) {
      printf("%04d", bloque);
    } else if (bloque != 0 || isEmpty(auxiliar)) {
      printf("%d", bloque);
      imprimiendo = 1;
    }
  }
  destruirPila(auxiliar);
}

void destruirLista(lista listaActual) {
  nodo actual = listaActual->front;
  nodo siguiente;
  while (actual != NULL) {
    siguiente = actual->nxt;
    free(actual);
    actual = siguiente;
  }
  free(listaActual);
}

void push(short valor, pila pilaActual) {
  nodo nuevo = crearNodo(valor);
  nuevo->nxt = pilaActual->top;
  pilaActual->top = nuevo;
}

short pop(pila pilaActual) {
  short valor;
  nodo tope = pilaActual->top;
  valor = tope->dato;
  pilaActual->top = tope->nxt;
  free(tope);
  return valor;
}

int isEmpty(pila pilaActual) { return pilaActual->top == NULL; }

void destruirPila(pila pilaActual) {
  while (!isEmpty(pilaActual)) {
    pop(pilaActual);
  }
  free(pilaActual);
}
