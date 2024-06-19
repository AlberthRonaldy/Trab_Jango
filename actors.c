#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct actor {
    char *nconst;
    char *primaryName;
    char *birthYear;
    char *deathYear;
    char *primaryProfession;
    char *knownForTitles;
};

struct dynamic_array {
    struct actor *data;
    size_t size;
    size_t capacity;
};

void init_dynamic_array(struct dynamic_array *array, size_t initial_capacity) {
    array->data = malloc(initial_capacity * sizeof(struct actor));
    if (array->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    array->size = 0;
    array->capacity = initial_capacity;
}

void resize_dynamic_array(struct dynamic_array *array) {
    size_t new_capacity = array->capacity * 2;
    struct actor *new_data = realloc(array->data, new_capacity * sizeof(struct actor));
    if (new_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    array->data = new_data;
    array->capacity = new_capacity;
}

void add_actor(struct dynamic_array *array, struct actor new_actor) {
    if (array->size == array->capacity) {
        resize_dynamic_array(array);
    }
    array->data[array->size++] = new_actor;
}

void free_dynamic_array(struct dynamic_array *array) {
    for (size_t i = 0; i < array->size; i++) {
        free(array->data[i].nconst);
        free(array->data[i].primaryName);
        free(array->data[i].birthYear);
        free(array->data[i].deathYear);
        free(array->data[i].primaryProfession);
        free(array->data[i].knownForTitles);
    }
    free(array->data);
}

void read_and_print_actors(const char* filename, struct dynamic_array *actors) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        printf("Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), file);  // Skip header line

    int count = 0;
    while (fgets(line, sizeof(line), file) && count < 20) {
        struct actor currentActor;

        currentActor.nconst = strdup(strtok(line, "\t"));
        currentActor.primaryName = strdup(strtok(NULL, "\t"));
        currentActor.birthYear = strdup(strtok(NULL, "\t"));
        currentActor.deathYear = strdup(strtok(NULL, "\t"));
        currentActor.primaryProfession = strdup(strtok(NULL, "\t"));
        currentActor.knownForTitles = strdup(strtok(NULL, "\t"));

        // Add actor to dynamic array
        add_actor(actors, currentActor);

        printf("nconst: %s\n", currentActor.nconst);
        printf("primaryName: %s\n", currentActor.primaryName);
        printf("birthYear: %s\n", currentActor.birthYear);
        printf("deathYear: %s\n", currentActor.deathYear);
        printf("primaryProfession: %s\n", currentActor.primaryProfession);
        printf("knownForTitles: %s\n", currentActor.knownForTitles);
        printf("\n");

        count++;
    }

    fclose(file);
}

int main() {
    const char* filename = "arquivos/datasets/name.basics.tsv";
    printf("Attempting to open file: %s\n", filename);

    struct dynamic_array actors;
    init_dynamic_array(&actors, 10);  // Start with capacity of 10

    read_and_print_actors(filename, &actors);

    free_dynamic_array(&actors);

    return 0;
}
