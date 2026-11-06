#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    char datos[200][20];
    int top;
}pilaChar;

void initPilaChar(pilaChar *p) {p->top = -1;}
int isEmptyChar(pilaChar *p) {return p->top == -1;}
void puchChar(pilaChar *p, char *val) {p->top++; strcpy(p->datos[p->top], val);}




int main(){

};