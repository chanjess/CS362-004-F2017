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
 * 2 tests, one for each player (player 1 is 'player', player 2 is 'opponent'):
 *   player deck has 1 fewer card
 *   player hand has 1 more card
 *   player played 1 card
 *   player discards 5 cards
 *   player has 2 more actions
 *   opponent deck unchanged
 *   coins are unchanged
 *   supply pile counts are unchanged
 */

void printResult(int expected, int actual, char *tmp);

int main (int argc, char** argv) {
    struct gameState pre, post;
    int numPlayers = 2;
    /* set choices to same as smithy, since they're not used for this card */
    int handPos = 0, choice1 = -1, choice2 = -1, choice3 = -1, bonus = 0;
    int seed = 10;
    int k[] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};

    int i;
    int expected, actual;
    int mismatch = 0;
    int numNewCardsInHand = 1;
    int numPlayedCards = 1;
    int numNewActions = 2;
    int playerCount;

    /* initialize 2 player game with seed 10 */
    initializeGame(numPlayers, k, seed, &pre);

    /* copy the game state, use post for testing, and compare to pre */
    post = pre;

    printf("test suite for playing village\n");
    for (playerCount = 0; playerCount < numPlayers; playerCount++) {
	cardEffect(village, choice1, choice2, choice3, &post, handPos, &bonus);

	printf("\ntest results for player %d\n", playerCount+1);
	expected = pre.handCount[playerCount] + numNewCardsInHand - numPlayedCards;
	actual = post.handCount[playerCount];
	printResult(expected, actual, "Player hand count");

	expected = pre.deckCount[playerCount] - numNewCardsInHand;
	actual = post.deckCount[playerCount];
	printResult(expected, actual, "Player deck count");

	expected = pre.playedCardCount + numPlayedCards;
	actual = post.playedCardCount;
	printResult(expected, actual, "Player played card count");

	expected = pre.numActions + numNewActions;
	actual = post.numActions;
	printResult(expected, actual, "Player actions count");

	expected = playerCount == 1 ? 
	    pre.deckCount[1-playerCount] - numNewCardsInHand :
	    pre.deckCount[1-playerCount];
	actual = post.deckCount[1-playerCount];
	printResult(expected, actual, "Opponent deck count");

	expected = pre.coins;
	actual = post.coins;
	printResult(expected, actual, "Coin count");

	/* have to call endTurn to actually discard */
	expected = numNewCardsInHand + pre.handCount[playerCount] - numPlayedCards;
	endTurn(&post);
	endTurn(&pre);
	actual = post.discardCount[playerCount];
	printResult(expected, actual, "Player discard count");

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
