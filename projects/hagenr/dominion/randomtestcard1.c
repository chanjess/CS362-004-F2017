#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "dominion_helpers.h"
#include "rngs.h"
#include "randomhelpers.h"

/*
 * NOTE: card behavior based on module notes from the instructor and the wiki
 *   http://wiki.dominionstrategy.com/index.php/Smithy
 * NOTE: test setup influenced by the testDrawCard sample from instructor
 *
 * random test suite for the smithy card
 * card behavior: play the smithy card and add three cards from their 
 *   deck to their hand. no changes to state of other player and no changes in
 *   the number of cards in the supply piles. 
 * test setup: create a gameState struct with random values, 'pre'. call
 *   cardEffect with a copy of 'pre' called 'post'. compare the structs and
 *   report differences
 * tests:
 *   player deck has 3 fewer cards
 *   player hand has 2 more cards
 *   player played 1 card
 *   opponent deck unchanged
 */


/* function prototype */
void testCardSmithy(struct gameState *pre, struct errors *e);

int main() {
    srand(time(NULL));

    printf("smithy card: running %d tests....\n", NUMTESTS);
    struct errors e = {0};
    int i;
    for (i = 0; i < NUMTESTS; i++) {
	struct gameState state;
	randomizeGameState(&state);
	testCardSmithy(&state, &e);
    }

    printf("done!\n");
    if (e.metric1 + e.metric2 + e.metric3 + e.metric4 + e.metric5 > 0)
	printf("\nAt least one test FAILED, please check below\n");
    else
	printf("\nAll tests PASS\n");
    printf("\n***** error rates *****\n");
    printf("player played card count: %.2f%%\n", e.metric1 * 1.0 / NUMTESTS * 100);
    printf("player hand count: %.2f%%\n", e.metric2 * 1.0 / NUMTESTS * 100);
    printf("deck count: %.2f%%\n", e.metric3 * 1.0 / NUMTESTS * 100);
    printf("opponent card counts: %.2f%%\n", e.metric4 * 1.0 / NUMTESTS * 100);
    return 0;
}

/*
 * void testCardSmithy(struct gameState *pre, struct errors *e)
 * pass in a pointer to a gameState struct with random values and a pointer to
 * an error struct
 * description: 
 */
void testCardSmithy(struct gameState *pre, struct errors *e) {
    // cardEffect variables
    int handPosn;  // range 0 - state->handCount[playerNumber]
    int choice1, choice2, choice3;  // range 0 - 26 (CARD values)
    int bonus;  // 0 - MAX_DECK

    // test variables
    int playerNumber = pre->whoseTurn;
    struct gameState post;
    int i;

    handPosn = randi(0, pre->handCount[playerNumber]);
    pre->hand[playerNumber][handPosn] = smithy;

    memcpy(&post, pre, sizeof(struct gameState));

    bonus = randi(0, MAX_DECK);
    choice1 = randi(curse, treasure_map);
    choice2 = randi(curse, treasure_map);
    choice3 = randi(curse, treasure_map);

    /* printf("pre game state\n"); */
    /* showGameState(pre); */
    cardEffect(smithy, choice1, choice2, choice3, &post, handPosn, &bonus); 
    /* printf("post game state\n"); */
    /* showGameState(&post); */

    /* compare pre- to post-gameState, increment errors struct */
    /* smithy rules */
    /* post played hand should be 1 more than pre */
    if (pre->playedCardCount + 1 != post.playedCardCount) e->metric1++;

    /* hand should have only 2 more cards */
    if (pre->handCount[playerNumber] + 2 != post.handCount[playerNumber]) e->metric2++;

    /* post deck should be 3 less than pre */
    if (pre->deckCount[playerNumber] - 3 != post.deckCount[playerNumber]) e->metric3++;

    /* opponent cards counts unchanged */
    for (i = 0; i < pre->numPlayers; i++) {
	if (i == playerNumber) continue;
	else {
	    if (pre->deckCount[i] != post.deckCount[i] || 
		    pre->handCount[i] != post.handCount[i] ||
		    pre->discardCount[i] != post.discardCount[i]) { e->metric4++; }
	}
    }
}

