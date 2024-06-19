#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie {
    char *tconst;
    char *primaryTitle;
    struct movie *next;
};

struct actor {
    char *nconst;
    char *name;
    struct movie *movies;
};

struct movie *create_movie(const char *tconst, const char *primaryTitle) {
    struct movie *new_movie = malloc(sizeof(struct movie));
    if (new_movie == NULL) {
        fprintf(stderr, "Memory allocation failed for movie\n");
        return NULL;
    }
    new_movie->tconst = strdup(tconst);
    if (new_movie->tconst == NULL) {
        fprintf(stderr, "Memory allocation failed for tconst\n");
        free(new_movie);
        return NULL;
    }
    new_movie->primaryTitle = strdup(primaryTitle);
    if (new_movie->primaryTitle == NULL) {
        fprintf(stderr, "Memory allocation failed for primaryTitle\n");
        free(new_movie->tconst);
        free(new_movie);
        return NULL;
    }
    new_movie->next = NULL;
    return new_movie;
}

void add_movie_to_actor(struct actor *actor, struct movie *new_movie) {
    new_movie->next = actor->movies;
    actor->movies = new_movie;
}

void free_movie(struct movie *movie) {
    free(movie->tconst);
    free(movie->primaryTitle);
    free(movie);
}

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
    const char* actor_filename = "arquivos/datasets/name.basics.tsv";
    const char* movie_filename = "arquivos/datasets/title.basics.tsv";

    FILE* actor_file = fopen(actor_filename, "r");
    if (!actor_file) {
        perror("Error opening actor file");
        fprintf(stderr, "Failed to open file: %s\n", actor_filename);
        exit(EXIT_FAILURE);
    }

    FILE* movie_file = fopen(movie_filename, "r");
    if (!movie_file) {
        perror("Error opening movie file");
        fprintf(stderr, "Failed to open file: %s\n", movie_filename);
        fclose(actor_file);
        exit(EXIT_FAILURE);
    }

    char actor_line[1024];
    fgets(actor_line, sizeof(actor_line), actor_file);  // Skip actor file header line

    struct actor **actors_array = malloc(10 * sizeof(struct actor *));
    if (actors_array == NULL) {
        fprintf(stderr, "Memory allocation failed for actors_array\n");
        fclose(actor_file);
        fclose(movie_file);
        exit(EXIT_FAILURE);
    }

    int num_actors = 0;
    while (fgets(actor_line, sizeof(actor_line), actor_file) && num_actors < 10) {
        struct actor *current_actor = malloc(sizeof(struct actor));
        if (current_actor == NULL) {
            fprintf(stderr, "Memory allocation failed for current_actor\n");
            fclose(actor_file);
            fclose(movie_file);
            exit(EXIT_FAILURE);
        }

        current_actor->nconst = strdup(strtok(actor_line, "\t"));
        current_actor->name = strdup(strtok(NULL, "\t"));

        // Inicializa a lista de filmes como vazia
        current_actor->movies = NULL;

        // Leitura dos tconst dos filmes de destaque
        char *movies_token = strtok(NULL, "\t");  // Token para os filmes
        while (movies_token != NULL) {
            struct movie *new_movie = create_movie(movies_token, "");

            // Busca o primaryTitle do filme pelo tconst no arquivo de filmes
            char movie_line[1024];
            fseek(movie_file, 0, SEEK_SET);  // Volta ao início do arquivo de filmes
            fgets(movie_line, sizeof(movie_line), movie_file);  // Skip movie file header line
            while (fgets(movie_line, sizeof(movie_line), movie_file)) {
                char *tconst = strdup(strtok(movie_line, "\t"));
                char *primaryTitle = strdup(strtok(NULL, "\t"));
                if (strcmp(tconst, movies_token) == 0) {
                    free(new_movie->primaryTitle);  // Libera o primaryTitle vazio
                    new_movie->primaryTitle = primaryTitle;  // Define o primaryTitle correto
                    free(tconst);
                    break;
                }
                free(tconst);
                free(primaryTitle);
            }

            if (new_movie != NULL) {
                add_movie_to_actor(current_actor, new_movie);
            }
            movies_token = strtok(NULL, "\t");
        }

        actors_array[num_actors++] = current_actor;
    }

    fclose(actor_file);
    fclose(movie_file);

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
