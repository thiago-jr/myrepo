#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  short dato;
  struct node *left;
  struct node *right;
} tipoNodo, *nodo;

typedef struct {
  nodo root;
} tipoArbol, *arbol;

arbol crearArbol();    // crea la estructura que contiene a los nodos
nodo crearNodo(short); // crea el nodno que almacena el dato y las direcicions
arbol crearArbolPreorden(short);
nodo creaeEnPreorden();
void preordenar(arbol);
void recorrerEnPreorden(nodo);

int main() {
  arbol myTree = crearArbol();
  preordenar(myTree);

  return 0;
}

void recorrerEnPreorden(nodo raiz) {
  if (raiz != NULL) {
    printf("%d", raiz->dato);
    recorrerEnPreorden(raiz->left);
    recorrerEnPreorden(raiz->right);
  }
}

void preordenar(arbol tree) {
  if (tree->root != NULL) {
    recorrerEnPreorden(tree->root);
  }
}

nodo crearEnPreorden() {
  int valor;
  scanf("%d", &valor);
  if (valor == 0) {
    return NULL;
  }
  nodo nuevo = crearNodo(valor);
  nuevo->left = crearEnPreorden();
  nuevo->right = crearEnPreorden();
  return nuevo;
}

arbol crearArbolPreorden(short valor) {
  arbol tree = crearArbol();
  tree->root = crearEnPreorden();
  return tree;
}

nodo crearNodo(short valor) {
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
