#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>

void initialize_graph(int vertices, int edges, int_fast8_t** graph_pointer, float edge_probability);
int_fast8_t** create_graph(int vertices, int edges);
void print_graph(int vertices, int edges, int_fast8_t** graph_pointer);
void graph_info(int vertices, int edges, int_fast8_t** graph_pointer);

int main() {
    setlocale(LC_ALL, "Russian");
    int_fast8_t** graph;
    int vertices, edges;
    float edge_probability;

    fprintf(stdout, "Введите количество вершин графа: ");
    if (!scanf("%d", &vertices) || vertices <= 1) {
        fprintf(stderr, "Ошибка ввода количества вершин");
        exit(1);
    }

    fprintf(stdout, "Введите вероятность создания ребра (от 0 до 100): ");
    if (!scanf("%f", &edge_probability) || edge_probability < 0 || edge_probability > 100) {
        fprintf(stderr, "Ошибка ввода вероятности");
        exit(1);
    }

    edges = vertices * (vertices - 1) / 2;
    graph = create_graph(vertices, edges);
    initialize_graph(vertices, edges, graph, edge_probability);
    printf("Матрица инцидентности: \n\n");
    print_graph(vertices, edges, graph);
    graph_info(vertices, edges, graph);

    for (int i = 0; i < vertices; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}

int_fast8_t** create_graph(int vertices, int edges) {
    int_fast8_t** array = (int_fast8_t**)malloc(sizeof(int_fast8_t*) * vertices);
    for (int i = 0; i < vertices; i++) {
        array[i] = (int_fast8_t*)calloc(edges, sizeof(int_fast8_t));
    }
    if (array == NULL) {
        fprintf(stderr, "Ошибка создания массива");
        exit(1);
    }
    return array;
}

void initialize_graph(int vertices, int edges, int_fast8_t** graph_pointer, float edge_probability) {
    srand((unsigned int)time(NULL));
    int edge_count = 0;
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            if ((float)rand() / (RAND_MAX / 100) < edge_probability && edge_count < edges) {
                graph_pointer[i][edge_count] = 1;
                graph_pointer[j][edge_count] = 1;
                edge_count++;
            }
        }
    }
}

void print_graph(int vertices, int edges, int_fast8_t** graph_pointer) {
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < edges; j++) {
            fprintf(stdout, "%d ", graph_pointer[i][j]);
        }
        fprintf(stdout, "\n");
    }
}

void graph_info(int vertices, int edges, int_fast8_t** graph_pointer) {
    int edge_count = 0;
    int* isolated_vertices = (int*)calloc(vertices, sizeof(int));
    int* leaf_vertices = (int*)calloc(vertices, sizeof(int));
    int* dominating_vertices = (int*)calloc(vertices, sizeof(int));
    int isolated_count = 0, leaf_count = 0, dominating_count = 0;

    for (int i = 0; i < vertices; i++) {
        int degree = 0;
        int is_isolated = 1;
        for (int j = 0; j < edges; j++) {
            if (graph_pointer[i][j] == 1) {
                is_isolated = 0;
                degree++;
                edge_count++;
            }
        }
        if (is_isolated) {
            isolated_vertices[isolated_count++] = i + 1;
        }
        else if (degree == 1) {
            leaf_vertices[leaf_count++] = i + 1;
        }
        if (degree == vertices - 1) {
            dominating_vertices[dominating_count++] = i + 1;
        }
    }

    edge_count /= 2;

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