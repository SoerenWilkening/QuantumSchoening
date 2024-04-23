#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>


int n;
int n_clause;
int repetitions;

typedef struct cnf{
    int n_c;
    int **clauses;
} cnf;

typedef struct set_of_swaps{
    int **swaps;
} set_of_swaps;

typedef struct params{
    char *file;
    int f;

    set_of_swaps swaps;
} params;

typedef struct assign{
    int n_;
    int *assignment;
} assign;

struct assign init();

void free_cnf(cnf * CNF);

cnf *load_cnf(char *file);

int eval_clause(struct assign *ass, int *clause, int clause_length);

int eval_cnf(struct assign *ass, cnf *CNF, int *clause_length);

int *int_to_bin(int val);

#endif //HELPER_FUNCTIONS_H
