/* funcoes.c — Implementações das funções declaradas em funcoes.h. */
#include "funcoes.h"
#include <math.h>
#include <string.h>

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

void listarEstacoes(struct Estacao *estacoes, int total)
{
    /* Caso especial: nenhuma estação cadastrada — informa e encerra. */
    if (total == 0)
    {
        printf("Nenhuma estação cadastrada.\n");
        return;
    }

    printf("\n=== %d estação(ões) cadastrada(s) ===\n", total);

    /* Itera pelas estações imprimindo cada campo em linha própria. */
    for (int i = 0; i < total; i++)
    {
        struct Estacao *e = &estacoes[i]; /* alias para encurtar o acesso */

        printf("\n--- Estação %d ---\n", i + 1);
        printf("ID........: %d\n", e->id);
        printf("Nome......: %s\n", e->nome);
        printf("Operador..: %s\n", e->operador);
        printf("Sensor....: %s\n", e->sensor);

        /* Data com zero à esquerda: dd/mm/aaaa. */
        printf("Data......: %02d/%02d/%04d\n", e->data.dia, e->data.mes, e->data.ano);

        /* Leituras separadas por vírgula na mesma linha. */
        printf("Leituras..: (n=%d) ", e->n);
        for (int j = 0; j < e->n; j++)
            printf("%.2f%s", e->leituras[j], (j == e->n - 1) ? "" : ", ");
        printf("\n");

        printf("Média.....: %.4f\n", e->media);
        printf("Variância.: %.4f\n", e->variancia);
        printf("Desv. pad.: %.4f\n", e->desvioPadrao);
    }
}

void buscarPorOperador(struct Estacao *estacoes, int total)
{
    /* Caso vazio: nada a buscar. */
    if (total == 0)
    {
        printf("Nenhuma estação cadastrada.\n");
        return;
    }

    /* Buffer do mesmo tamanho do campo operador na struct (40). */
    char alvo[40];
    printf("Operador a buscar: ");
    scanf("%39s", alvo);

    int encontrados = 0;

    /* Percorre todas as estações; strcmp == 0 indica nomes iguais. */
    for (int i = 0; i < total; i++)
    {
        if (strcmp(estacoes[i].operador, alvo) == 0)
        {
            struct Estacao *e = &estacoes[i];
            printf("\n--- Estação ID %d ---\n", e->id);
            printf("Nome......: %s\n", e->nome);
            printf("Sensor....: %s\n", e->sensor);
            printf("Data......: %02d/%02d/%04d\n", e->data.dia, e->data.mes, e->data.ano);
            printf("Média.....: %.4f\n", e->media);
            printf("Desv. pad.: %.4f\n", e->desvioPadrao);
            encontrados++;
        }
    }

    /* Se nenhuma estação bateu com o operador, avisa o usuário. */
    if (encontrados == 0)
        printf("Nenhuma estação encontrada para o operador '%s'.\n", alvo);
    else
        printf("\n%d estação(ões) encontrada(s).\n", encontrados);
}

void editarEstacao(struct Estacao *estacoes, int total)
{
    /* Caso vazio: nada a editar. */
    if (total == 0)
    {
        printf("Nenhuma estação cadastrada.\n");
        return;
    }

    int id;
    printf("ID da estação a editar: ");
    scanf("%d", &id);

    /* Busca linear pelo id. idx = -1 sinaliza "não encontrado". */
    int idx = -1;
    for (int i = 0; i < total; i++)
    {
        if (estacoes[i].id == id)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        printf("Estação com ID %d não encontrada.\n", id);
        return;
    }

    /* Ponteiro para a estação encontrada — alterações são feitas in-place. */
    struct Estacao *e = &estacoes[idx];
    int op;

    /* Submenu de edição: roda até o usuário escolher 0 (Voltar). */
    do
    {
        printf("\n--- Editar Estação ID %d ---\n", e->id);
        printf("1 - Nome     (%s)\n", e->nome);
        printf("2 - Operador (%s)\n", e->operador);
        printf("3 - Sensor   (%s)\n", e->sensor);
        printf("4 - Data     (%02d/%02d/%04d)\n",
               e->data.dia, e->data.mes, e->data.ano);
        printf("0 - Voltar\n");
        printf("Escolha: ");

        /* Mesma proteção contra entrada não numérica do menu principal. */
        if (scanf("%d", &op) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            op = -1;
        }

        switch (op)
        {
            case 1:
                printf("Novo nome: ");
                scanf("%39s", e->nome);
                break;

            case 2:
                printf("Novo operador: ");
                scanf("%39s", e->operador);
                break;

            case 3:
                printf("Novo sensor: ");
                scanf("%19s", e->sensor);
                break;

            case 4:
            {
                /* Lê a data num temporário e só grava após validação,
                 * para não corromper a data atual em caso de cancelamento. */
                struct DataLeitura nova;
                do
                {
                    printf("Nova data (dia mes ano): ");
                    scanf("%d %d %d", &nova.dia, &nova.mes, &nova.ano);
                    if (!valida_data(nova))
                        printf("Data inválida (verifique dia/mês/ano; ano 1900-2100).\n");
                } while (!valida_data(nova));
                e->data = nova;
                break;
            }

            case 0:
                break;

            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (op != 0);

    printf("Edição concluída.\n");
}