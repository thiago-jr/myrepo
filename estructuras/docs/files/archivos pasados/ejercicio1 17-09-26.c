#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    float dato;
    struct node *nxt;
}tiponodo, *nodo;

nodo crearnodo(float valor);
void orderInsertion(nodo *head,float valor);
void printList(nodo head);
void freeList(nodo head);

int main(){

    return 0;
}

nodo crearnodo(float valor){
    nodo nuevonodo = (nodo)malloc(sizeof(tiponodo));
    nuevonodo->dato = valor;
    nuevonodo->nxt = NULL;
    return nuevonodo;
}

void orderInsertion(nodo *head,float valor){
    nodo nuevo = crearnodo(valor);

    if(*head == NULL || (*head)->dato >= valor){
        nuevo->nxt = *head;
        *head = nuevo;

        return;
    }
    nodo actual = *head
};
    

