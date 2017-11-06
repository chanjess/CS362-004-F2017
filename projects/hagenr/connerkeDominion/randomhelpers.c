// randomhelpers.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dominion_helpers.h"
#include "randomhelpers.h"
#include "rngs.h"

/*
 * void randomizeGameState(struct gameState *state)
 * pass in a pointer to a blank gameState struct
 * description: assign reasonable random values to struct data members
 */
void randomizeGameState(struct gameState *state) {
    int numPlayers = randi(2, MAX_PLAYERS); // range 0 - MAX_PLAYERS
    int playerNumber = randi(0, numPlayers);  // range 0 - numPlayers

    int i;
    // assign random garbage, from the class lecture
    for (i = 0; i < sizeof(struct gameState); i++) {
	((char*)state)[i] = floor(Random() * 256);
    }

    // assign reasonable values to the salient data members
    state->deckCount[playerNumber] = randi(MINCARDS, MAX_DECK);
    state->discardCount[playerNumber] = randi(0, MAX_DECK - state->deckCount[playerNumber]);
    state->handCount[playerNumber] = randi(0, MAX_DECK - state->deckCount[playerNumber] - state->discardCount[playerNumber]);
    state->playedCardCount = 0; // this one took a long time to find!!
    state->numPlayers = numPlayers;
    state->whoseTurn = playerNumber;

    // assign random cards to deck, discard pile, and hand
    for (i = 0; i < state->deckCount[playerNumber]; i++)
	state->deck[playerNumber][i] = randi(curse, treasure_map);
    for (i = 0; i < state->discardCount[playerNumber]; i++)
	state->discard[playerNumber][i] = randi(curse, treasure_map);
    for (i = 0; i < state->handCount[playerNumber]; i++)
	state->hand[playerNumber][i] = randi(curse, treasure_map);
}

/* 
 * int randi(int low, int high)
 * description: returns a random integer in range [low, high)
 * based on algorithm discussed in CS 475
 */
int randi(int low, int high) {
    double r = rand();
    return (int)(low + r * (high - low) / (double)RAND_MAX);
}

/*
 * void showGameState(struct gameState *state) 
 * pass in a pointer to a gameState struct
 * description: prints out the data members of the gameState struct, very
 *   helpful for debugging
 */ 
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

