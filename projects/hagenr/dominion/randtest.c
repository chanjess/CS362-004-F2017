#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

/* global constants */
#define NUMTESTS 1
#define MINCARDS 3

/* function prototypes */
void testCardAdventurer(struct gameState *state);
int randi(int low, int high);
void showGameState(struct gameState *state);


/* k = { 7, 9, 22, 14, 17, 11, 21, 25, 19, 13 }
    enum CARD {curse, estate, duchy, province, copper, silver, gold,
7:  adventurer, council_room, feast, gardens, mine, remodel,
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

    int i;
    for (i = 0; i < NUMTESTS; i++) {
	struct gameState state;
	numPlayers = randi(2, MAX_PLAYERS);
	playerNumber = randi(0, numPlayers);

	// initializing this way is silly...
	struct gameState G;
	printf("\nrandom game state\n");
    	for (i = 0; i < sizeof(struct gameState); i++) {
	    ((char*)&G)[i] = floor(Random() * 256);
	}
	
	G.deckCount[playerNumber] = randi(MINCARDS, MAX_DECK);
	G.discardCount[playerNumber] = randi(0, MAX_DECK - G.deckCount[playerNumber]);
	G.handCount[playerNumber] = randi(0, MAX_DECK - G.deckCount[playerNumber] - G.discardCount[playerNumber]);
	G.numPlayers = numPlayers;
	G.whoseTurn = playerNumber;

	printf("\npre gameState\n");
	showGameState(&G);
	
	for (i = 0; i < G.deckCount[playerNumber]; i++)
	    G.deck[playerNumber][i] = randi(curse, treasure_map);
	
	for (i = 0; i < G.discardCount[playerNumber]; i++)
	    G.discard[playerNumber][i] = randi(curse, treasure_map);
	
	for (i = 0; i < G.handCount[playerNumber]; i++)
	    G.hand[playerNumber][i] = randi(curse, treasure_map);
/*
	printf("\nrandom player 2 deck\n");
	for (i = 0; i < G.deckCount[playerNumber]; i++)
	    printf("card %d: %d\n", i, G.deck[playerNumber][i]);
	printf("\nrandom player 2 discards\n");
	for (i = 0; i < G.discardCount[playerNumber]; i++)
	    printf("card %d: %d\n", i, G.discard[playerNumber][i]);
	printf("\nrandom player 2 hand\n");
	for (i = 0; i < G.handCount[playerNumber]; i++)
	    printf("card %d: %d\n", i, G.hand[playerNumber][i]);
*/
	int b = 0;
	int handPosn = randi(0, G.handCount[playerNumber]);
	G.hand[playerNumber][handPosn] = adventurer;

	cardEffect(adventurer, 0, 0, 0, &G, handPosn, &b);
	/* endTurn(&G); */
	printf("\npost gameState\n");
	showGameState(&G);
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

    /* printf("played cards: "); */
    /* for (i = 0; i < MAX_DECK; i++) */
	/* printf("%d ", state->playedCards[i]); */
    /* printf("\n"); */
}

/* 
 * int randi(int low, int high)
 * returns a random integer in range [low, high)
 * based on algorithm discussed in CS 475
 */
int randi(int low, int high) {
    double r = rand();
    return (int)(low + r * (high - low) / (double)RAND_MAX);
}
