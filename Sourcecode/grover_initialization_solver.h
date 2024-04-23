//
// Created by Sören Wilkening on 28.02.23.
//

#ifndef SIMP_SCH_GROVER_INITIALIZATION_SOLVER_H
#define SIMP_SCH_GROVER_INITIALIZATION_SOLVER_H

#include "helper_functions.h"

int *exact_calc(void *param);

int approximate_calculation();

int *change_state(int haming_distance);

int *exact_hamming_distance_calc(void *param);

#endif //SIMP_SCH_GROVER_INITIALIZATION_SOLVER_H
