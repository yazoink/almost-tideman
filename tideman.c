#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 9
#define MAX_STR 30

void get_candidates(int argc, char *argv[], char *candidates[]);
int get_int(char *print);
void fill_ballot(int candidate_num, char *ballot[], char *candidates[]);
int find_str_in_arr(int len, char str[], char *arr[]);
int find_candidate(int candidate_num, char *candidates[], char *ballot[], char candidate[]);
int get_arr_size(char *arr[], int len);
void get_scores(char *ballot[], char *candidates[], int *scores, int candidate_num);
void free_all(char *arr[], int len);
void sort_winners(int *scores, char *candidates[], int candidate_num);
void print_winners(char *candidates[], int *scores, int candidate_num);

int main(int argc, char *argv[])
{
    if (argc > MAX + 1)
    {
        printf("ERROR: the maximum number of candidates is %d.\n", MAX);
        return 1;
    }
    if (argc < 2)
    {
        printf("Usage: ./tideman <candidates>\n");
        return 2;
    }
    int candidate_num = argc - 1;
    char *candidates[candidate_num];
    get_candidates(argc, argv, candidates);

    int voters = get_int("Number of voters: ");

    char *ballot[candidate_num];
    ballot[0] = NULL;
    int scores[candidate_num];
    for (int i = 0; i < candidate_num; i++)
    {
        scores[i] = 0;
    }

    for (int i = 0; i < voters; i++)
    {
        fill_ballot(candidate_num, ballot, candidates);
        get_scores(ballot, candidates, scores, candidate_num); // lowest is winner

        printf("\n");
        free_all(ballot, candidate_num);
    }
    sort_winners(scores, candidates, candidate_num);
    print_winners(candidates, scores, candidate_num);
    free_all(candidates, candidate_num);
}

void get_candidates(int argc, char *argv[], char *candidates[])
{
    for (int i = 0; i < argc - 1; i++)
    {
        candidates[i] = (char *)malloc(strlen(argv[i + 1]) + 1); // FREE LATER
        strcpy(candidates[i], argv[i + 1]);
    }
}

int get_int(char *print)
{
    int result;
    printf("%s", print);
    scanf("%d", &result);
    return result;
}

void fill_ballot(int candidate_num, char *ballot[], char *candidates[])
{
    for (int i = 0; i < candidate_num; i++)
    {
        char vote[MAX_STR];
        int is_valid = 1;
        int skip[candidate_num];
        do
        {
            printf("Rank %d: ", i + 1);
            scanf("%s", vote);
            is_valid = find_candidate(candidate_num, candidates, ballot, vote);
        } 
        while (is_valid != 0);

        ballot[i] = (char *)malloc(strlen(vote) + 1); // FREE LATER
        strcpy(ballot[i], vote);
        ballot[i + 1] = NULL;
    }
}

int find_str_in_arr(int len, char str[], char *arr[])
{
    for (int i = 0; i < len; i++)
    {
        if (strcmp(str, arr[i]) == 0)
        {
            return 0;
        }
    }
}

int find_candidate(int candidate_num, char *candidates[], char *ballot[], char candidate[])
{
    if (ballot[0] != NULL)
    {
        if (find_str_in_arr(get_arr_size(ballot, candidate_num), candidate, ballot) == 0)
        {
            return 1;
        }
    }
    for (int i = 0; i < candidate_num; i++)
    {
        if (find_str_in_arr(candidate_num, candidate, candidates) == 0)
        {
            return 0;
        }
    }
    return 1;
}

int get_arr_size(char *arr[], int len)
{
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (arr[i] != NULL)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    return count;
}

void get_scores(char *ballot[], char *candidates[], int *scores, int candidate_num)
{
    for (int i = 0; i < candidate_num; i++) // num of candidate_num
    {
        for (int j = 0; j < candidate_num; j++) // find candidate position in ballot
        {
            if (strcmp(candidates[i], ballot[j]) == 0)
            {
                scores[i] = scores[i] + j;
            }
        }
    }
}

void free_all(char *arr[], int len)
{
    for (int i = 0; i < len; i++)
    {
        free(arr[i]);
    }
}

void move_string(char *str1, char *str2)
{
    str1 = (char *)malloc(strlen(str2) + 1);
    strcpy(str1, str2);
    free(str2);
}

void sort_winners(int *scores, char *candidates[], int candidate_num)
{
    int i, j, pos, swp_i;
    char *swp_s;
    for (i = 0; i < candidate_num - 1; i++)
    {
        pos = i;
        for (j = i + 1; j < candidate_num; j++)
        {
            if (scores[pos] > scores[j])
            {
                pos = j;
            }
        }
        if (pos != i)
        {
            move_string(swp_s, candidates[i]);
            swp_i = scores[i];

            move_string(candidates[i], candidates[pos]);
            scores[i] = scores[pos];

            move_string(candidates[pos], swp_s);
            scores[pos] = swp_i;
        }
    }
}

void print_winners(char *candidates[], int *scores, int candidate_num)
{
    int winner_num = 1;
    for (int i = 1; i < candidate_num; i++)
    {
        if (scores[i] == scores[0])
        {
            winner_num++;
        }
        else
        {
            break;
        }
    }
    for (int i = 0; i < winner_num; i++)
    {
        printf("%s", candidates[i]);
        if (i != winner_num - 1)
        {
            printf(" ");
        }
        else
        {
            printf("\n");
        }
    }
}