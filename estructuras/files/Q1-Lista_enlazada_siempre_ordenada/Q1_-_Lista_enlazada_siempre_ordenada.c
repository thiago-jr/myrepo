
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int dato;
  struct node *nxt;
} tipoNodo, *nodo;

typedef struct {
  nodo top;
} tipoLista, *lista;

nodo crearNodo(int);
lista crearLista();
void insertarOrdenado(int, lista);
void imprimirLista(lista);
void destruirLista(lista);
int isEmpty(lista);

int main() {
  lista ordenada = crearLista();
  int numero;
  scanf("%d", &numero);
  while (numero != 0) {
    insertarOrdenado(numero, ordenada);
    scanf("%d", &numero);
  }
  imprimirLista(ordenada);
  destruirLista(ordenada);

  return 0;
}

nodo crearNodo(int valor) {
  nodo nuevo = (nodo)malloc(sizeof(tipoNodo));
  nuevo->dato = valor;
  nuevo->nxt = NULL;
  return nuevo;
}

lista crearLista() {
  lista nuevaLista = (lista)malloc(sizeof(tipoLista));
  nuevaLista->top = NULL;
  return nuevaLista;
}

void insertarOrdenado(int valor, lista listaActual) {
  nodo nuevo = crearNodo(valor);
  nodo actual = listaActual->top;
  if (actual == NULL || valor <= actual->dato) {
    nuevo->nxt = actual;
    listaActual->top = nuevo;
  } else {
    while (actual->nxt != NULL && actual->nxt->dato < valor) {
      actual = actual->nxt;
    }
    nuevo->nxt = actual->nxt;
    actual->nxt = nuevo;
  }
}

void imprimirLista(lista listaActual) {
  nodo actual = listaActual->top;
  while (actual != NULL) {
    printf("%d", actual->dato);
    if (actual->nxt != NULL) {
      printf(" ");
    }
    actual = actual->nxt;
  }
}

void destruirLista(lista listaActual) {
  nodo tope;
  while (!isEmpty(listaActual)) {
    tope = listaActual->top;
    listaActual->top = tope->nxt;
    free(tope);
  }
  free(listaActual);
}

int isEmpty(lista listaActual) { return listaActual->top == NULL; }
