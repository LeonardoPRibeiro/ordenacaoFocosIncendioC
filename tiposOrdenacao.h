#ifndef TIPOS_ORDENACAO_H
#define TIPOS_ORDENACAO_H

#define MAX_LINE_LENGTH 1024

// Estrutura para representar um registro de foco de incêndio
typedef struct {
    char id_bdq[32];
    char foco_id[48];
    double lat;
    double lon;
    char data_pas[32];
    char pais[32];
    char estado[32];
    char municipio[64];
    char bioma[64];
} FocoIncendio;

// Contadores de operações para análise de performance
typedef struct {
    long long comparacoes;
    long long trocas;
} MetricasOrdenacao;

// Protótipos das funções de manipulação de arquivos e comparações
FocoIncendio* ler_csv(const char* nome_arquivo, int* total_registros);
void escrever_csv(const char* nome_arquivo, FocoIncendio* focos, int total_registros);
int comparar(FocoIncendio a, FocoIncendio b, int criterio, MetricasOrdenacao* m);

// Protótipos dos Algoritmos de Ordenação
void ordenar_merge(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m);
void ordenar_insertion(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m);
void ordenar_quick(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m);
void ordenar_selection(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m);
void ordenar_bubble(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m);
void ordenar_shell(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m);

#endif