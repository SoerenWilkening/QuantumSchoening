//
// Created by Sören Wilkening on 28.02.23.
//

#include "grover_initialization_solver.h"
#include "state_generator.h"
#include <math.h>
#include <string.h>

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

long double binomialCoefficients(int n_, int k) {
    long double C[k + 1];
    memset(C, 0, sizeof(C));
    C[0] = 1;
    for (int i = 1; i <= n_; i++) {
        for (int j = min(i, k); j > 0; j--)
            C[j] = C[j] + C[j - 1];
    }
    return C[k];
}

int schoening_routine(struct assign * ass, cnf *res, set_of_swaps *swaps, int *clause_length) {
    int cl;
    int eval = eval_cnf(ass, res, clause_length);
    if (eval) {
        return 1;
    } else {
        for (int rep = 0; rep < repetitions; rep++) {

            for (cl = 0; cl < n_clause; cl++) {
                if (eval_clause(ass, res->clauses[cl], clause_length[cl]) == 0) {
                    break;
                }
            }
            if (cl == n_clause) {
                return 1;
            } else {
                int r = swaps->swaps[rep][cl] % clause_length[cl];
                ass->assignment[abs(res->clauses[cl][r]) - 1] = !ass->assignment[abs(res->clauses[cl][r]) - 1];
            }
        }
    }
    return 0;
}

long int
calculation_hamming_distance(int hamming_distance, long double reps, int *length, cnf *CNF, params **par,
                             struct assign sol) {
    long int total = 0;

    for (int i = 0; i < reps; i++) {
        if (i % 100 == 0 || i == reps - 1) {
            printf("\rrep = %d", i);
            fflush(stdout);
        }

        struct assign new = state_with_hamming_distance(sol, hamming_distance);
        total += schoening_routine(&new, CNF, &par[0]->swaps, length);
        free(new.assignment);
    }
    return total;
}

/* currently implemented for hamming distance = 1 */
long int calculation_hamming_distance_exact(int hamming_distance, long double reps, int *length, cnf *CNF, params **par,
                                            struct assign sol) {
    int total = 0;
    for (int i = 0; i < reps; i++) {
        printf("\rrep = %d", i);
        fflush(stdout);

        struct assign new = init();
        memcpy(new.assignment, sol.assignment, sol.n_ * sizeof(int));
        new.assignment[i] = !new.assignment[i];

        total += schoening_routine(&new, CNF, &par[0]->swaps, length);
        free(new.assignment);

    }
    return total;
}


int approximate_calculation() {

    char abc[200], def[200];
    int a, x, y, *length = calloc(n_clause, sizeof(int));

    printf("read data\n");
    // preparing the data
    FILE *data = fopen("output_simplified.dimacs", "r");
    fscanf(data, "%s %s %d %d\n", abc, def, &x, &y);

    cnf *CNF = calloc(1, sizeof(cnf));
    CNF[0].n_c = n_clause;
    CNF[0].clauses = calloc(n_clause, sizeof(int *));
    for (int i = 0; i < n_clause; i++) CNF[0].clauses[i] = calloc(10, sizeof(int));

    int count = 0, index = 0, check;
    while (fscanf(data, "%d", &a) == 1) {
        if (check = (a != 0)) CNF[0].clauses[count][index] = a;
        length[count] = check ? length[count] + 1 : length[count];
        index = check ? index + 1 : 0;
        count = check ? count : count + 1;
    }
    fclose(data);

    printf("read solution\n");
    struct assign sol = init();

    data = fopen("output_simplified_solution.txt", "r");

    count = 0;
    while (fscanf(data, "%d", &a) == 1) sol.assignment[count++] = a;
    fclose(data);

    printf("start calculation\n");
    int hamming_distance;
    params **par = generate_swaps(1, length);


    char filename[300];
    char create[300];
    for (hamming_distance = 1; hamming_distance < 12; hamming_distance++) {
        long int res;
        long double reps = binomialCoefficients(n, hamming_distance);
        int maximum = 12345;
        reps = (reps < maximum) ? reps : maximum;

        if (reps < maximum)
            res = calculation_hamming_distance_exact(hamming_distance, reps, length, CNF, par, sol);
        else
            res = calculation_hamming_distance(hamming_distance, reps, length, CNF, par, sol);
        sprintf(filename, "approximate_results/hamming_distance=%d.txt", hamming_distance);
        sprintf(create, "mkdir -p approximate_results/");
        system(create);
        FILE *file = fopen(filename, "a");
        printf("\n");

        fprintf(file, "%.0Lf %ld\n", reps, res);
        fclose(file);
    }

    free(sol.assignment);
    free(length);


    return 0;
}


