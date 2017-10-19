#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

/*
 * NOTE: card behavior based on module notes from the instructor.
 * NOTE: test set up influenced by the steward sample test case from instructor
 *
 * test suite for the smithy card
 * card behavior: player discards the smithy and adds three cards from their 
 *   deck to their hand. no changes to state of other player and no changes in
 *   the number of cards in the supply piles. 
 * checks
 *   number of smithy cards in hand decremented
 *   deck has 3 fewer cards
 *   hand has 2 more cards
 *   supply piles are unchanged
 *   player 2 cards unchanged
 * test setup: call initializeGame with 2 players, random seed of 10.
 * test 1: call update coins with standard initial game state for player 0,
 *         no bonus. initial hand has 4 coppers, coins should be 4 * 1 = 4
 * test 2: change player 0 hand to have 2 coppers, 1 silver, 1 gold. call
 *         updateCoins with bonus of 1. coins should be 2 + 2 + 3 + 1 = 8
 */

int main (int argc, char** argv) {
    struct gameState pre, post;
    int numPlayers = 2;
    int playerOne = 0;
    int playerTwo = 1;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 10;
    int k[] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};

    /* initialize 2 player game with seed 10 */
    initializeGame(numPlayers, k, seed, &pre);

    /* copy the game state, use post for testing, and compare to pre */
    post = pre;

    printf("test suite for smithy\n");
    printf("test 1: ");
    /* int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus) */
    cardEffect(smithy, choice1, choice2, choice3, &post, handPos, &bonus); 

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
