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
    int id;
    char *name;
    struct movie *movies;  // Lista encadeada de filmes de destaque
};

// Função para criar um novo ator
struct actor *create_actor(int id, const char *name) {
    struct actor *new_actor = malloc(sizeof(struct actor));
    if (new_actor == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_actor->id = id;
    new_actor->name = strdup(name);
    if (new_actor->name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_actor->movies = NULL;  // Inicializa a lista de filmes como vazia
    return new_actor;
}

// Função para adicionar um filme à lista de filmes de destaque de um ator
void add_movie_to_actor(struct actor *actor, struct movie *new_movie) {
    new_movie->next = actor->movies;
    actor->movies = new_movie;
}

// Função para liberar a memória alocada para um ator, incluindo sua lista de filmes
void free_actor(struct actor *actor) {
    free(actor->name);
    struct movie *current_movie = actor->movies;
    while (current_movie != NULL) {
        struct movie *temp = current_movie;
        current_movie = current_movie->next;
        free(temp->tconst);
        free(temp->primaryTitle);
        free(temp);
    }
    free(actor);
}

// Função de exemplo para imprimir os dados de um ator e seus filmes
void print_actor_info(struct actor *actor) {
    printf("ID: %d\n", actor->id);
    printf("Name: %s\n", actor->name);

    printf("Movies:\n");
    struct movie *current_movie = actor->movies;
    while (current_movie != NULL) {
        printf("  tconst: %s\n", current_movie->tconst);
        printf("  primaryTitle: %s\n", current_movie->primaryTitle);
        current_movie = current_movie->next;
    }
}

int main() {
    // Exemplo de uso
    struct actor *john_doe = create_actor(1, "John Doe");

    // Adicionando filmes de destaque
    struct movie *movie1 = malloc(sizeof(struct movie));
    movie1->tconst = strdup("tt1234567");
    movie1->primaryTitle = strdup("Movie 1");

    struct movie *movie2 = malloc(sizeof(struct movie));
    movie2->tconst = strdup("tt9876543");
    movie2->primaryTitle = strdup("Movie 2");

    add_movie_to_actor(john_doe, movie1);
    add_movie_to_actor(john_doe, movie2);

    // Imprimindo informações do ator
    print_actor_info(john_doe);

    // Liberando memória alocada
    free_actor(john_doe);

    return 0;
}
