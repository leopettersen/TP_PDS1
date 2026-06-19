/* funcoes.h — Declarações de tipos e protótipos do sistema. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef FUNCOES_H
#define FUNCOES_H

struct DataLeitura
{
    int dia; /* dia do mês da leitura (1 a 31) */
    int mes; /* mês da leitura (1 a 12) */
    int ano; /* ano da leitura (1900 a 2100) */
};

struct Estacao
{
    int id;                  /* identificador único da estação */
    char nome[40];           /* nome da estação */
    char operador[40];       /* nome do operador responsável */
    char sensor[20];         /* identificação/modelo do sensor */
    int n;                   /* quantidade de leituras coletadas */
    float *leituras;         /* vetor dinâmico com as n leituras */
    float media;             /* média das leituras */
    float variancia;         /* variância das leituras */
    float desvioPadrao;      /* desvio padrão das leituras */
    struct DataLeitura data; /* data em que as leituras foram realizadas */
};

/**
 * @brief exibe o menu principal do sistema.
 *
 * Imprime as 9 opções disponíveis (1 a 8 para funcionalidades e 0 para sair)
 * e o prompt para o usuário digitar a escolha.
 */
void exibirMenu(void);

/**
 * @brief aloca a memória dinamicamente para n floats.
 * 
 * Esta função aloca memória dinamicamente, utilizando malloc para um número n de números flutuantes.
 * 
 * @param n número de floats que serão alocados (int).
 * @return *leituras: retorna um array de números flutuantes (float).
 */
float *lerLeituras(int n);

/**
 * @brief verifica se o ano é bissexto.
 * 
 * Esta função verifica se o ano presente na data informada é bissexto.
 * 
 * @param data data informada pelo usuário (struct DataLeitura).
 * @return se o ano é bissexto ou não (True or False).
 */
bool ano_bissexto(struct DataLeitura data);

/**
 * @brief verifica se a data informada é válida.
 * 
 * Esta função valida a data informada pelo usuário.
 * 
 * @param data data informada pelo usuário (struct DataLeitura).
 * @return se a data é válida ou não (True or False).
 * @note Condições: A data deve existir; 1900 ≤ ano ≤ 2100.
 */
bool valida_data(struct DataLeitura data);

/**
 * @brief calcula a média de n leituras recursivamente.
 *
 * FUNÇÃO RECURSIVA OBRIGATÓRIA do projeto: implementa uma soma recursiva
 * com caso base n == 0 e divide o resultado pela quantidade total para
 * obter a média aritmética.
 *
 * @param v vetor de leituras (float*).
 * @param n quantidade de elementos a considerar (int).
 * @return média aritmética dos n elementos (float).
 */
float calcularMediaRecursiva(float *v, int n);

/**
 * @brief calcula a variância das leituras.
 *
 * Variância populacional: soma dos quadrados dos desvios em relação à média,
 * dividida por n.
 *
 * @param v vetor de leituras (float*).
 * @param n quantidade de elementos (int).
 * @param media média previamente calculada (float).
 * @return variância dos n elementos (float).
 */
float calcularVariancia(float *v, int n, float media);

/**
 * @brief calcula o desvio padrão a partir da variância.
 *
 * Usa sqrt() de <math.h>.
 *
 * @param variancia variância previamente calculada (float).
 * @return desvio padrão (float).
 */
float calcularDesvioPadrao(float variancia);

/**
 * @brief preenche os campos estatísticos da estação.
 *
 * Calcula média (recursiva), variância e desvio padrão das leituras da
 * estação e grava nos campos media, variancia e desvioPadrao.
 *
 * @param e ponteiro para a estação (struct Estacao*).
 */
void calcularEstatisticas(struct Estacao *e);

/**
 * @brief adiciona uma nova estação ao vetor.
 *
 * Lê interativamente os campos da estação (id, nome, operador, sensor, data,
 * quantidade e leituras), valida data (via valida_data), intervalo de n
 * (1 a 9999) e unicidade do id. Aloca dinamicamente o vetor de leituras
 * via lerLeituras (que faz malloc(n*sizeof(float))) e verifica retorno NULL.
 * Após ler as leituras, chama calcularEstatisticas e insere ao final do vetor,
 * incrementando *total.
 *
 * @param estacoes vetor de estações já cadastradas (struct Estacao*).
 * @param total ponteiro para a quantidade atual de estações (int*).
 * @note Capacidade máxima de 100 estações; em caso de erro a inclusão é abortada.
 */
void adicionarEstacao(struct Estacao *estacoes, int *total);

