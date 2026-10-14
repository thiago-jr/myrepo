#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    short dato;
    struct node *left;
    struct node *right;
}tipoNodo, *nodo;

typedef struct {
    nodo root;
}tipoArbol, *arbol;

nodo crearNodo (short); // crea el nodno que almacena el dato y las direcicions
arbol crearArbol (); // crea la estructura que contiene a los nodos
arbol crearArbolPreorden (short);

int main()
{
    
}

arbol crearArbolPreorden (short valor){
    nodo nuevo = crearNodo(valor);
    nuevo->dato;
    nuevo->left = crearArbol();
    nuevo->right = crearArbol();

}

nodo crearNodo (short valor){
    nodo nuevo = (nodo) malloc(sizeof(tipoNodo));
    nuevo->dato = valor;
    nuevo->left = NULL;
    nuevo->right = NULL;
    return nuevo;
}

arbol crearArbol (){
    arbol nuevo = (arbol) malloc(sizeof(tipoArbol));
    nuevo->root = NULL;
    return nuevo;
}  