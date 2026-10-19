#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    short dato;
    struct node *nxt;
}tipoNodo, *nodo;

typedef struct{
    nodo top;
}tipoLista, *lista;

nodo crearNodo(short);
lista crearLista();
void push(short, lista);
short pop(lista);
int isEmpty(lista);

int main()
{
    lista binario = crearLista();
    int numero;
    short bit;
    scanf("%d", &numero);
    while(numero != 0){
        bit = (short) (numero % 2);
        numero = (numero - bit) / 2;
        push(bit, binario);
    }
    while(!isEmpty(binario)){
        printf("%d", pop(binario));
    }
    
    
    return 0;
}

nodo crearNodo(short valor){
    nodo nuevo = (nodo) malloc(sizeof(tipoNodo));
    nuevo->dato = valor;
    nuevo->nxt = NULL;
    return nuevo;
}

lista crearLista(){
    lista nuevaLista = (lista) malloc(sizeof(tipoLista));
    nuevaLista->top = NULL;
    return nuevaLista;
}

void push(short valor, lista listaActual){
    nodo nuevo = crearNodo(valor);
    nuevo->nxt = listaActual->top;
    listaActual->top = nuevo;
}

short pop(lista listaActual){
    short valor;
    nodo tope = listaActual->top;
    valor = tope->dato;
    listaActual->top = tope->nxt;
    free(tope);
    return valor;
}

int isEmpty(lista listaActual){
    return listaActual->top == NULL;
}