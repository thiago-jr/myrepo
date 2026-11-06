
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  char dato[21];
  struct node *nxt;
} tipoNodo, *nodo;

typedef struct {
  nodo front;
  nodo rear;
} tipoLista, *lista;

nodo crearNodo(char *);
lista crearLista();
void enqueue(char *, lista);
void dequeue(lista);
int isEmpty(lista);
int longitudPrefijo(lista);
void imprimirPrefijo(lista, int);
void destruirLista(lista);

int main() {
  lista cadenas = crearLista();
  char palabra[21];
  int n, i, longitud;
  scanf("%d", &n);
  for (i = 0; i < n; i++) {
    scanf("%20s", palabra);
    enqueue(palabra, cadenas);
  }
  longitud = longitudPrefijo(cadenas);
  imprimirPrefijo(cadenas, longitud);
  destruirLista(cadenas);

  return 0;
}

nodo crearNodo(char *valor) {
  int i = 0;
  nodo nuevo = (nodo)malloc(sizeof(tipoNodo));
  while (valor[i] != '\0') {
    nuevo->dato[i] = valor[i];
    i++;
  }
  nuevo->dato[i] = '\0';
  nuevo->nxt = NULL;
  return nuevo;
}

lista crearLista() {
  lista nuevaLista = (lista)malloc(sizeof(tipoLista));
  nuevaLista->front = NULL;
  nuevaLista->rear = NULL;
  return nuevaLista;
}

void enqueue(char *valor, lista listaActual) {
  nodo nuevo = crearNodo(valor);
  if (listaActual->rear == NULL) {
    listaActual->front = nuevo;
  } else {
    listaActual->rear->nxt = nuevo;
  }
  listaActual->rear = nuevo;
}

void dequeue(lista listaActual) {
  nodo tope = listaActual->front;
  listaActual->front = tope->nxt;
  if (listaActual->front == NULL) {
    listaActual->rear = NULL;
  }
  free(tope);
}

int isEmpty(lista listaActual) { return listaActual->front == NULL; }

int longitudPrefijo(lista listaActual) {
  nodo base = listaActual->front;
  nodo actual = base->nxt;
  int longitud = 0;
  int i;
  while (base->dato[longitud] != '\0') {
    longitud++;
  }
  while (actual != NULL) {
    i = 0;
    while (i < longitud && base->dato[i] == actual->dato[i]) {
      i++;
    }
    longitud = i;
    actual = actual->nxt;
  }
  return longitud;
}

void imprimirPrefijo(lista listaActual, int longitud) {
  int i;
  if (longitud == 0) {
    printf("-");
  } else {
    for (i = 0; i < longitud; i++) {
      printf("%c", listaActual->front->dato[i]);
    }
  }
}

void destruirLista(lista listaActual) {
  while (!isEmpty(listaActual)) {
    dequeue(listaActual);
  }
  free(listaActual);
}
