#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef FUNCOES_H
#define FUNCOES_H

struct DataLeitura
{
    int dia;
    int mes;
    int ano;
};

struct Estacao
{
    int id;
    char nome[40];
    char operador[40];
    char sensor[20];
    int n;
    float *leituras;
    float media;
    float variancia;
    float desvioPadrao;
    struct DataLeitura data;
};

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