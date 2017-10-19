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
    int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 10;
    int k[] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};

    int i;
    int mismatch = 0;

    /* initialize 2 player game with seed 10 */
    initializeGame(numPlayers, k, seed, &pre);

    /* pre.hand[playerOne][0] = smithy; */

    /* copy the game state, use post for testing, and compare to pre */
    post = pre;

    printf("test suite for smithy\n");
    printf("test 1: ");
    /* printf("before cardEffect hand count pre: %d, post: %d\n", pre.handCount[playerOne], post.handCount[playerOne]); */
    /* cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus) */
    cardEffect(smithy, choice1, choice2, choice3, &post, handPos, &bonus);
    printf("hand count pre: %d, post: %d\n", pre.handCount[playerOne], post.handCount[playerOne]);
    printf("played card count pre: %d, post: %d\n", pre.playedCardCount, post.playedCardCount);
    printf("deck count pre: %d, post: %d\n", pre.deckCount[playerOne], post.deckCount[playerOne]);
    printf("coin count pre: %d, post: %d\n", pre.coins, post.coins);
    endTurn(&post);
    printf("discard count pre: %d, post: %d\n", pre.discardCount[playerOne], post.discardCount[playerOne]);
    printf("deck count pre: %d, post: %d\n", pre.deckCount[playerOne], post.deckCount[playerOne]);
    for (i = 0; i < treasure_map + 1; i++) {
	if (pre.supplyCount[i] != post.supplyCount[i]) {
	    mismatch = 1;
	    break;
	}
    }
    if (mismatch) {
	printf("supply counts don't match: %d\n", i);
    } else {
	printf("supply counts match\n");
    }


    /* printf("%s", (actual == expected) ? "PASS" : "FAIL"); */
    /* printf(" - should have 4 coins for initial hand\n"); */


    return 0;
}
