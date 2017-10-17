#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

/*
 * test suite 
 * function: int updateCoins(int player, struct gameState *state, int bonus)
 * function description: iterates through player's hand, adding value to the
 *   gameState coins field for each treasure card. Adds the bonus after looking
 *   at all cards in the hand.
 *   returns 0
 * test setup: 
 *   test 1: 
 */
int main (int argc, char** argv) {
    struct gameState g;
    int numPlayers = 2;
    int seed = 10;
    int k[] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};
    int bonus;
    int actual, expected;

    initializeGame(numPlayers, k, seed, &g);
    printf("copper: %d, silver: %d, gold: %d\n", g.supplyCount[copper],
	    g.supplyCount[silver], g.supplyCount[gold]);

    return 0;
}
