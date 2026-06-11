#include "funcoes.h"

float *lerLeituras(int n)
{
    float *leituras = (float*) malloc(n * sizeof(float));
    return leituras;
}

bool ano_bissexto(struct DataLeitura data)
{
    if (data.ano % 4 == 0)
    {
        if (data.ano % 400 == 0)
            return true;

        if (data.ano % 100 == 0)
            return false;
        
        return true;
    }
    return false;
}

bool valida_data(struct DataLeitura data)
{
    if (data.dia > 31 ||
        data.dia < 1 ||
        data.mes > 12 ||
        data.mes < 1 ||
        data.ano > 2100 ||
        data.ano < 1900)
        return false;
    
    switch (data.mes)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return true;

        case 4:
        case 6:
        case 9:
        case 11:
            return data.dia <= 30;
        
        case 2:
            return data.dia <= (ano_bissexto(data) ? 29 : 28);
        
        default:
            return false;
    }
}