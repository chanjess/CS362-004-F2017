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
 *   http://wiki.dominionstrategy.com/index.php/Adventurer
 * NOTE: test setup influenced by the testDrawCard sample from instructor
 *
 * random test suite for the adventurer card
 * card behavior: play the adventurer card. keep revealing cards from your deck
 *   until you find 2 treasure cards, which you add to your hand. discard the
 *   other revealed cards
 * test setup: create a gameState struct with random values, 'pre'. call
 *   cardEffect with a copy of 'pre' called 'post'. compare the structs and
 *   report differences
 * tests:
 *   player deck has x fewer cards - no visibility into this metric
 *   player hand has 2 more treasure cards
 *   player hand has 1 more card
 *   played card has 1 more card
 *   player deck + discard has 2 fewer cards
 *   opponent card counts unchanged
 */

/* function prototype */
void testCardAdventurer(struct gameState *pre, struct errors *e);

int main() {
    srand(time(NULL));

    printf("adventurer card: running %d random tests....\n", NUMTESTS);
    struct errors e = {0};
    int i;
    for (i = 0; i < NUMTESTS; i++) {
	struct gameState state;
	randomizeGameState(&state);
	testCardAdventurer(&state, &e);
    }

    printf("done!\n");
    if (e.metric1 + e.metric2 + e.metric3 + e.metric4 + e.metric5 > 0)
	printf("\nAt least one test FAILED, please check below\n");
    else
	printf("\nAll tests PASS\n");
    printf("\n***** error rates *****\n");
    printf("treasure card counts: %.2f%%\n", e.metric1 * 1.0 / NUMTESTS * 100);
    printf("player hand count: %.2f%%\n", e.metric2 * 1.0 / NUMTESTS * 100);
    printf("total discards+deck count: %.2f%%\n", e.metric3 * 1.0 / NUMTESTS * 100);
    printf("opponent card counts: %.2f%%\n", e.metric4 * 1.0 / NUMTESTS * 100);
    printf("played card counts: %.2f%%\n", e.metric5 * 1.0 / NUMTESTS * 100);
    return 0;
}

/*
 * void testCardAdventurer(struct gameState *pre, struct errors *e)
 * pass in a pointer to a gameState struct with random values and a point to
 * an error struct
 * description: 
 */
void testCardAdventurer(struct gameState *pre, struct errors *e) {
    // cardEffect variables
    int handPosn;  // range 0 - state->handCount[playerNumber]
    int choice1, choice2, choice3;  // range 0 - 26 (CARD values)
    int bonus;  // 0 - MAX_DECK

    // test variables
    int playerNumber = pre->whoseTurn;
    struct gameState post;
    int i;
    int preTreasureCardsInHand = 0;
    int postTreasureCardsInHand = 0;

    handPosn = randi(0, pre->handCount[playerNumber]);
    pre->hand[playerNumber][handPosn] = adventurer;

    memcpy(&post, pre, sizeof(struct gameState));

    bonus = randi(0, MAX_DECK);
    choice1 = randi(curse, treasure_map);
    choice2 = randi(curse, treasure_map);
    choice3 = randi(curse, treasure_map);

    /* printf("pre game state\n"); */
    /* showGameState(pre); */
    cardEffect(adventurer, choice1, choice2, choice3, &post, handPosn, &bonus); 
    /* printf("post game state\n"); */
    /* showGameState(&post); */

    /* compare pre- to post-gameState, increment errors struct */
    /* adventure rules */
    /* post hand should have 2 more treasure cards */
    for (i = 0; i < pre->handCount[playerNumber]; i++) {
	int cardPre = pre->hand[playerNumber][i];
	if (cardPre == copper || cardPre == gold || cardPre == silver) {
	    preTreasureCardsInHand++;
	}
    }
    for (i = 0; i < post.handCount[playerNumber]; i++) {
	int cardPost = post.hand[playerNumber][i];
	if (cardPost == copper || cardPost == gold || cardPost == silver) {
	    postTreasureCardsInHand++;
	}
    }
    if (preTreasureCardsInHand + 2 != postTreasureCardsInHand) e->metric1++;

    /* hand should have 1 more card */
    if (pre->handCount[playerNumber] + 1 != post.handCount[playerNumber]) e->metric2++;

    /* post deck + discard should be 2 less than pre */
    if (pre->deckCount[playerNumber] + pre->discardCount[playerNumber] != 
	    post.deckCount[playerNumber] + post.discardCount[playerNumber] + 2) { e->metric3++; }

    /* opponent cards counts unchanged */
    for (i = 0; i < pre->numPlayers; i++) {
	if (i == playerNumber) continue;
	else {
	    if (pre->deckCount[i] != post.deckCount[i] || 
		    pre->handCount[i] != post.handCount[i] ||
		    pre->discardCount[i] != post.discardCount[i]) { e->metric4++; }
	}
    }

    /* post played card count should be 1 more than pre */
    if (pre->playedCardCount + 1 != post.playedCardCount) e->metric5++;
}
