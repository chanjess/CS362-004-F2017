#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include "rngs.h"

/*
 * NOTE: card behavior based on module notes from the instructor.
 * NOTE: card behavior based on module notes from the instructor and the wiki
 *   http://wiki.dominionstrategy.com/index.php/Council_Room
 * NOTE: test setup influenced by the steward sample test case from instructor
 *
 * test suite for the council room
 * card behavior: draw 4 cards, get 1 more buy, other player(s) get to draw card
 * test setup: call initializeGame with 2 players, random seed of 10.
 * 2 tests, one for each player (player 1 is 'player', player 2 is 'opponent'):
 *   player deck has 4 fewer cards
 *   player hand has 3 more cards
 *   player played 1 card
 *   player discards 7 cards
 *   player has an extra buy
 *   opponent deck has 1 fewer card
 *   opponent hand has 1 more card
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
    int numNewCardsInHand = 4;
    int numPlayedCards = 1;
    int numExtraBuys = 1;
    int numNewCardsInOtherHand = 1;
    int playerCount;

    /* initialize 2 player game with seed 10 */
    initializeGame(numPlayers, k, seed, &pre);

    /* copy the game state, use post for testing, and compare to pre */
    post = pre;

    printf("test suite for playing council room\n");
    for (playerCount = 0; playerCount < numPlayers; playerCount++) {
	cardEffect(council_room, choice1, choice2, choice3, &post, handPos, &bonus);

	printf("\ntest results for player %d\n", playerCount+1);
	expected = pre.handCount[playerCount] + numNewCardsInHand - numPlayedCards;
	actual = post.handCount[playerCount];
	printResult(expected, actual, "Player hand count");

	expected = playerCount == 1 ?
	    pre.deckCount[playerCount] - numNewCardsInHand - numNewCardsInOtherHand :
	    pre.deckCount[playerCount] - numNewCardsInHand;
	actual = post.deckCount[playerCount];
	printResult(expected, actual, "Player deck count");

	expected = pre.playedCardCount + numPlayedCards;
	actual = post.playedCardCount;
	printResult(expected, actual, "Player played card count");

	expected = pre.numBuys + numExtraBuys;
	actual = post.numBuys;
	printResult(expected, actual, "Player number of buys");

	expected = playerCount == 1 ? 
	    pre.deckCount[1-playerCount] - numNewCardsInHand - numNewCardsInOtherHand:
	    pre.deckCount[1-playerCount] - numNewCardsInOtherHand;
	actual = post.deckCount[1-playerCount];
	printResult(expected, actual, "Opponent deck count");

	expected = pre.handCount[1-playerCount] + numNewCardsInOtherHand;
	actual = post.handCount[1-playerCount];
	printResult(expected, actual, "Opponent hand count");

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
