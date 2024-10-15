#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>

void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
int_fast8_t** create_graph(int size);
void print_graph(int size, int_fast8_t** graph_pointer);
void graph_info(int size, int_fast8_t** graph_pointer);
void print_inc_matr(int size, int_fast8_t** graph_pointer);

int main() {
    setlocale(LC_ALL, "Russian");
    int_fast8_t** graph;
    int size;
    float edge_probability;

    fprintf(stdout, "Введите количество вершин графа: ");
    if (!scanf("%d", &size) || size <= 1) {
        fprintf(stderr, "Ошибка ввода размера массива");
        exit(1);
    }

    fprintf(stdout, "Введите вероятность создания ребра (от 0 до 100): ");
    if (!scanf("%f", &edge_probability) || edge_probability < 0 || edge_probability > 100) {
        fprintf(stderr, "Ошибка ввода вероятности");
        exit(1);
    }

    graph = create_graph(size);
    initialize_graph(size, graph, edge_probability);
    printf("Матрица смежности: \n\n");
    print_graph(size, graph);
    graph_info(size, graph);

    printf("\nМатрица инцидентности: \n\n");
    print_inc_matr(size, graph);

    for (int i = 0; i < size; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}

int_fast8_t** create_graph(int size) {
    int_fast8_t** array = (int_fast8_t**)malloc(sizeof(int_fast8_t*) * size);
    for (int i = 0; i < size; i++) {
        array[i] = (int_fast8_t*)malloc(sizeof(int_fast8_t) * size);
    }
    if (array == NULL) {
        fprintf(stderr, "Ошибка создания массива");
        exit(1);
    }
    return array;
}

void initialize_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
                graph_pointer[i][j] = 1;
                graph_pointer[j][i] = 1;
            }
            else {
                graph_pointer[i][j] = 0;
                graph_pointer[j][i] = 0;
            }
        }
        graph_pointer[i][i] = 0;
    }
}

void print_graph(int size, int_fast8_t** graph_pointer) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(stdout, "%d ", graph_pointer[j][i]);
        }
        fprintf(stdout, "\n");
    }
}

void graph_info(int size, int_fast8_t** graph_pointer) {
    int edge_count = 0;
    int* isolated_vertices = (int*)calloc(size, sizeof(int));
    int* leaf_vertices = (int*)calloc(size, sizeof(int));
    int* dominating_vertices = (int*)calloc(size, sizeof(int));
    int isolated_count = 0, leaf_count = 0, dominating_count = 0;

    for (int i = 0; i < size; i++) {
        int degree = 0;
        int is_isolated = 1;
        int is_dominating = 1;
        for (int j = 0; j < size; j++) {
            if (graph_pointer[i][j] == 1) {
                is_isolated = 0;
                degree++;
                if (i < j) {
                    edge_count++;
                }
            }
            else if (i != j) {
                is_dominating = 0;
            }
        }
        if (is_isolated) {
            isolated_vertices[isolated_count++] = i + 1;
        }
        else if (degree == 1) {
            leaf_vertices[leaf_count++] = i + 1;
        }
        if (is_dominating) {
            dominating_vertices[dominating_count++] = i + 1;
        }
    }

    printf("\nРазмер графа: %d\n", edge_count);

    printf("\nИзолированные вершины:\n");
    if (isolated_count == 0) {
        printf("Изолированных вершин нет\n");
    }
    else {
        for (int i = 0; i < isolated_count; i++) {
            printf("%d ", isolated_vertices[i]);
        }
        printf("\n");
    }

    printf("\nКонцевые вершины:\n");
    if (leaf_count == 0) {
        printf("Концевых вершин нет\n");
    }
    else {
        for (int i = 0; i < leaf_count; i++) {
            printf("%d ", leaf_vertices[i]);
        }
        printf("\n");
    }

    printf("\nДоминирующие вершины:\n");
    if (dominating_count == 0) {
        printf("Доминирующих вершин нет\n");
    }
    else {
        for (int i = 0; i < dominating_count; i++) {
            printf("%d ", dominating_vertices[i]);
        }
        printf("\n");
    }

    free(isolated_vertices);
    free(leaf_vertices);
    free(dominating_vertices);
}
void print_inc_matr(int size, int_fast8_t** graph_pointer) {
    int edge_count = 0;

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (graph_pointer[i][j] == 1) {
                edge_count++;
            }
        }
    }

    int** inc_matr = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        inc_matr[i] = (int*)calloc(edge_count, sizeof(int));
    }

    int curr_edge = 0;
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (graph_pointer[i][j] == 1) {
                inc_matr[i][curr_edge] = 1;
                inc_matr[j][curr_edge] = 1;
                curr_edge++;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < edge_count; j++) {
            printf("%d ", inc_matr[i][j]);
        }
        printf("\n");
    }

    free(inc_matr);
}