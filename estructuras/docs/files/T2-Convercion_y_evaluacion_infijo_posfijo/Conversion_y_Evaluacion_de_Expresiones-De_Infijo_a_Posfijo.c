
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  char dato[201];
  struct node *nxt;
} tipoNodo, *nodo;

typedef struct {
  nodo front;
  nodo rear;
} tipoCola, *cola;

typedef struct {
  nodo top;
} tipoPila, *pila;

typedef struct nodeValor {
  double dato;
  struct nodeValor *nxt;
} tipoNodoValor, *nodoValor;

typedef struct {
  nodoValor top;
} tipoPilaValor, *pilaValor;

nodo crearNodo(char *);
cola crearCola();
pila crearPila();
pilaValor crearPilaValor();
void copiar(char *, char *);
void enqueue(char *, cola);
void dequeue(cola, char *);
int isEmptyCola(cola);
void destruirCola(cola);
void push(char *, pila);
void pop(pila, char *);
char topeOperador(pila);
int isEmptyPila(pila);
void destruirPila(pila);
void pushValor(double, pilaValor);
double popValor(pilaValor);
int isEmptyPilaValor(pilaValor);
void destruirPilaValor(pilaValor);
int esOperador(char);
int precedencia(char);
int esDerecha(char);
double potencia(double, double);
double operar(char, double, double);
void procesarToken(char *, cola, pila);

int main() {
  cola salida = crearCola();
  pila operadores = crearPila();
  pilaValor valores = crearPilaValor();
  char expresion[256];
  char token[201];
  int i = 0, j, primero = 1;
  double a, b, resultado;
  fgets(expresion, 256, stdin);
  while (expresion[i] != '\0') {
    while (expresion[i] == ' ' || expresion[i] == '\n' || expresion[i] == '\r') {
      i++;
    }
    if (expresion[i] != '\0') {
      j = 0;
      while (expresion[i] != '\0' && expresion[i] != ' ' &&
             expresion[i] != '\n' && expresion[i] != '\r') {
        token[j] = expresion[i];
        j++;
        i++;
      }
      token[j] = '\0';
      procesarToken(token, salida, operadores);
    }
  }
  while (!isEmptyPila(operadores)) {
    pop(operadores, token);
    enqueue(token, salida);
  }
  while (!isEmptyCola(salida)) {
    dequeue(salida, token);
    if (!primero) {
      printf(" ");
    }
    primero = 0;
    printf("%s", token);
    if (token[1] == '\0' && esOperador(token[0])) {
      b = popValor(valores);
      a = popValor(valores);
      pushValor(operar(token[0], a, b), valores);
    } else {
      pushValor(atof(token), valores);
    }
  }
  printf("\n");
  resultado = popValor(valores);
  if (resultado > -1e15 && resultado < 1e15 &&
      resultado == (double)(long long)resultado) {
    printf("%lld\n", (long long)resultado);
  } else {
    printf("%.10g\n", resultado);
  }
  destruirCola(salida);
  destruirPila(operadores);
  destruirPilaValor(valores);

  return 0;
}

nodo crearNodo(char *valor) {
  nodo nuevo = (nodo)malloc(sizeof(tipoNodo));
  copiar(nuevo->dato, valor);
  nuevo->nxt = NULL;
  return nuevo;
}

cola crearCola() {
  cola nuevaCola = (cola)malloc(sizeof(tipoCola));
  nuevaCola->front = NULL;
  nuevaCola->rear = NULL;
  return nuevaCola;
}

pila crearPila() {
  pila nuevaPila = (pila)malloc(sizeof(tipoPila));
  nuevaPila->top = NULL;
  return nuevaPila;
}

pilaValor crearPilaValor() {
  pilaValor nuevaPila = (pilaValor)malloc(sizeof(tipoPilaValor));
  nuevaPila->top = NULL;
  return nuevaPila;
}

void copiar(char *destino, char *origen) {
  int i = 0;
  while (origen[i] != '\0') {
    destino[i] = origen[i];
    i++;
  }
  destino[i] = '\0';
}

