#ifndef STATE_GENERATOR_H
#define STATE_GENERATOR_H

#include "helper_functions.h"

params **generate_swaps(int num_threads, int *clause_length);

struct assign state_with_hamming_distance(struct assign vector, int haming_distance);

#endif //STATE_GENERATOR_H
