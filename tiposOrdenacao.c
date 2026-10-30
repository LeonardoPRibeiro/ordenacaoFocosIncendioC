#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tiposOrdenacao.h"

// Função para ler o CSV de entrada com alocação dinâmica segura
FocoIncendio* ler_csv(const char* nome_arquivo, int* total_registros) {
    FILE* file = fopen(nome_arquivo, "r");
    if (!file) {
        return NULL; // Tratado na main com perror
    }

    char linha[MAX_LINE_LENGTH];
    if (!fgets(linha, sizeof(linha), file)) {
        fclose(file);
        return NULL;
    }

    int capacidade = 1000;
    int qtd = 0;
    FocoIncendio* focos = malloc(capacidade * sizeof(FocoIncendio));
    if (!focos) {
        fclose(file);
        return NULL;
    }

    while (fgets(linha, sizeof(linha), file)) {
        linha[strcspn(linha, "\r\n")] = 0;
        if (strlen(linha) == 0) continue;

        if (qtd >= capacidade) {
            capacidade *= 2;
            FocoIncendio* temp = realloc(focos, capacidade * sizeof(FocoIncendio));
            if (!temp) {
                free(focos);
                fclose(file);
                return NULL;
            }
            focos = temp;
        }

        char* token = strtok(linha, ",");
        if (!token) continue;
        strcpy(focos[qtd].id_bdq, token);

        token = strtok(NULL, ",");
        if (token) strcpy(focos[qtd].foco_id, token);

        token = strtok(NULL, ",");
        focos[qtd].lat = token ? atof(token) : 0.0;

        token = strtok(NULL, ",");
        focos[qtd].lon = token ? atof(token) : 0.0;

        token = strtok(NULL, ",");
        if (token) strcpy(focos[qtd].data_pas, token);

        token = strtok(NULL, ",");
        if (token) strcpy(focos[qtd].pais, token);

        token = strtok(NULL, ",");
        if (token) strcpy(focos[qtd].estado, token);

        token = strtok(NULL, ",");
        if (token) strcpy(focos[qtd].municipio, token);

        token = strtok(NULL, ",");
        if (token) strcpy(focos[qtd].bioma, token);

        qtd++;
    }

    fclose(file);
    *total_registros = qtd;
    return focos;
}

// Função para exportar os dados ordenados para um novo CSV
void escrever_csv(const char* nome_arquivo, FocoIncendio* focos, int total_registros) {
    FILE* file = fopen(nome_arquivo, "w");
    if (!file) {
        perror("[ERRO CRITICO] Nao foi possivel criar o arquivo de saida");
        return;
    }

    fprintf(file, "id_bdq,foco_id,lat,lon,data_pas,pais,estado,municipio,bioma\n");

    for (int i = 0; i < total_registros; i++) {
        fprintf(file, "%s,%s,%.6f,%.6f,%s,%s,%s,%s,%s\n",
            focos[i].id_bdq,
            focos[i].foco_id,
            focos[i].lat,
            focos[i].lon,
            focos[i].data_pas,
            focos[i].pais,
            focos[i].estado,
            focos[i].municipio,
            focos[i].bioma
        );
    }

    fclose(file);
}

// Função de comparação entre dois registros conforme o critério
int comparar(FocoIncendio a, FocoIncendio b, int criterio, MetricasOrdenacao* m) {
    m->comparacoes++;
    switch (criterio) {
        case 1: return strcmp(a.data_pas, b.data_pas);
        case 2: return strcmp(a.bioma, b.bioma);
        case 3: return strcmp(a.municipio, b.municipio);
        default: return strcmp(a.data_pas, b.data_pas);
    }
}

// --- 1. MERGE SORT ---
void merge(FocoIncendio* arr, int esquerda, int meio, int direita, int criterio, MetricasOrdenacao* m) {
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;

    FocoIncendio* L = malloc(n1 * sizeof(FocoIncendio));
    FocoIncendio* R = malloc(n2 * sizeof(FocoIncendio));
    if (!L || !R) {
        perror("[ERRO] Falha de alocacao de memoria no Merge");
        exit(1);
    }

    for (int i = 0; i < n1; i++) { L[i] = arr[esquerda + i]; m->trocas++; }
    for (int j = 0; j < n2; j++) { R[j] = arr[meio + 1 + j]; m->trocas++; }

    int i = 0, j = 0, k = esquerda;
    while (i < n1 && j < n2) {
        if (comparar(L[i], R[j], criterio, m) <= 0) {
            arr[k] = L[i++];
        } else {
            arr[k] = R[j++];
        }
        m->trocas++;
        k++;
    }
    while (i < n1) { arr[k++] = L[i++]; m->trocas++; }
    while (j < n2) { arr[k++] = R[j++]; m->trocas++; }

    free(L);
    free(R);
}

void ordenar_merge(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m) {
    if (esquerda < direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        ordenar_merge(arr, esquerda, meio, criterio, m);
        ordenar_merge(arr, meio + 1, direita, criterio, m);
        merge(arr, esquerda, meio, direita, criterio, m);
    }
}

// --- 2. INSERTION SORT ---
void ordenar_insertion(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m) {
    for (int i = esquerda + 1; i <= direita; i++) {
        FocoIncendio chave = arr[i];
        m->trocas++;
        int j = i - 1;
        while (j >= esquerda && comparar(arr[j], chave, criterio, m) > 0) {
            arr[j + 1] = arr[j];
            m->trocas++;
            j--;
        }
        arr[j + 1] = chave;
        m->trocas++;
    }
}

// --- 3. QUICK SORT ---
int particionar(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m) {
    FocoIncendio pivo = arr[direita];
    m->trocas++;
    int i = (esquerda - 1);

    for (int j = esquerda; j <= direita - 1; j++) {
        if (comparar(arr[j], pivo, criterio, m) <= 0) {
            i++;
            FocoIncendio temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            m->trocas += 3;
        }
    }
    FocoIncendio temp = arr[i + 1];
    arr[i + 1] = arr[direita];
    arr[direita] = temp;
    m->trocas += 3;

    return (i + 1);
}

void ordenar_quick(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m) {
    if (esquerda < direita) {
        int pi = particionar(arr, esquerda, direita, criterio, m);
        ordenar_quick(arr, esquerda, pi - 1, criterio, m);
        ordenar_quick(arr, pi + 1, direita, criterio, m);
    }
}

// --- 4. SELECTION SORT ---
void ordenar_selection(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m) {
    for (int i = esquerda; i < direita; i++) {
        int min_idx = i;
        for (int j = i + 1; j <= direita; j++) {
            if (comparar(arr[j], arr[min_idx], criterio, m) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            FocoIncendio temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
            m->trocas += 3;
        }
    }
}

// --- 5. BUBBLE SORT ---
void ordenar_bubble(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m) {
    int n = direita - esquerda + 1;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = esquerda; j <= direita - i - 1; j++) {
            if (comparar(arr[j], arr[j + 1], criterio, m) > 0) {
                FocoIncendio temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                m->trocas += 3;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

// --- 6. SHELL SORT ---
void ordenar_shell(FocoIncendio* arr, int esquerda, int direita, int criterio, MetricasOrdenacao* m) {
    int n = direita - esquerda + 1;
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = esquerda + gap; i <= direita; i++) {
            FocoIncendio temp = arr[i];
            m->trocas++;
            int j;
            for (j = i; j >= esquerda + gap && comparar(arr[j - gap], temp, criterio, m) > 0; j -= gap) {
                arr[j] = arr[j - gap];
                m->trocas++;
            }
            arr[j] = temp;
            m->trocas++;
        }
    }
}