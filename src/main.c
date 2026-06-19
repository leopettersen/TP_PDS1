/* main.c — Ponto de entrada do sistema. */
#include "func/funcoes.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    /* Armazenamento em memória: vetor fixo com até 100 estações cadastradas. */
    struct Estacao estacoes[100];
    int total = 0;   /* quantidade de estações já cadastradas no vetor */
    int opcao;       /* opção escolhida pelo usuário no menu */

    /* Loop principal do sistema:
     *   1) exibe o menu;
     *   2) lê a opção do usuário;
     *   3) despacha a ação correspondente no switch;
     *   4) encerra quando opcao == 0.
     */
    do
    {
        exibirMenu();

        /* Se a entrada não for numérica, descarta a linha para evitar loop infinito. */
        if (scanf("%d", &opcao) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            opcao = -1;
        }

        switch (opcao)
        {
            case 1: /* Adicionar estação */
                adicionarEstacao(estacoes, &total);
                break;

            case 2: /* Listar estações */
                listarEstacoes(estacoes, total);
                break;

            case 3: /* Buscar por operador */
                buscarPorOperador(estacoes, total);
                break;

            case 4: /* Editar estação */
                editarEstacao(estacoes, total);
                break;

            case 5: /* Remover estação */
                removerEstacao(estacoes, &total);
                break;

            case 6: /* Detectar anomalias */
                detectarAnomalias(estacoes, total);
                break;

            case 7: /* Salvar CSV */
                salvarCSV(estacoes, total, "estacoes.csv");
                break;

            case 8: /* Carregar CSV */
                printf("Em construção\n");
                break;

            case 0:
                printf("Encerrando o sistema.\n");
                break;

            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
