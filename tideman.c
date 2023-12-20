#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define MAX 9
#define MAX_STR 30

typedef struct {
    char *name;
    uint_fast8_t score;
} vote_t;

void check_malloc (const void* ptr);
uint_fast8_t get_voters(void);
void get_candidates(const uint_fast8_t candidate_i, char* argv[], vote_t* candidates);
void take_votes(vote_t* candidates, const uint_fast8_t candidate_i);
void sort_scores(vote_t* candidates, uint_fast8_t unsorted_i, uint_fast8_t count);
uint_fast8_t get_winners(vote_t* candidates, uint_fast8_t candidate_i);

int
main(int argc, char* argv[]) {
    vote_t* candidates;
    uint_fast8_t candidate_i, voter_i, winner_i;
    if (argc < 2) {
        printf("Usage: ./tideman <candidates>\n");
        return 1;
    }
    if (argc > MAX + 1) {
        printf("Error: the maximum number of candidates is 9.\n");
        return 1;
    }
    candidate_i = argc - 1;
    candidates = malloc(sizeof(vote_t) * candidate_i);
    check_malloc(&candidates);
    voter_i = get_voters();
    get_candidates(candidate_i, argv, candidates);
    for (uint_fast8_t i = 0; i < candidate_i; ++i) {
        candidates[i].score = 0;
    }
    for (uint_fast8_t i = 0; i < voter_i; ++i) {
        take_votes(candidates, candidate_i);
        printf("\n");
    }
    sort_scores(candidates, candidate_i - 1, 0);
    winner_i = get_winners(candidates, candidate_i);
    for (uint_fast8_t i = 0; i < winner_i; ++i) {
        printf("%s", candidates[i].name);
        free(candidates[i].name);
        if (i == winner_i - 1) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    free(candidates);
}

void
check_malloc (const void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Error: memory not allocated.\n");
        exit(1);
    }
}

uint_fast8_t
get_voters(void) {
    uint_fast8_t num, result;
    result = 0;
    while (!result) {
        printf("Number of voters: ");
        result = scanf("%" SCNuFAST8, &num);
        while (getchar() != '\n');
    }
    return num;
}

void
get_candidates(const uint_fast8_t candidate_i, char* argv[], vote_t *candidates) {
    for (uint_fast8_t i = 0; i < candidate_i; ++i) {
        candidates[i].name = (char*)malloc(sizeof(argv[i + 1]));
        check_malloc(&candidates[i].name);
        strcpy(candidates[i].name, argv[i + 1]);
    }
}

uint_fast8_t
vote_valid(const char *str, const vote_t *candidates, const uint_fast8_t candidate_i) {
    for (uint_fast8_t i = 0; i < candidate_i; ++i) {
        if (strcmp(candidates[i].name, str) == 0) {
            return candidate_i - i;
        }
    }
    return 0;
}

void
take_votes(vote_t* candidates, const uint_fast8_t candidate_i) {
    for (uint_fast8_t i = 0; i < candidate_i; ++i) {
        char str[MAX_STR];
        uint_fast8_t valid = 0;
        while (!valid) {
            printf("Rank %" PRIuFAST8 ": ", i + 1);
            scanf("%[^\n]s", str);
            while (getchar() != '\n');
            valid = vote_valid(str, candidates, candidate_i);
        }
        candidates[i].score = candidates[i].score + valid;
    }
}

void
swap_candidates(vote_t* candidates, uint_fast8_t count, uint_fast8_t index) {
    char* temp_s = NULL;
    uint_fast8_t temp_i = candidates[count].score;
    temp_s = (char*)malloc(sizeof(candidates[count].name + 1));
    check_malloc(&temp_s);
    strcpy(temp_s, candidates[count].name);
    candidates[count].score = candidates[index].score;
    candidates[count].name = (char*)realloc(candidates[count].name, sizeof(candidates[index].name + 1));
    check_malloc(&candidates[count].name);
    strcpy(candidates[count].name, candidates[index].name);
    candidates[index].score = temp_i;
    candidates[index].name = (char*)realloc(candidates[index].name, sizeof(temp_s + 1));
    strcpy(candidates[index].name, temp_s);
    free(temp_s);
}

void
sort_scores(vote_t* candidates, uint_fast8_t unsorted_i, uint_fast8_t count) {
    uint_fast8_t min, index;
    if (!unsorted_i) {
        return;
    }
    min = 255;
    for (uint_fast8_t i = count; i < unsorted_i; ++i) {
        if (candidates[i].score < min) {
            min = candidates[i].score;
            index = i;
        }
    }
    swap_candidates(candidates, count, index);
    --unsorted_i;
    ++count;
    sort_scores(candidates, unsorted_i, count);
}

uint_fast8_t
get_winners(vote_t* candidates, uint_fast8_t candidate_i) {
    uint_fast8_t i, winner_i;
    winner_i = 1;
    for (i = 1; i < candidate_i - 1; ++i) {
        if (candidates[i].score < candidates[0].score) {
            free(candidates[i].name);
        } else {
            ++winner_i;
        }
    }
    return winner_i;
}