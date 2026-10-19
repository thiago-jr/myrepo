#include <stdio.h>
#include <stdlib.h>
#include "lista_simple.h"

nodo crearNodo(short valor){
    nodo nuevo = (nodo) malloc(sizeof(tipoNodo));
    if(nuevo == NULL){
        printf("Error: sin memoria\n");
        exit(1);
    }
    nuevo->dato = valor;
    nuevo->nxt = NULL;
    return nuevo;
}

lista crearLista(){
    lista nuevaLista = (lista) malloc(sizeof(tipoLista));
    if(nuevaLista == NULL){
        printf("Error: sin memoria\n");
        exit(1);
    }
    nuevaLista->top = NULL;
    return nuevaLista;
}

void push(short valor, lista listaActual){
    nodo nuevo = crearNodo(valor);
    nuevo->nxt = listaActual->top;
    listaActual->top = nuevo;
}

short pop(lista listaActual){
    if(isEmpty(listaActual)){            // ANTES crasheaba si estaba vacia
        printf("Error: lista vacia\n");
        return -1;
    }
    short valor;
    nodo tope = listaActual->top;
    valor = tope->dato;
    listaActual->top = tope->nxt;
    free(tope);                          // sin esto: fuga de memoria
    return valor;
}

void enqueue(short valor, lista listaActual){
    nodo nuevo = crearNodo(valor);
    if(isEmpty(listaActual)){
        listaActual->top = nuevo;
        return;
    }
    nodo aux = listaActual->top;
    while(aux->nxt != NULL){             // caminar hasta el ultimo
        aux = aux->nxt;
    }
    aux->nxt = nuevo;
}

short extract(lista listaActual){
    return pop(listaActual);
}

void insert(short valor, int pos, lista listaActual){
    if(pos <= 0 || isEmpty(listaActual)){
        push(valor, listaActual);
        return;
    }
    nodo aux = listaActual->top;
    int i;
    for(i = 0; i < pos-1 && aux->nxt != NULL; i++){
        aux = aux->nxt;
    }
    nodo nuevo = crearNodo(valor);
    nuevo->nxt = aux->nxt;
    aux->nxt = nuevo;
}

int size(lista listaActual){
    int cont = 0;
    nodo aux = listaActual->top;
    while(aux != NULL){
        cont++;
        aux = aux->nxt;
    }
    return cont;
}

int isEmpty(lista listaActual){
    return listaActual->top == NULL; // devuelve 1 si es vacia, 0 si no lo es
}

void imprimir(lista listaActual){
    nodo aux = listaActual->top;
    while(aux != NULL){
        printf("%d ", aux->dato);
        aux = aux->nxt;
    }
    printf("\n");
}

void destruirLista(lista listaActual){
    while(!isEmpty(listaActual)){
        pop(listaActual);                // libera nodo por nodo
    }
    free(listaActual);                   // libera la lista en si
}