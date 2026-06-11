#include "func/funcoes.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;

    do
    {
        printf("Digite a quantidade de leituras (1 a 9999): ");
        scanf("%d",&n);
        if (n < 1 || n > 9999)
            printf("Valor invalido!");
    } while (n < 1 || n > 9999);

    float *leituras = lerLeituras(n);








    free(leituras);
    return 0;
}