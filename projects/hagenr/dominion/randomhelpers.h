#ifndef RANDOMHELPERS_H
#define RANDOMHELPERS_H

// random test helper functions
#include "dominion.h"

/* global constants */
#define NUMTESTS 100
#define MINCARDS 3

/* 
 * struct to hold number of mismatches between pre- & post-gameState
 */
struct errors {
    int metric1;
    int metric2;
    int metric3;
    int metric4;
    int metric5;
};

/* function prototypes */
int randi(int low, int high);
void showGameState(struct gameState *state);
void randomizeGameState(struct gameState *state);


#endif
