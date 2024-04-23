//
// Created by Sören Wilkening on 28.02.23.
//

#include "state_generator.h"
#include <string.h>


params **generate_swaps(int num_threads, int *clause_length) {

    set_of_swaps swaps[num_threads];
    params **par = calloc(num_threads, sizeof(params *));

    int longest_clause = 0;
    for (int i = 0; i < n_clause; ++i) {
        if (clause_length[i] > longest_clause)
            longest_clause = clause_length[i];
    }

    for (int k = 0; k < num_threads; k++) {
        swaps[k].swaps = calloc(repetitions, sizeof(int *));

        for (int i = 0; i < repetitions; i++) {
            swaps[k].swaps[i] = calloc(n_clause, sizeof(int));
        }
        for (int i = 0; i < repetitions; i++) {
            swaps[k].swaps[i] = calloc(n_clause, sizeof(int));
            int r = rand() % longest_clause;
            for (int j = 0; j < n_clause; j++) swaps[k].swaps[i][j] = r;

        }
        par[k] = calloc(1, sizeof(params));
        par[k]->swaps = swaps[k];
    }

    return par;

}

int item_not_in_vector(int item, int *vector, int n_) {
    for (int i = 0; i < n_; ++i) {
        if (vector[i] == item) return 0;
    }
    return 1;
}


int *change_state(int haming_distance){
    int *changes = calloc(haming_distance, sizeof(int));
    for (int i = 0; i < haming_distance; ++i) changes[i] = -1;

    changes[0] = rand() % n;
    for (int i = 0; i < haming_distance; ++i) {
        int a = 0;
        while (a == 0) {
            int b = rand() % n;
            if (item_not_in_vector(b, changes, haming_distance)) {
                changes[i] = b;
                a = 1;
            }
        }
    }
    return changes;
}


struct assign state_with_hamming_distance(struct assign vector, int haming_distance) {

    int changes[haming_distance];
    for (int i = 0; i < haming_distance; ++i) changes[i] = -1;

    struct assign new = init();
    memcpy(new.assignment, vector.assignment, vector.n_ * sizeof(int));

    changes[0] = rand() % n;
    for (int i = 0; i < haming_distance; ++i) {
        int a = 0;
        while (a == 0) {
            int b = rand() % n;
            if (item_not_in_vector(b, changes, haming_distance)) {
                changes[i] = b;
                a = 1;
            }
        }
        new.assignment[changes[i]] = (new.assignment[changes[i]] + 1) % 2;
    }

    return new;
}