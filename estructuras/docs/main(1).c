/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP,
Ruby, C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS,
JS Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int dato;
  struct node *left;
  struct node *right;
} tipoNodo, *nodo;

typedef struct {
  nodo root;
} tipoArbol, *arbol;

nodo crearNodo(int);
nodo crearEnPreorden();
arbol crearArbol();
arbol crearArbolPreorden();
void preorden(arbol);
void recorridoPreorden(nodo);

int main() {
  arbol myarbol = crearArbolPreorden();
  preorden(myarbol);

  return 0;
}

void preorden(arbol tree) {
  if (tree->root != NULL) {
    recorridoPreorden(tree->root);
  }
}

void recorridoPreorden(nodo raiz) {
  if (raiz != NULL) {
    printf("%d", raiz->dato);
    recorridoPreorden(raiz->left);
    recorridoPreorden(raiz->right);
  }
}

nodo crearEnPreorden() {
  int valor;
  scanf("%d", &valor);
  if (valor == 0) {
    return NULL;
  }
  nodo nuevo =
      crearNodo(valor); // solo se asigna cuando se termina de recorrer la rama
  nuevo->left = crearEnPreorden();
  nuevo->right = crearEnPreorden();
  return nuevo;
}

arbol crearArbolPreorden() {
  arbol tree = crearArbol();
  tree->root = crearEnPreorden();
}

nodo crearNodo(int valor) {
  nodo nuevo = (nodo)malloc(sizeof(tipoNodo));
  nuevo->dato = valor;
  nuevo->left = NULL;
  nuevo->right = NULL;
  return nuevo;
}

arbol crearArbol() {
  arbol nuevo = (arbol)malloc(sizeof(tipoArbol));
  nuevo->root = NULL;
  return nuevo;
}
