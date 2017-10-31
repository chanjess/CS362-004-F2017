#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

/*
 * NOTE: card behavior based on module notes from the instructor and the wiki
 *   http://wiki.dominionstrategy.com/index.php/Adventurer
 * NOTE: test setup influenced by the testDrawCard sample from instructor
 *
 * random test suite for the adventurer card
 * card behavior: play the adventurer card. keep revealing cards from your deck
 *   until you find 2 treasure cards, which you add to your hand. discard the
 *   other revealed cards
 * test setup: create 2 gameState structs with random values. call cardEffect 
 *   with 1 gameState and random args. manually manipulate the other gameState
 *   via adventurer rules and same random arg values. compare the structs and
 *   report differences
 * 2 tests, one for each player (player 1 is 'player', player 2 is 'opponent'):
 *   player deck has x fewer cards - no visibility into this metric
 *   player hand has 1 more cards
 *   player played 1 card
 *   opponent deck unchanged
 *   coins are unchanged
 *   player discards 7 cards
 *   supply pile counts are unchanged
 */

/* global constants */
#define NUMTESTS 10
#define MINCARDS 3

/* function prototypes */
void testCardAdventurer(struct gameState *state);
int randi(int low, int high);
void showGameState(struct gameState *state);

/* 
 * int randi(int low, int high)
 * returns a random integer in range [low, high)
 * based on algorithm discussed in CS 475
 */
int randi(int low, int high) {
    double r = rand();
    return (int)(low + r * (high - low) / (double)RAND_MAX);
}

void showGameState(struct gameState *state) {
    int i;
    printf("num players: %d\n", state->numPlayers);
    printf("outpost played: %d\n", state->outpostPlayed);
    printf("outpost turn: %d\n", state->outpostTurn);
    printf("whose turn: %d\n", state->whoseTurn);
    printf("phase: %d\n", state->phase);
    printf("num actions: %d\n", state->numActions);
    printf("coins: %d\n", state->coins);
    printf("num buys: %d\n", state->numBuys);
    printf("played card count: %d\n", state->playedCardCount);

    printf("supply count: ");
    for (i = 0; i < treasure_map + 1; i++)
	printf("%d ", state->supplyCount[i]);
    printf("\n");

    printf("embargo tokens: ");
    for (i = 0; i < treasure_map + 1; i++)
	printf("%d ", state->embargoTokens[i]);
    printf("\n");

    printf("hand count: ");
    for (i = 0; i < MAX_PLAYERS; i++)
	printf("p%d: %d ", i, state->handCount[i]);
    printf("\n");

    printf("deck count: ");
    for (i = 0; i < MAX_PLAYERS; i++)
	printf("p%d: %d ", i, state->deckCount[i]);
    printf("\n");

    printf("discard count: ");
    for (i = 0; i < MAX_PLAYERS; i++)
	printf("p%d: %d ", i, state->discardCount[i]);
    printf("\n");

    printf("played cards: ");
    for (i = 0; i < MAX_DECK; i++)
	printf("%d ", state->playedCards[i]);
    printf("\n");
}
/* k = { 7, 9, 22, 14, 17, 11, 21, 25, 19, 13 }
   enum CARD 
   {curse, estate, duchy, province,
   copper, silver, gold,
7: adventurer, council_room, feast, gardens, mine, remodel,
13: smithy, village, baron, great_hall, minion, steward,
19: tribute, ambassador, cutpurse, embargo, outpost, salvager,
25: sea_hag, treasure_map };
*/

int main() {
    srand(time(NULL));
    int k[] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};
    int seed = 10;

    int numPlayers; // range 0 - MAX_PLAYERS
    int playerNumber; // range 0 - numPlayers

    /* int treasureInDeck; */
    /* int treasureCards[] = {copper, silver, gold}; */
    /* int szTreasureCards = sizeof(treasureCards) / sizeof(treasureCards[0]); */
    /* int tcIdx; */

    int i;
    for (i = 0; i < NUMTESTS; i++) {
	struct gameState state;
	numPlayers = randi(2, MAX_PLAYERS);
	playerNumber = randi(0, numPlayers);

	initializeGame(numPlayers, k, seed, &state);
	state.handCount[playerNumber] = 5;
	state.discardCount[playerNumber] = 0;
	state.whoseTurn = playerNumber;

	if (i + 1 == NUMTESTS)
	    state.deckCount[playerNumber] = 2;
	else
	    state.deckCount[playerNumber] = randi(0, MAX_DECK);

	/* treasureInDeck = randi(0, state.deckCount[playerNumber]); */
	/* for (i = 0; i < treasureInDeck; i++) { */
	/*     tcIdx = randi(0, szTreasureCards); */
	/*     state.deck[playerNumber][i] = treasureCards[tcIdx]; */
	/*     state.supplyCount[treasureCards[tcIdx]]--; */
	/* } */

	testCardAdventurer(&state);
    }

    return 0;
}

void testCardAdventurer(struct gameState *state) {
    int handPos;  // range 0 - 4
    int choice1, choice2, choice3;  // range 0 - 26 (CARD values)
    int bonus;  // 0 - MAX_RAND

    handPos = 1;
    bonus = 0;
    choice1 = choice2 = choice3 = 0;
    printf("pre game state\n");
    showGameState(state);
    cardEffect(adventurer, choice1, choice2, choice3, state, handPos, &bonus); 
    endTurn(state);
    printf("post game state\n");
    showGameState(state);
}
