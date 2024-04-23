#ifndef GROVER_INITIALIZATION_SOLVER_H
#define GROVER_INITIALIZATION_SOLVER_H

#include "helper_functions.h"

int *exact_calc(void *param);

int approximate_calculation();

int *change_state(int haming_distance);

int *exact_hamming_distance_calc(void *param);

#endif //GROVER_INITIALIZATION_SOLVER_H
