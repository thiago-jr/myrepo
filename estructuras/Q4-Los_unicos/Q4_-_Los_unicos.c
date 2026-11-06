
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int dato;
  struct node *nxt;
} tipoNodo, *nodo;

typedef struct {
  nodo front;
  nodo rear;
} tipoLista, *lista;

nodo crearNodo(int);
lista crearLista();
void enqueue(int, lista);
int dequeue(lista);
int isEmpty(lista);
int contarOcurrencias(int, lista);
void filtrarUnicos(lista, lista);
void imprimirLista(lista);
void destruirLista(lista);

int main() {
  lista original = crearLista();
  lista unicos = crearLista();
  int cantidad, i, numero;
  scanf("%d", &cantidad);
  for (i = 0; i < cantidad; i++) {
    scanf("%d", &numero);
    enqueue(numero, original);
  }
  filtrarUnicos(original, unicos);
  imprimirLista(unicos);
  destruirLista(original);
  destruirLista(unicos);

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
  nuevaLista->front = NULL;
  nuevaLista->rear = NULL;
  return nuevaLista;
}

void enqueue(int valor, lista listaActual) {
  nodo nuevo = crearNodo(valor);
  if (listaActual->rear == NULL) {
    listaActual->front = nuevo;
  } else {
    listaActual->rear->nxt = nuevo;
  }
  listaActual->rear = nuevo;
}

int dequeue(lista listaActual) {
  int valor;
  nodo tope = listaActual->front;
  valor = tope->dato;
  listaActual->front = tope->nxt;
  if (listaActual->front == NULL) {
    listaActual->rear = NULL;
  }
  free(tope);
  return valor;
}

int isEmpty(lista listaActual) { return listaActual->front == NULL; }

int contarOcurrencias(int valor, lista listaActual) {
  int contador = 0;
  nodo actual = listaActual->front;
  while (actual != NULL) {
    if (actual->dato == valor) {
      contador++;
    }
    actual = actual->nxt;
  }
  return contador;
}

void filtrarUnicos(lista origen, lista destino) {
  nodo actual = origen->front;
  while (actual != NULL) {
    if (contarOcurrencias(actual->dato, origen) == 1) {
      enqueue(actual->dato, destino);
    }
    actual = actual->nxt;
  }
}

void imprimirLista(lista listaActual) {
  nodo actual = listaActual->front;
  while (actual != NULL) {
    printf("%d", actual->dato);
    if (actual->nxt != NULL) {
      printf(" ");
    }
    actual = actual->nxt;
  }
}

void destruirLista(lista listaActual) {
  while (!isEmpty(listaActual)) {
    dequeue(listaActual);
  }
  free(listaActual);
}
