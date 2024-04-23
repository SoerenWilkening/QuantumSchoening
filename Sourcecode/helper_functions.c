#include "helper_functions.h"


struct assign init(){
    struct assign ass;

    ass.n_ = n;
    ass.assignment = calloc(n, sizeof(int));

    return ass;
}


void free_cnf(cnf * CNF){
    for(int i = 0; i < CNF->n_c; ++i){
        free(CNF[0].clauses[i]);
    }
    free(CNF[0].clauses);
}


cnf *load_cnf(char *file){
    printf("%s\n", file);
    FILE *data = fopen(file, "r");
    char abc[200];

    for(int i = 0; i < 28; i++){
        fscanf(data, "%s\n", abc);
    }

    int a, b, c, d, e = 0;

    cnf *CNF = calloc(1, sizeof(cnf));
    CNF[0].n_c = n_clause;
    CNF[0].clauses = calloc(n_clause, sizeof(int *));

    while(fscanf(data, "%d %d %d %d", &a, &b, &c, &d) == 4){

        CNF[0].clauses[e] = calloc(3, sizeof(int));

        CNF[0].clauses[e][0] = a;
        CNF[0].clauses[e][1] = b;
        CNF[0].clauses[e][2] = c;

        e++;
    }
    fclose(data);

    return CNF;
}


int eval_clause(struct assign *ass, int *clause, int clause_length){
    int res = 0;

    for (int i = 0; i < clause_length; i++){
        if(clause[i] < 0){
            res += 1 - ass->assignment[abs(clause[i]) - 1];
        }else{
            res += ass->assignment[abs(clause[i]) - 1];
        }
    }

    if(res == 0) return 0;
    else return 1;
}


int eval_cnf(struct assign *ass, cnf *CNF, int *clause_length){
    int res = 0;

    for(int i = 0; i < CNF[0].n_c; i++){
        int eval = eval_clause(ass, CNF[0].clauses[i], clause_length[i]);
        if (eval == 0){
            return 0;
        }
        res = res + eval;
    }

    if (res < CNF[0].n_c){
        return 0;
    } else{
        return 1;
    }
}


int *int_to_bin(int val){
    int i = 0, *res = calloc(n, sizeof(int));

    while(val > 0){
        res[i] = val % 2;
        val = val / 2;
        i++;
    }
    return res;
}
