#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include "rngs.h"

/*
 * NOTE: card behavior based on module notes from the instructor and the wiki
 *   http://wiki.dominionstrategy.com/index.php/Village
 * NOTE: test setup influenced by the steward sample test case from instructor
 *
 * test suite for the village card
 * card behavior: draw 1 card, get 2 more actions
 * test setup: call initializeGame with 2 players, random seed of 10.
 * tests
 *   player 1 deck has 1 fewer card
 *   player 1 hand has 1 more card
 *   player 1 played 1 card
 *   player 1 discards 5 cards
 *   player 1 has 2 more actions
 *   player 2 deck unchanged
 *   coins are unchanged
 *   supply pile counts are unchanged
 */

void printResult(int expected, int actual, char *tmp);

int main (int argc, char** argv) {
    struct gameState pre, post;
    int numPlayers = 2;
    int playerOne = 0;
    int playerTwo = 1;
    /* not sure what the choices should be for council room, 0 makes sense */
    int handPos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 10;
    int k[] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};

    int i;
    int expected, actual;
    int mismatch = 0;
    int numNewCardsInHand = 1;
    int numPlayedCards = 1;
    int numNewActions = 2;

    /* initialize 2 player game with seed 10 */
    initializeGame(numPlayers, k, seed, &pre);

    /* copy the game state, use post for testing, and compare to pre */
    post = pre;

    printf("test suite for playing village\n");
    cardEffect(village, choice1, choice2, choice3, &post, handPos, &bonus);

    expected = pre.handCount[playerOne] + numNewCardsInHand - numPlayedCards;
    actual = post.handCount[playerOne];
    printResult(expected, actual, "Player 1 hand count");

    expected = pre.deckCount[playerOne] - numNewCardsInHand;
    actual = post.deckCount[playerOne];
    printResult(expected, actual, "Player 1 deck count");

    expected = pre.playedCardCount + numPlayedCards;
    actual = post.playedCardCount;
    printResult(expected, actual, "Player 1 played card count");

    expected = pre.numActions + numNewActions;
    actual = post.numActions;
    printResult(expected, actual, "Player 1 actions count");

    expected = pre.deckCount[playerTwo];
    actual = post.deckCount[playerTwo];
    printResult(expected, actual, "Player 2 deck count");

    expected = pre.coins;
    actual = post.coins;
    printResult(expected, actual, "Coin count");

    endTurn(&post);

    expected = numNewCardsInHand + pre.handCount[playerOne] - numPlayedCards;
    actual = post.discardCount[playerOne];
    printResult(expected, actual, "Player 1 discard count");

    for (i = 0; i < treasure_map + 1; i++) {
	if (pre.supplyCount[i] != post.supplyCount[i]) {
	    mismatch = 1;
	    break;
	}
    }
    if (!mismatch) {
	printf("PASSED: Supply counts unchanged\n");
    } else {
	printf("FAILED: Supply counts don't match at card %d\n", i);
    }

    return 0;
}

void printResult(int expected, int actual, char *tmp) {
    char *testName = tmp;
    if (expected == actual) {
	printf("PASSED: %s", testName);
    } else {
	printf("FAILED: %s - expected %d, got %d", testName, expected, actual);
    }
    printf("\n");
}
