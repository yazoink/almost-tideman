#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define MAX 9
#define MAX_STR 30

typedef struct {
    char *name;
    size_t score;
} vote_t;

void check_malloc (const void* ptr);
uint8_t get_voters(void);
void get_candidates(const uint8_t candidate_num, char* argv[], vote_t *candidates);
void take_votes(vote_t *candidates, const uint8_t voter_num, const uint8_t candidate_num);

int
main(int argc, char* argv[]) {
    vote_t* candidates;
    uint8_t candidate_num, voter_num;

    if (argc < 2) {
        printf("Usage: ./tideman <candidates>\n");
        return 1;
    }

    if (argc > MAX + 1) {
        printf("Error: the maximum number of candidates is 9.\n");
        return 1;
    }

    candidate_num = argc - 1;
    candidates = malloc(sizeof(vote_t) * candidate_num);
    check_malloc(&candidates);

    voter_num = get_voters();
    get_candidates(candidate_num, argv, candidates);

    for (size_t i = 0; i < candidate_num; ++i) {
        candidates[i].score = 0;
    }

    for (size_t i = 0; i < voter_num; ++i) {
        take_votes(candidates, voter_num, candidate_num);
    }
}

void
check_malloc (const void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Error: memory not allocated.\n");
        exit(1);
    }
}

uint8_t
get_voters(void) {
    uint8_t num, result;
    while (!result) {
        printf("Number of voters: ");
        result = scanf("%" SCNu8, &num);
        while (getchar() != '\n');
    }
    return num;
}

void
get_candidates(const uint8_t candidate_num, char* argv[], vote_t *candidates) {
    for (size_t i = 0; i < candidate_num; ++i) {
        candidates[i].name = (char*)malloc(sizeof(argv[i + 1]));
        check_malloc(&candidates[i].name);
        strcpy(candidates[i].name, argv[i + 1]);
    }
}

uint8_t
vote_valid(const char *str, const vote_t *candidates, const uint8_t candidate_num) {
    for (size_t i = 0; i < candidate_num; ++i) {
        if (strcmp(candidates[i].name, str) == 0) {
            return i + 1;
        }
    }
    return 0;
}

void
take_votes(vote_t *candidates, const uint8_t voter_num, const uint8_t candidate_num) {
    for (size_t i = 0; i < candidate_num; ++i) {
        char str[MAX_STR];
        uint8_t valid = 0;
        while (!valid) {
            printf("Rank %zu: ", i + 1);
            scanf("%[^\n]s", str);
            while (getchar() != '\n');
            valid = vote_valid(str, candidates, candidate_num);
        }
        candidates[valid].score = candidates[valid].score + i;
    }
}