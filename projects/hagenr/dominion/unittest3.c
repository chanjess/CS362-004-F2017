#include "dominion.h"
#include "dominion_helper.h"
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
 * test setup: call initializeGame with 2 players, random seed of 10.
 *   test 1: call update coins with standard initial game state for player 0,
 *           no bonus. coins should be 46 * 1 + 40 * 2 + 30 * 3 = 216
 *   test 2: 
 */
/* int updateCoins(int player, struct gameState *state, int bonus); */

int main (int argc, char** argv) {
    struct gameState g;
    int numPlayers = 2;
    int playerNum = 0;
    int seed = 10;
    int k[] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};
    int bonus = 0;
    int i;
    int actual, expected;

    /* initialize 2 player game with seed 10 */
    initializeGame(numPlayers, k, seed, &g);

    printf("test suite for updateCoins\n");
    // printf("copper: %d, silver: %d, gold: %d\n", g.supplyCount[copper], g.supplyCount[silver], g.supplyCount[gold]);
    for (i = 0; i < g.handCount[playerNum]; i++) {
	printf("card %d: %d\n", i, g.hand[playerNum][i]);
    }
    printf("test 1: ");
    expected = 216;
    updateCoins(playerNum, &g, bonus);
    actual = g.coins;
    printf("%s", (actual == expected) ? "PASS" : "FAIL");
    printf(" - should have 216 coins for initial deal\n");

    return 0;
}
