#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um filme
struct movie {
    char *tconst;
    char *primaryTitle;
    struct movie *next;  // Próximo filme na lista encadeada
};

// Estrutura para representar um ator
struct actor {
    char *nconst;
    char *name;
    struct movie *movies;  // Lista encadeada de filmes de destaque
};

// Função para criar um novo filme
struct movie *create_movie(const char *tconst, const char *primaryTitle) {
    struct movie *new_movie = malloc(sizeof(struct movie));
    if (new_movie == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_movie->tconst = strdup(tconst);
    new_movie->primaryTitle = strdup(primaryTitle);
    if (new_movie->tconst == NULL || new_movie->primaryTitle == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_movie->next = NULL;
    return new_movie;
}

// Função para adicionar um filme à lista de filmes de destaque de um ator
void add_movie_to_actor(struct actor *actor, struct movie *new_movie) {
    new_movie->next = actor->movies;
    actor->movies = new_movie;
}

// Função para liberar a memória alocada para um filme
void free_movie(struct movie *movie) {
    free(movie->tconst);
    free(movie->primaryTitle);
    free(movie);
}

// Função para liberar a memória alocada para um ator, incluindo sua lista de filmes
void free_actor(struct actor *actor) {
    free(actor->nconst);
    free(actor->name);
    struct movie *current_movie = actor->movies;
    while (current_movie != NULL) {
        struct movie *temp = current_movie;
        current_movie = current_movie->next;
        free_movie(temp);
    }
    free(actor);
}

// Função de exemplo para imprimir os dados de um ator e seus filmes
void print_actor_info(struct actor *actor) {
    printf("nconst: %s\n", actor->nconst);
    printf("name: %s\n", actor->name);

    printf("Movies:\n");
    struct movie *current_movie = actor->movies;
    while (current_movie != NULL) {
        printf("  tconst: %s\n", current_movie->tconst);
        printf("  primaryTitle: %s\n", current_movie->primaryTitle);
        current_movie = current_movie->next;
    }
}

int main() {
    const char* filename = "arquivos/datasets/name.basics.tsv";
    printf("Attempting to open file: %s\n", filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), file);  // Skip header line

    // Array dinâmico para armazenar os atores
    struct actor **actors_array = malloc(10 * sizeof(struct actor *));
    if (actors_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int num_actors = 0;
    while (fgets(line, sizeof(line), file) && num_actors < 10) {
        struct actor *current_actor = malloc(sizeof(struct actor));
        if (current_actor == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        current_actor->nconst = strdup(strtok(line, "\t"));
        current_actor->name = strdup(strtok(NULL, "\t"));

        // Inicializa a lista de filmes como vazia
        current_actor->movies = NULL;

        // Leitura dos filmes de destaque
        char *movies_token = strtok(NULL, "\t");  // Token para os filmes
        while (movies_token != NULL) {
            struct movie *new_movie = create_movie(movies_token, "");
            add_movie_to_actor(current_actor, new_movie);
            movies_token = strtok(NULL, "\t");
        }

        actors_array[num_actors++] = current_actor;
    }

    fclose(file);

    // Imprime informações dos atores
    for (int i = 0; i < num_actors; i++) {
        printf("Actor %d:\n", i + 1);
        print_actor_info(actors_array[i]);
        printf("\n");
    }

    // Libera memória alocada
    for (int i = 0; i < num_actors; i++) {
        free_actor(actors_array[i]);
    }
    free(actors_array);

    return 0;
}
