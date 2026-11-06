#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    char caracter;
    struct node *nxt;
}tiponodo, *nodo;

typedef struct{
    nodo top;
}tipolista, *lista;

nodo crearnodo(char);
lista crearlista();
void push(char, lista);
int isempty(lista);

int main(){

    return 0;
}