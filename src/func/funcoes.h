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

#endif