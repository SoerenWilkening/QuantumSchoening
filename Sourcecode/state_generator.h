//
// Created by Sören Wilkening on 28.02.23.
//

#ifndef SIMP_SCH_STATE_GENERATOR_H
#define SIMP_SCH_STATE_GENERATOR_H

#include "helper_functions.h"

params **generate_swaps(int num_threads, int *clause_length);

struct assign state_with_hamming_distance(struct assign vector, int haming_distance);

#endif //SIMP_SCH_STATE_GENERATOR_H
