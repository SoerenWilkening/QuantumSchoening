#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "Sourcecode/grover_initialization_solver.h"
#include "Sourcecode/state_generator.h"
#include <string.h>

#define large_instance 1
#define small_instance 0

int main(int argc, char *argv[]) {

    // change this parameter for small scale or large scale calculations
    int method;
    if (argc == 1) method = small_instance;
    else {
        if (strcmp(argv[1], "0"))
            method = small_instance;
        else method = large_instance;
    }

    srand(time(NULL));
    char abc[200];

    int num_threads = 1;

    params **par;
    pthread_t threads[num_threads];

    if (method == large_instance) {
        // execute the approximate caclulation for large instance
        n = 1414;
        n_clause = 18438;
        repetitions = 3 * n;
        approximate_calculation();
        return 0;
    }

    // this will only be executed, if "method" is not set to "large_instance"
    int f1 = 1, f2 = 100;
    if (argc > 2){
        if (argc == 4){
            f1 = atoi(argv[2]);
            f2 = atoi(argv[3]);
        }
        else{
            printf("not enough values!");
            return 1;
        }
    }
    
    for (int f = f1; f <= f2; f++) {
        n = 20;
        n_clause = 91;
        repetitions = 60;
        int clause_length[n_clause];
        for (int i = 0; i < n_clause; ++i) clause_length[i] = 3;

        printf("i = %d\n", f);
        par = generate_swaps(num_threads, clause_length);
        for (int k = 0; k < num_threads; k++) {
            par[k]->f = f;
            sprintf(abc, "uf20-91/uf20-0%d.cnf", f);
            par[k]->file = abc;
            pthread_create(&threads[k], NULL, (void *) exact_hamming_distance_calc, (void *) par[k]);
        }
        for (int k = 0; k < num_threads; k++) pthread_join(threads[k], NULL);
    }
    return 0;
}
