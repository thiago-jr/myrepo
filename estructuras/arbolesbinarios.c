/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *left;
    struct node *right;
}tipoNodo, *nodo;

typedef struct {
    nodo root;
} tipoArbol, *arbolBinario;

nodo crearNodo(int);
arbolBinario crearArbolBinario();
arbolBinario crearArbolBinarioPreorden();
nodo crearEnPreorden();
void preorden(arbolBinario);
void recorridoPreorden(nodo);

int main()
{
    arbolBinario myTree = crearArbolBinarioPreorden();
    preorden(myTree);

    return 0;
}

nodo crearNodo(int valor) {
    nodo nuevoNodo = (nodo) malloc(sizeof(tipoNodo));
    nuevoNodo->data = valor;
    nuevoNodo->left = NULL;
    nuevoNodo->right = NULL;
    return nuevoNodo;
}

arbolBinario crearArbolBinario() {
    arbolBinario nuevoArbol = (arbolBinario) malloc(sizeof(tipoArbol));
    nuevoArbol->root = NULL;
    return nuevoArbol;
}

arbolBinario crearArbolBinarioPreorden() {
    arbolBinario tree = crearArbolBinario();
    tree->root = crearEnPreorden();
    return tree;
}

nodo crearEnPreorden() {
    int valor;
    scanf("%d", &valor);
    if (valor == 0) {
        return NULL;
    }
    nodo nuevoNodo = crearNodo(valor);
    nuevoNodo->left = crearEnPreorden();
    nuevoNodo->right = crearEnPreorden();
    return nuevoNodo;
}

void preorden(arbolBinario tree) {
    if (tree->root != NULL) {
        recorridoPreorden(tree->root);
    }
}

void recorridoPreorden(nodo raiz) {
    if (raiz != NULL){
        printf("%d ", raiz->data);
        recorridoPreorden(raiz->left);
        recorridoPreorden(raiz->right);
    }
}

