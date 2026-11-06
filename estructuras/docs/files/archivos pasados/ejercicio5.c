#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n = 0;
    char cadena[10][20];

    scanf("%d", &n);

    if (n >= 2 && n <= 10)
    {
        
        for (int i = 0; i < n; i++)
        {
            scanf(" %19s", &cadena[i]);
        
        }    
    }
    
    return 0;
}