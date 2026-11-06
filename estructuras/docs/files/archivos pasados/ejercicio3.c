#include <stdio.h>
#include <stdlib.h>
#include <float.h>

int main()
{   
    float A[10] = {0};
    float flo = 0, mayor1 = 0, mayor2 = 0, menor1 = 0, menor2 = 0;

    for (int i = 0; i < 10; i++)
    {
        scanf("%f", &flo);
        A[i] = flo;
    }

    mayor1 = -FLT_MAX, mayor2 = -FLT_MAX;
    menor1 = FLT_MAX, menor2 = FLT_MAX;
    
    for (int i = 0; i < 10; i++)
    {
        if (A[i] > mayor1)
        {
            mayor2 = mayor1;
            mayor1 = A[i];
        }
        else if (A[i] > mayor2 && A[i] != mayor1)
        {
            mayor2 = A[i];
        }
        
        if (A[i] < menor1)
        {
            menor2 = menor1;
            menor1 = A[i];
        }
        else if (A[i] < menor2 && A[i] != menor1)
        {
            menor2 = A[i];
        }
        
        
    }

    printf("%.2f\n", mayor2);
    printf("%.2f", menor2);

    return 0;
}