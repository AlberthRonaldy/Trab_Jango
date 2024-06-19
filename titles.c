#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie {
    char *tconst;
    char *titleType;
    char *primaryTitle;
    char *originalTitle;
    int isAdult;
    int startYear;
    char *endYear;
    char *runtimeMinutes;
    char *genres;
    struct movie *next;  // Próximo filme na lista de adjacências
};

struct adjacency_list {
    struct movie **nodes;  // Array de nós da lista de adjacências
    size_t size;
    size_t capacity;
};

// Função de hashing para mapear o tconst do filme para um índice na lista de adjacências
size_t hash_function(const char *key, size_t size) {
    size_t hash = 0;
    size_t len = strlen(key);
    for (size_t i = 0; i < len; i++) {
        hash += key[i];
    }
    return hash % size;
}

void init_adjacency_list(struct adjacency_list *adj_list, size_t initial_capacity) {
    adj_list->nodes = malloc(initial_capacity * sizeof(struct movie *));
    if (adj_list->nodes == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    adj_list->size = 0;
    adj_list->capacity = initial_capacity;

    for (size_t i = 0; i < initial_capacity; i++) {
        adj_list->nodes[i] = NULL;
    }
}

// Função para redimensionar a lista de adjacências de filmes
void resize_adjacency_list(struct adjacency_list *adj_list) {
    size_t new_capacity = adj_list->capacity * 2;
    struct movie **new_nodes = realloc(adj_list->nodes, new_capacity * sizeof(struct movie *));
    if (new_nodes == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    adj_list->nodes = new_nodes;
    adj_list->capacity = new_capacity;

    // Inicializa novos slots com NULL
    for (size_t i = adj_list->size; i < new_capacity; i++) {
        adj_list->nodes[i] = NULL;
    }
}

// Função para adicionar um filme à lista de adjacências
void add_movie_to_adj_list(struct adjacency_list *adj_list, struct movie *new_movie) {
    if (adj_list->size == adj_list->capacity) {
        resize_adjacency_list(adj_list);
    }

    size_t index = hash_function(new_movie->tconst, adj_list->capacity);
    new_movie->next = adj_list->nodes[index];
    adj_list->nodes[index] = new_movie;
    adj_list->size++;
}

// Função para liberar a memória alocada
void free_adjacency_list(struct adjacency_list *adj_list) {
    for (size_t i = 0; i < adj_list->capacity; i++) {
        struct movie *current = adj_list->nodes[i];
        while (current != NULL) {
            struct movie *temp = current;
            current = current->next;
            free(temp->tconst);
            free(temp->titleType);
            free(temp->primaryTitle);
            free(temp->originalTitle);
            free(temp->endYear);
            free(temp->runtimeMinutes);
            free(temp->genres);
            free(temp);
        }
    }
    free(adj_list->nodes);
}

// Função para ler e mostrar os filmes
void read_and_print_movies(const char *filename, struct adjacency_list *adj_list) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        printf("Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), file);  // Skip header line

    int count = 0;
    while (fgets(line, sizeof(line), file) && count < 10) {
        char *tconst = strtok(line, "\t");
        char *titleType = strtok(NULL, "\t");
        char *primaryTitle = strtok(NULL, "\t");
        char *originalTitle = strtok(NULL, "\t");
        int isAdult = atoi(strtok(NULL, "\t"));
        int startYear = atoi(strtok(NULL, "\t"));
        char *endYear = strtok(NULL, "\t");
        char *runtimeMinutes = strtok(NULL, "\t");
        char *genres = strtok(NULL, "\t");

        // Cria um novo filme
        struct movie *new_movie = malloc(sizeof(struct movie));
        if (new_movie == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        new_movie->tconst = strdup(tconst);
        new_movie->titleType = strdup(titleType);
        new_movie->primaryTitle = strdup(primaryTitle);
        new_movie->originalTitle = strdup(originalTitle);
        new_movie->isAdult = isAdult;
        new_movie->startYear = startYear;
        new_movie->endYear = strdup(endYear);
        new_movie->runtimeMinutes = strdup(runtimeMinutes);
        new_movie->genres = strdup(genres);
        new_movie->next = NULL;

        add_movie_to_adj_list(adj_list, new_movie);

        printf("tconst: %s\n", new_movie->tconst);
        printf("titleType: %s\n", new_movie->titleType);
        printf("primaryTitle: %s\n", new_movie->primaryTitle);
        printf("originalTitle: %s\n", new_movie->originalTitle);
        printf("isAdult: %d\n", new_movie->isAdult);
        printf("startYear: %d\n", new_movie->startYear);
        printf("endYear: %s\n", new_movie->endYear);
        printf("runtimeMinutes: %s\n", new_movie->runtimeMinutes);
        printf("genres: %s\n", new_movie->genres);
        printf("\n");

        count++;
    }

    fclose(file);
}

int main() {
    const char *filename = "arquivos/datasets/title.basics.tsv";
    printf("Attempting to open file: %s\n", filename);

    struct adjacency_list movies_adj_list;
    init_adjacency_list(&movies_adj_list, 100);  // Inicializa com capacidade de 100

    read_and_print_movies(filename, &movies_adj_list);

    free_adjacency_list(&movies_adj_list);

    return 0;
}
