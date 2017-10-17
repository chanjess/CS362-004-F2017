#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

/*
 * test suite 
 * function: int scoreFor (int player, struct gameState *state)
 * function description: iterates through player's deck (hand, discard, deck),
 *   accumulating points for each victory card according to these rules (verified
 *   on the site http://wiki.dominionstrategy.com/index.php/Victory_point
 *     curse: -1
 *     estate: 1
 *     duchy: 3
 *     province: 6
 *     great hall: 1
 *     gardens: 1 point for every 10 cards in deck
 * test setup: call initializeGame with 2 players, random seed of 10.
 *   test 1: call update coins with standard initial game state for player 0,
 *           no bonus. initial hand has 4 coppers, coins should be 4 * 1 = 4
 *   test 2: change player 0 hand to have 2 coppers, 1 silver, 1 gold. call
 *           updateCoins with bonus of 1. coins should be 2 + 2 + 3 + 1 = 8
 */

int main (int argc, char** argv) {
    struct gameState g;
    int numPlayers = 2;
    int playerNum = 0;
    int seed = 10;
    int k[] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};
    int bonus = 0;
    int actual, expected;

    /* initialize 2 player game with seed 10 */
    initializeGame(numPlayers, k, seed, &g);

    printf("test suite for updateCoins\n");
    printf("test 1: ");
    expected = 4;
    updateCoins(playerNum, &g, bonus);
    actual = g.coins;
    printf("%s", (actual == expected) ? "PASS" : "FAIL");
    printf(" - should have 4 coins for initial hand\n");
    
    /* change player 0 hand. now has 1 gold, 1 silver, 2 copper. give 1 bonus */
    g.hand[playerNum][0] = gold;
    g.hand[playerNum][1] = silver;
    bonus = 1;

    printf("test 2: ");
    expected = 8;
    updateCoins(playerNum, &g, bonus);
    actual = g.coins;
    printf("%s", (actual == expected) ? "PASS" : "FAIL");
    printf(" - should have 8 coins for second hand\n");
    return 0;
}
