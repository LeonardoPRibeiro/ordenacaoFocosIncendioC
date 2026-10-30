#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h> 
#include "tiposOrdenacao.h"

int main() {
    setlocale(LC_ALL, "Portuguese");
    int doWhile = 1;

    while (doWhile == 1) {
        char arquivo_entrada[256];
        char arquivo_saida[256];
        int criterio = 0;
        int tipo_ordenacao = 0;

        printf("\n============================================================\n");
        printf("=== SISTEMA DE CONVERSAO E ORDENACAO DE FOCOS DE INCENDIO ===\n");
        printf("============================================================\n");

        // 1. Entrada do Arquivo de Entrada
        printf("Digite o nome do arquivo de entrada (ex: focos_br_sp_ref_2024): ");
        if (scanf("%255s", arquivo_entrada) != 1) {
            printf("[ERRO] Falha na leitura do nome do arquivo.\n");
            while (getchar() != '\n' && getchar() != EOF);
            continue;
        }
        strcat(arquivo_entrada, ".csv");

        // 2. Entrada do Arquivo de Saída
        printf("Digite o nome do arquivo de saida (ex: teste2): ");
        if (scanf("%255s", arquivo_saida) != 1) {
            printf("[ERRO] Falha na leitura do nome do arquivo de saida.\n");
            while (getchar() != '\n' && getchar() != EOF);
            continue;
        }
        strcat(arquivo_saida, ".csv");

        // 3. Escolha do Critério com Validação Profissional
        do {
            printf("\nEscolha o criterio de ordenacao:\n");
            printf("1 - Data da Passagem (data_pas)\n");
            printf("2 - Bioma (bioma)\n");
            printf("3 - Municipio (municipio)\n");
            printf("Opcao: ");
            if (scanf("%d", &criterio) != 1) {
                printf("[ERRO] Entrada invalida. Digite um numero entre 1 e 3.\n");
                while (getchar() != '\n' && getchar() != EOF);
                criterio = 0;
            } else if (criterio < 1 || criterio > 3) {
                printf("[AVISO] Opcao fora do intervalo. Tente novamente.\n");
            }
        } while (criterio < 1 || criterio > 3);

        // 4. Escolha do Algoritmo de Ordenação com Validação
        do {
            printf("\nEscolha o tipo de ordenacao:\n");
            printf("1 - Merge Sort\n");
            printf("2 - Insertion Sort\n");
            printf("3 - Quick Sort\n");
            printf("4 - Selection Sort\n");
            printf("5 - Bubble Sort\n");
            printf("6 - Shell Sort\n");
            printf("Opcao: ");
            if (scanf("%d", &tipo_ordenacao) != 1) {
                printf("[ERRO] Entrada invalida. Digite um numero entre 1 e 6.\n");
                while (getchar() != '\n' && getchar() != EOF);
                tipo_ordenacao = 0;
            } else if (tipo_ordenacao < 1 || tipo_ordenacao > 6) {
                printf("[AVISO] Opcao de ordenacao invalida. Tente novamente.\n");
            }
        } while (tipo_ordenacao < 1 || tipo_ordenacao > 6);

        // 5. Carregamento dos Dados com Tratamento de Erro de Arquivo
        int total_registros = 0;
        FocoIncendio* focos = ler_csv(arquivo_entrada, &total_registros);
        
        if (focos == NULL || total_registros <= 0) {
            printf("[ERRO CRITICO] Nao foi possivel carregar o arquivo '%s'. Verifique se o nome esta correto e se o arquivo existe.\n", arquivo_entrada);
            printf("Deseja tentar novamente?\nPara continuar digite - 1 | Para finalizar digite - 2\nOpcao: ");
            if (scanf("%d", &doWhile) != 1) doWhile = 2;
            while (getchar() != '\n' && getchar() != EOF);
            continue;
        }

        printf("\n[INFO] Arquivo '%s' carregado com sucesso. Total de registros: %d\n", arquivo_entrada, total_registros);

        MetricasOrdenacao metricas = {0, 0};

        // --- 6. INÍCIO DA MEDIÇÃO DE TEMPO ---
        clock_t inicio = clock();

        // Execução do Algoritmo Escolhido com Mensagem Informativa
        switch (tipo_ordenacao) {
            case 1:
                printf("[INFO] Executando ordenacao via Merge Sort...\n");
                ordenar_merge(focos, 0, total_registros - 1, criterio, &metricas);
                break;
            case 2:
                printf("[INFO] Executando ordenacao via Insertion Sort...\n");
                ordenar_insertion(focos, 0, total_registros - 1, criterio, &metricas);
                break;
            case 3:
                printf("[INFO] Executando ordenacao via Quick Sort...\n");
                ordenar_quick(focos, 0, total_registros - 1, criterio, &metricas);
                break;
            case 4:
                printf("[INFO] Executando ordenacao via Selection Sort...\n");
                ordenar_selection(focos, 0, total_registros - 1, criterio, &metricas);
                break;
            case 5:
                printf("[INFO] Executando ordenacao via Bubble Sort...\n");
                ordenar_bubble(focos, 0, total_registros - 1, criterio, &metricas);
                break;
            case 6:
                printf("[INFO] Executando ordenacao via Shell Sort...\n");
                ordenar_shell(focos, 0, total_registros - 1, criterio, &metricas);
                break;
        }

        // FIM DA MEDIÇÃO DE TEMPO
        clock_t fim = clock();
        double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
        // -------------------------------------

        // 7. Exibição de Resultados e Geração do Arquivo de Saída
       // 7. Exibição de Resultados e Geração do Arquivo de Saída
        printf("\n============================================================\n");
        printf("               RESULTADOS DA ANALISE DE PERFORMANCE         \n");
        printf("============================================================\n");
        printf("Total de registros processados: %d\n", total_registros); // <--- Adicionado aqui!
        printf("Criterio escolhido: %s\n", criterio == 1 ? "Data da Passagem" : (criterio == 2 ? "Bioma" : "Municipio"));
        printf("Total de Comparacoes realizadas: %lld\n", metricas.comparacoes);
        printf("Total de Movimentacoes/Atribuicoes: %lld\n", metricas.trocas);
        printf("Tempo de execucao: %.4f segundos (%.2f ms)\n", tempo_gasto, tempo_gasto * 1000.0);
        printf("============================================================\n");

        escrever_csv(arquivo_saida, focos, total_registros);
        printf("[SUCESSO] Arquivo ordenado gerado em: '%s'\n", arquivo_saida);

        escrever_csv(arquivo_saida, focos, total_registros);
        printf("[SUCESSO] Arquivo ordenado gerado em: '%s'\n", arquivo_saida);

        // 8. Liberação Segura de Memória
        free(focos);
        printf("[INFO] Memoria liberada com sucesso.\n");

        // 9. Opção de Repetição do Programa com Validação
        printf("\nDeseja continuar o programa?\nPara continuar digite - 1 | Para finalizar digite - 2\nOpcao: ");
        if (scanf("%d", &doWhile) != 1) {
            doWhile = 2; // Encerra por segurança caso o usuário digite texto incorreto
        }
        while (getchar() != '\n' && getchar() != EOF); // Limpa buffer residual
    }

    printf("\nPrograma Finalizado com sucesso!\n");
    return 0;
}