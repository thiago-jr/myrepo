#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    char cadena[10];
    int frecuencia[26] = {0};

    for (int i = 0; i < 10; i++)
    {
        scanf("%c", &cadena[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        char car = tolower(cadena[i]);

        if (car >= 'a' && car <= 'z')
        {
            frecuencia[car - 'a']++;
        }
        
    }

    for (int i = 0; i < 26; i++)
    {
        if (frecuencia[i] > 0)
        {
            printf("%c: %d\n", 'a' + i, frecuencia[i]);
        }
        
        
    }
}