#include <stdio.h>

int main() {

    char cadena[100];
    int i, balance = 0;

    printf("caracteres: ");
    scanf("%s", cadena);
    
    for (i = 0; cadena[i] != '\0'; i++) {
        if (cadena[i] == '(' || cadena[i] == '{' || cadena[i] == '[') {
            balance++;
        } else if (cadena[i] == ')' || cadena[i] == '}' || cadena[i] == ']') {
            balance--;
            if (balance < 0) {
                break;
            }
        }
    }

    if (balance == 0) {
        printf("SI\n");
    } else {
        printf("NO\n");
    }

    return 0;

}