void enqueue(char *valor, cola colaActual) {
  nodo nuevo = crearNodo(valor);
  if (colaActual->rear == NULL) {
    colaActual->front = nuevo;
  } else {
    colaActual->rear->nxt = nuevo;
  }
  colaActual->rear = nuevo;
}

void dequeue(cola colaActual, char *destino) {
  nodo tope = colaActual->front;
  copiar(destino, tope->dato);
  colaActual->front = tope->nxt;
  if (colaActual->front == NULL) {
    colaActual->rear = NULL;
  }
  free(tope);
}

int isEmptyCola(cola colaActual) { return colaActual->front == NULL; }

void destruirCola(cola colaActual) {
  char descartado[201];
  while (!isEmptyCola(colaActual)) {
    dequeue(colaActual, descartado);
  }
  free(colaActual);
}

void push(char *valor, pila pilaActual) {
  nodo nuevo = crearNodo(valor);
  nuevo->nxt = pilaActual->top;
  pilaActual->top = nuevo;
}

void pop(pila pilaActual, char *destino) {
  nodo tope = pilaActual->top;
  copiar(destino, tope->dato);
  pilaActual->top = tope->nxt;
  free(tope);
}

char topeOperador(pila pilaActual) { return pilaActual->top->dato[0]; }

int isEmptyPila(pila pilaActual) { return pilaActual->top == NULL; }

void destruirPila(pila pilaActual) {
  char descartado[201];
  while (!isEmptyPila(pilaActual)) {
    pop(pilaActual, descartado);
  }
  free(pilaActual);
}

void pushValor(double valor, pilaValor pilaActual) {
  nodoValor nuevo = (nodoValor)malloc(sizeof(tipoNodoValor));
  nuevo->dato = valor;
  nuevo->nxt = pilaActual->top;
  pilaActual->top = nuevo;
}

double popValor(pilaValor pilaActual) {
  double valor;
  nodoValor tope = pilaActual->top;
  valor = tope->dato;
  pilaActual->top = tope->nxt;
  free(tope);
  return valor;
}

int isEmptyPilaValor(pilaValor pilaActual) { return pilaActual->top == NULL; }

void destruirPilaValor(pilaValor pilaActual) {
  while (!isEmptyPilaValor(pilaActual)) {
    popValor(pilaActual);
  }
  free(pilaActual);
}

int esOperador(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int precedencia(char c) {
  if (c == '^') {
    return 3;
  }
  if (c == '*' || c == '/') {
    return 2;
  }
  return 1;
}

int esDerecha(char c) { return c == '^'; }

double potencia(double base, double exponente) {
  int e = (int)exponente;
  int negativo = 0;
  double resultado = 1;
  if (e < 0) {
    negativo = 1;
    e = -e;
  }
  while (e > 0) {
    resultado = resultado * base;
    e--;
  }
  if (negativo) {
    resultado = 1 / resultado;
  }
  return resultado;
}

double operar(char operador, double a, double b) {
  if (operador == '+') {
    return a + b;
  }
  if (operador == '-') {
    return a - b;
  }
  if (operador == '*') {
    return a * b;
  }
  if (operador == '/') {
    return a / b;
  }
  return potencia(a, b);
}

void procesarToken(char *token, cola salida, pila operadores) {
  char c = token[0];
  char auxiliar[201];
  if (esOperador(c) && token[1] == '\0') {
    while (!isEmptyPila(operadores) && esOperador(topeOperador(operadores)) &&
           (precedencia(topeOperador(operadores)) > precedencia(c) ||
            (precedencia(topeOperador(operadores)) == precedencia(c) &&
             !esDerecha(c)))) {
      pop(operadores, auxiliar);
      enqueue(auxiliar, salida);
    }
    push(token, operadores);
  } else if (c == '(') {
    push(token, operadores);
  } else if (c == ')') {
    pop(operadores, auxiliar);
    while (auxiliar[0] != '(') {
      enqueue(auxiliar, salida);
      pop(operadores, auxiliar);
    }
  } else {
    enqueue(token, salida);
  }
}