/**
 * @brief lista todas as estações cadastradas.
 *
 * Imprime, para cada estação, todos os campos (id, nome, operador, sensor,
 * data formatada como dd/mm/aaaa, leituras e estatísticas). Caso não haja
 * estações cadastradas, exibe uma mensagem informando.
 *
 * @param estacoes vetor de estações cadastradas (struct Estacao*).
 * @param total quantidade de estações no vetor (int).
 */
void listarEstacoes(struct Estacao *estacoes, int total);

/**
 * @brief busca e lista estações de um operador específico.
 *
 * Pede o nome do operador ao usuário e percorre o vetor comparando com
 * strcmp. Imprime os dados das estações cujo campo operador coincide.
 * Avisa quando nenhuma estação é encontrada.
 *
 * @param estacoes vetor de estações cadastradas (struct Estacao*).
 * @param total quantidade de estações no vetor (int).
 */
void buscarPorOperador(struct Estacao *estacoes, int total);

/**
 * @brief edita os campos de uma estação existente.
 *
 * Pede o id e localiza a estação correspondente. Em caso de sucesso, abre
 * um submenu que permite alterar nome, operador, sensor ou data
 * (reaproveitando valida_data). Avisa se o id não existir.
 *
 * @param estacoes vetor de estações cadastradas (struct Estacao*).
 * @param total quantidade de estações no vetor (int).
 * @note Não altera o id, nem o vetor de leituras nem as estatísticas.
 */
void editarEstacao(struct Estacao *estacoes, int total);

/**
 * @brief remove uma estação do vetor pelo id.
 *
 * Pede o id, localiza a estação, libera (free) o vetor leituras dela para
 * evitar vazamento, desloca as estações seguintes uma posição para trás
 * e decrementa *total. Avisa se o id não existir.
 *
 * @param estacoes vetor de estações cadastradas (struct Estacao*).
 * @param total ponteiro para a quantidade atual de estações (int*).
 */
void removerEstacao(struct Estacao *estacoes, int *total);

/**
 * @brief detecta leituras anômalas (critério 2σ) em uma estação.
 *
 * Pede o id da estação e lista todas as leituras x cujo desvio absoluto em
 * relação à média ultrapasse duas vezes o desvio padrão: |x - media| > 2*σ.
 * Usa fabs (math.h). Avisa quando o id não existir ou quando não houver
 * nenhuma leitura anômala.
 *
 * @param estacoes vetor de estações cadastradas (struct Estacao*).
 * @param total quantidade de estações no vetor (int).
 */
void detectarAnomalias(struct Estacao *estacoes, int total);

/**
 * @brief grava todas as estações em arquivo CSV.
 *
 * Cabeçalho:
 *   ID,Nome,Operador,Sensor,Data,N,Media,Variancia,DesvioPadrao,Leituras
 * Data no formato dd/mm/aaaa. As leituras vão na última coluna separadas
 * por ';' (para não conflitar com o ',' do CSV). Verifica falha no fopen.
 *
 * @param estacoes vetor de estações cadastradas (struct Estacao*).
 * @param total quantidade de estações no vetor (int).
 * @param nomeArquivo caminho do arquivo CSV de saída (char*).
 */
void salvarCSV(struct Estacao *estacoes, int total, char *nomeArquivo);

/**
 * @brief carrega estações de um arquivo CSV.
 *
 * Abre o arquivo, descarta a primeira linha (cabeçalho) e processa cada
 * linha subsequente com fgets + strtok (separador ','). A última coluna
 * (leituras) é tokenizada com separador ';'. Aloca o vetor de leituras
 * com malloc e recalcula média, variância e desvio padrão via
 * calcularEstatisticas. Avisa se o arquivo não existir.
 *
 * @param estacoes vetor onde as estações serão acrescentadas (struct Estacao*).
 * @param total ponteiro para a quantidade atual; é incrementado a cada
 *              estação carregada (int*).
 * @param nomeArquivo caminho do arquivo CSV de entrada (char*).
 * @note As estações são APENDADAS ao vetor existente (não substituem).
 */
void carregarCSV(struct Estacao *estacoes, int *total, char *nomeArquivo);

/**
 * @brief libera toda a memória dinâmica das estações.
 *
 * Percorre o vetor e chama free() no campo leituras de cada estação,
 * devolvendo ao heap os blocos alocados por malloc em adicionarEstacao
 * e carregarCSV. Deve ser chamada antes do programa encerrar para evitar
 * vazamento de memória.
 *
 * @param estacoes vetor de estações cadastradas (struct Estacao*).
 * @param total quantidade de estações no vetor (int).
 */
void liberarTudo(struct Estacao *estacoes, int total);

#endif