int *exact_calc(void *param) {

    params *par = (params *) param;

    char *file = par->file;
    set_of_swaps swaps = par->swaps;

    struct timespec start, finish;
    double elapsed;
    int *total = calloc(1, sizeof(int));

    cnf *res = load_cnf(file);
    struct assign ass = init();

    clock_gettime(CLOCK_MONOTONIC, &start);

    int n_sol = 0, j, count = 0;

    int clause_length[n_clause];
    for (int i = 0; i < n_clause; ++i) clause_length[i] = 3;

    for (j = 0; j < pow(2, n); j++) {
        free(ass.assignment);
        ass.assignment = int_to_bin(j);

        count += eval_cnf(&ass, res, clause_length);

        n_sol += schoening_routine(&ass, res, &swaps, clause_length);
    }

    free_cnf(res);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    *total = n_sol;

    printf("N_0 = %d, N_res = %d, T = %f\n", count, n_sol, elapsed);

    char abc[1024];
    char create[1024];
    sprintf(abc, "exact_histogram/%d.txt", 2);
    sprintf(create, "mkdir -p exact_histogram/");
    system(create);
    FILE *file_results = fopen(abc, "a");
    fprintf(file_results, "%d\n", n_sol);
    fclose(file_results);
    pthread_exit((void *) total);
}

int hamming_distance(int *a1, int *a2){
    int h_dist = 0;
    for (int i = 0; i < n; ++i) {
        h_dist += (a1[i] != a2[i]);
    }
    return h_dist;
}

int *exact_hamming_distance_calc(void *param){
    params *par = (params *) param;

    char *file = par->file;
    set_of_swaps swaps = par->swaps;

    int *total_h = calloc(n + 1, sizeof(int));

    int clause_length[n_clause];
    for (int i = 0; i < n_clause; ++i) clause_length[i] = 3;

    cnf *res = load_cnf(file);
    struct assign ass = init();
    int count = 0;

    // calculate the satisfying asignment of the given CNF formula
    for (int j = 0; j < pow(2, n); j++) {
        free(ass.assignment);
        ass.assignment = int_to_bin(j);

        count += eval_cnf(&ass, res, clause_length);
        if (count == 1) break;
    }

    struct assign ass2 = init();

    // iterate over all the assignments. If schöning routine maps to satisfying assignment, increase counter
    // for resprective hamming distance
    for (int j = 0; j < pow(2, n); j++) {
        free(ass2.assignment);
        ass2.assignment = int_to_bin(j);

        int ham = hamming_distance(ass.assignment, ass2.assignment);
        total_h[ham] += schoening_routine(&ass2, res, &swaps, clause_length);
    }
    for (int i = 0; i < n + 1; ++i) {
        char abc[1024];
        char create[1024];
        sprintf(abc, "exact_results/hamming_distance_f=3/hamming_distance=%d.txt", i);
        sprintf(create, "mkdir -p exact_results/hamming_distance_f=3/");
        system(create);
        FILE *file_results = fopen(abc, "a");
        fprintf(file_results, "%d\n", total_h[i]);
        fclose(file_results);
    }
    free(total_h);
    return NULL;
}






