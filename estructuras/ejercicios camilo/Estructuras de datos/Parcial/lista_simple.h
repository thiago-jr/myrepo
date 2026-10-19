#ifndef LISTA_SIMPLE_H
#define LISTA_SIMPLE_H

typedef struct node {
    short dato;
    struct node *nxt;
} tipoNodo, *nodo;

typedef struct {
    nodo top;
} tipoLista, *lista;

nodo crearNodo(short valor);
lista crearLista();
void push(short valor, lista listaActual);
short pop(lista listaActual);
void enqueue(short valor, lista listaActual);
short extract(lista listaActual);
void insert(short valor, int pos, lista listaActual);
int size(lista listaActual);
int isEmpty(lista listaActual);
void imprimir(lista listaActual);
void destruirLista(lista listaActual);

#endif