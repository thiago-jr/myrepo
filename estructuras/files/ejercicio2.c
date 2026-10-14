#include <stdio.h>
#include <stdlib.h>

int main ()
{
    int num = 0;
    int A[20] = {0};

    for (int i = 0; i < 20; i++)
    {
        
        scanf("%d", &num);
        A[i] = num;
        
    }

    for (int i = 0; i < 20; i++)
    {
        int cont = 0;
        for (int j = 0; j < 20; j++)
        {
            if (A[i] == A[j])
            {
                cont++;
            }
            
        }

        if (cont == 1)
        {
            printf("%d\t",A[i]);
        }
        
    }
    
    return 0;

}

