
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
void imprimirLista(lista);
void destruirLista(lista);

int main() {
  lista original = crearLista();
  lista pares = crearLista();
  lista impares = crearLista();
  int numero;
  scanf("%d", &numero);
  while (numero != 0) {
    enqueue(numero, original);
    scanf("%d", &numero);
  }
  while (!isEmpty(original)) {
    numero = dequeue(original);
    if (numero % 2 == 0) {
      enqueue(numero, pares);
    } else {
      enqueue(numero, impares);
    }
  }
  imprimirLista(pares);
  printf("\n");
  imprimirLista(impares);
  printf("\n");
  destruirLista(original);
  destruirLista(pares);
  destruirLista(impares);

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
