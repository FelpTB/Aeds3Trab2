#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    float peso;
    float beneficio;
} type_item;

// Algoritmo de Força Bruta, recebe como parâmetros a capacidade da mochila (W), o peso e valor de cada item (type_items) e a quantidade de items(n).
float FB(float W, type_item *items, int n) {
    int i, j;
    float max_beneficio = 0.0;
    for (i = 0; i < (1 << n); i++) {
        float peso_atual = 0.0, beneficio_atual = 0.0;
        for (j = 0; j < n; j++) {
            if (i & (1 << j)) {
                peso_atual += items[j].peso;
                beneficio_atual += items[j].beneficio;
            }
        }
        if (peso_atual <= W && beneficio_atual > max_beneficio) {
            max_beneficio = beneficio_atual;
        }
    }
    return max_beneficio;
}


// Algoritmo de Programação Dinâmica, recebe como parâmetros a capacidade da mochila (W), o peso e valor de cada item (type_items) e a quantidade de items(n).
float dinamica(int W, type_item *items, int n) {
    int i, w;
    float K[n + 1][W + 1];
    for (i = 0; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                K[i][w] = 0;
            }
            else if (items[i - 1].peso <= w) {
                float val;
                if (w - items[i - 1].peso < 0) {
                    val = 0;
                } else {
                val = items[i - 1].beneficio + K[i - 1][(int)(w - items[i - 1].peso)];

                }
                K[i][w] = (val > K[i - 1][w]) ? val : K[i - 1][w];
            }
            else {
                K[i][w] = K[i - 1][w];
            }
        }
    }
    return K[n][W];
}

// Algoritmo Guloso, recebe como parâmetros a capacidade da mochila (W), o peso e valor de cada item (type_items) e a quantidade de items(n).
float guloso(float W, type_item *items, int n) {
    int i, j;
    float ratio[n];
    for (i = 0; i < n; i++) {
        ratio[i] = items[i].beneficio / items[i].peso;
    }
    int index[n];
    for (i = 0; i < n; i++) {
        index[i] = i;
    }
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (ratio[index[i]] < ratio[index[j]]) {
                int temp = index[i];
                index[i] = index[j];
                index[j] = temp;
            }
        }
    }
    float max_beneficio = 0.0;
    float peso_atual = 0.0;
    for (i = 0; i < n; i++) {
        if (peso_atual + items[index[i]].peso <= W) {
            max_beneficio += items[index[i]].beneficio;
            peso_atual += items[index[i]].peso;
        }
    }
    return max_beneficio;
}

// Printa o peso e valor de cada item na tela do terminal.
void imprimir_items(type_item *items, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("Peso: %.2f, Beneficio: %.2f\n", items[i].peso, items[i].beneficio);
    }
}

// Abre o arquivo e lê a capacidade da mochila (capacidad), o peso e valor de cada item (items) e a quantidade de items(n_items).
type_item *ler_items(char *filename, int *n_items, float *capacidad) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        puts("Não foi possível abrir o arquivo");
        exit(0);
    }

    fscanf(fp, "%d %f", n_items, capacidad);
    printf("Número de elementos: %d, Capacidade: %.2f\n", *n_items, *capacidad);

    type_item *items = (type_item *)malloc(*n_items * sizeof(type_item));

    for (int i = 0; i < *n_items; i++) {
        fscanf(fp, "%f %f", &items[i].peso, &items[i].beneficio);
    }

    fclose(fp);

    return items;
}

int main() {

    //Cria as variaveis para armazenar a capacidade da mochila e o número de items
    int n_items;
    float capacidad;
  
    //Chamada da fução ara ler o arquivo. Obs: O campo "entrada.txt" deve ser substutuido pelo nome do arquivo.
    type_item *items = ler_items("/home/felipe/Documents/Trab2Aeds/pasta/teste6.txt", &n_items, &capacidad);
    imprimir_items(items, n_items);

    //Algoritmo de escolha simples, para que o usuário possa definir o algoritmo que será usado
    printf("Escolha um algoritmo entre 1 e 3\n");
    int x = 3;
    // scanf("%d",&x);

    if (x == 1) {
        printf("A combinação máxima de valores de acordo com o algoritmo de Força Bruta é: %.2f\n", FB(capacidad ,items, n_items));
    } else if (x == 2) {
        printf("A combinação máxima de valores de acordo com o algoritmo Guloso é: %.2f\n", guloso(capacidad ,items, n_items));
    } else if (x == 3) {
        printf("A combinação máxima de valores de acordo com o algoritmo de Programação Dinâmica é: %.2f\n", dinamica(capacidad ,items, n_items));
    } 

    free(items);
    return 0;
}