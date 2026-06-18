/* funcoes.c — Implementações das funções declaradas em funcoes.h. */
#include "funcoes.h"
#include <math.h>

void exibirMenu(void)
{
    printf("\n=== Menu ===\n");
    printf("1 - Adicionar estação\n");
    printf("2 - Listar estações\n");
    printf("3 - Buscar por operador\n");
    printf("4 - Editar estação\n");
    printf("5 - Remover estação\n");
    printf("6 - Detectar anomalias\n");
    printf("7 - Salvar CSV\n");
    printf("8 - Carregar CSV\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

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

/* FUNÇÃO RECURSIVA
 * Caso base: n == 0 -> retorna 0.
 * Recursão (soma recursiva): soma(n) = v[n-1] + soma(n-1), media = soma / n.
 * Como esta função retorna a média (e não a soma), reconstrói a soma do
 * prefixo a partir da média do prefixo: soma(n-1) = media(n-1) * (n-1). */
float calcularMediaRecursiva(float *v, int n)
{
    if (n == 0)
        return 0.0f;
    return (v[n - 1] + calcularMediaRecursiva(v, n - 1) * (n - 1)) / n;
}

float calcularVariancia(float *v, int n, float media)
{
    /* Variância populacional: somatório dos (v[i] - media)^2 dividido por n. */
    if (n == 0)
        return 0.0f;

    float soma = 0.0f;
    for (int i = 0; i < n; i++)
    {
        float desvio = v[i] - media;
        soma += desvio * desvio;
    }
    return soma / n;
}

float calcularDesvioPadrao(float variancia)
{
    /* Desvio padrão é a raiz quadrada da variância. */
    return (float) sqrt(variancia);
}

void calcularEstatisticas(struct Estacao *e)
{
    /* Pipeline: média (recursiva) -> variância (usa a média) -> desvio padrão. */
    e->media = calcularMediaRecursiva(e->leituras, e->n);
    e->variancia = calcularVariancia(e->leituras, e->n, e->media);
    e->desvioPadrao = calcularDesvioPadrao(e->variancia);
}