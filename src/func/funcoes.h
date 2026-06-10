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

#endif