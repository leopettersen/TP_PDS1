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

void adicionarEstacao(struct Estacao *estacoes, int *total)
{
    /* Capacidade do vetor fixo definido em main.c. */
    if (*total >= 100)
    {
        printf("Limite de 100 estações atingido. Inclusão abortada.\n");
        return;
    }

    struct Estacao nova; /* preenchemos um struct local e copiamos ao final */
    int idValido;

    /* 1) ID único: lê e compara com os já cadastrados; repete enquanto duplicar. */
    do
    {
        printf("ID: ");
        scanf("%d", &nova.id);

        idValido = 1;
        for (int i = 0; i < *total; i++)
        {
            if (estacoes[i].id == nova.id)
            {
                idValido = 0;
                break;
            }
        }
        if (!idValido)
            printf("ID já cadastrado. Tente outro.\n");
    } while (!idValido);

    /* 2) Campos textuais. Limites (%39s/%19s) refletem o tamanho dos buffers
     *    declarados em struct Estacao para evitar estouro. %s pula whitespace,
     *    então lê uma única palavra (sem espaços). */
    printf("Nome: ");
    scanf("%39s", nova.nome);
    printf("Operador: ");
    scanf("%39s", nova.operador);
    printf("Sensor: ");
    scanf("%19s", nova.sensor);

    /* 3) Data: lê dia/mes/ano e valida com valida_data (reaproveitada). */
    do
    {
        printf("Data (dia mes ano, ex.: 15 3 2026): ");
        scanf("%d %d %d", &nova.data.dia, &nova.data.mes, &nova.data.ano);
        if (!valida_data(nova.data))
            printf("Data inválida (verifique dia/mês/ano; ano entre 1900 e 2100).\n");
    } while (!valida_data(nova.data));

    /* 4) Quantidade de leituras: 1 a 9999. */
    do
    {
        printf("Quantidade de leituras (1 a 9999): ");
        scanf("%d", &nova.n);
        if (nova.n < 1 || nova.n > 9999)
            printf("Valor inválido.\n");
    } while (nova.n < 1 || nova.n > 9999);

    /* 5) ALOCAÇÃO DINÂMICA do vetor de leituras.
     *    lerLeituras encapsula malloc(n * sizeof(float)). Se o sistema não
     *    tiver memória disponível, malloc retorna NULL — precisamos checar
     *    antes de gravar nele, senão acessamos ponteiro inválido (crash).
     *    Em caso de falha, abortamos a inclusão sem alterar estacoes nem total. */
    nova.leituras = lerLeituras(nova.n);
    if (nova.leituras == NULL)
    {
        printf("Falha de alocação de memória. Estação não cadastrada.\n");
        return;
    }

    /* 6) Leitura dos n valores no vetor recém-alocado. */
    for (int i = 0; i < nova.n; i++)
    {
        printf("Leitura %d: ", i + 1);
        scanf("%f", &nova.leituras[i]);
    }

    /* 7) Estatísticas: preenche media, variancia, desvioPadrao em nova. */
    calcularEstatisticas(&nova);

    /* 8) Cópia para o vetor (a cópia do struct copia também o PONTEIRO
     *    nova.leituras — o bloco alocado continua válido no heap). */
    estacoes[*total] = nova;
    (*total)++;

    printf("Estação cadastrada com sucesso. Total: %d\n", *total);
}