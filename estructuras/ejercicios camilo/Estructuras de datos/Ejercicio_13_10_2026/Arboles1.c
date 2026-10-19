#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int dato;
    struct node *left;
    struct node *right;
} tipoNodo, *nodo;

typedef struct {
    nodo root;
} tipoArbol, *arbolBinario;

nodo crearNodo(int valor){
    nodo nuevo = (nodo) malloc(sizeof(tipoNodo));
    nuevo->dato = valor;
    nuevo->left = NULL;
    nuevo->right = NULL;
    return nuevo;
}

arbolBinario crearArbol(){
    arbolBinario nuevo = (arbolBinario) malloc(sizeof(tipoArbol));
    nuevo->root = NULL;
    return nuevo;
}

nodo crearArbolPreorden(){
    int valor;
    scanf("%d", &valor);
    if(valor == 0){
        return NULL;
    }
    nodo nuevo = crearNodo(valor);
    nuevo->left = crearArbolPreorden();
    nuevo->right = crearArbolPreorden();
    return nuevo;
}

void imprimirArbol(nodo raiz) {

    printf("%d ", raiz->dato);
    imprimirArbol(raiz->left);
    imprimirArbol(raiz->right);
}

// 5 0 0
// 7 3 0 0 9 0 0
// 8 4 2 0 0 0 10 0 12 0 0

int main() {

    arbolBinario arbol = crearArbol();

    printf("VALORES:\n");
    arbol->root = crearArbolPreorden();
    imprimirArbol(arbol->root);
    free(arbol);

    return 0;
}