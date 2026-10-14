#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    int N[30] = {0};
    int frecuencia[6] = {0};
    int a, b, c, d, e;

    for ( int i = 0; i < 30; i++)
    {   
        int numero = rand() % 6;
        N[i] = numero;
        
        if (N[i]== 1)
        {
            a++;
        }
        if (N[i]== 2)
        {
            b++;
        }
        if (N[i]== 3)
        {
            c++;
        }
        if (N[i]== 4)
        {
            d++;
        }
        if (N[i]== 5)
        {
            e++;
        }
        
    }

    printf("1 - %d\n",a);
    printf("2 - %d\n",b);
    printf("3 - %d\n",c);
    printf("4 - %d\n",d);
    printf("5 - %d\n",e);
 



    return 0;
}
