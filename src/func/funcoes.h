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

#endif