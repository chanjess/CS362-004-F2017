#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
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
 * test setup: create a gameState struct with random values, 'pre'. call
 *   cardEffect with a copy of 'pre' called 'post'. compare the structs and
 *   report differences
 * tests, one for each player
 *   player deck has x fewer cards - no visibility into this metric
 *   player hand has 2 more treasure cards
 *   player hand has 2 more cards
 *   player deck + discard has 2 fewer cards
 *   opponent card counts unchanged
 */

/* global constants */
#define NUMTESTS 10
#define MINCARDS 3

/* 
 * struct to hold number of mismatches between pre- & post-gameState
 */
struct errors {
    int handCount;
    int totalDeckDiscardCount;
    int treasureCount;
    int opponentCardCounts;
};

/* function prototypes */
void testCardAdventurer(struct gameState *pre, struct errors *e);
int randi(int low, int high);
void showGameState(struct gameState *state);
void randomizeGameState(struct gameState *state);

int main() {
    srand(time(NULL));

    struct errors e = {0};
    int i;
    for (i = 0; i < NUMTESTS; i++) {
	struct gameState state;
	randomizeGameState(&state);
	testCardAdventurer(&state, &e);
    }

    printf("hand count errors: %d, total: %d, treasure: %d, opponent: %d\n", e.handCount, e.totalDeckDiscardCount, e.treasureCount, e.opponentCardCounts);
    return 0;
}

void randomizeGameState(struct gameState *state) {
    int numPlayers = randi(2, MAX_PLAYERS); // range 0 - MAX_PLAYERS
    int playerNumber = randi(0, numPlayers);  // range 0 - numPlayers

    int i;
    for (i = 0; i < sizeof(struct gameState); i++) {
	((char*)state)[i] = floor(Random() * 256);
    }

    state->deckCount[playerNumber] = randi(MINCARDS, MAX_DECK);
    state->discardCount[playerNumber] = randi(0, MAX_DECK - state->deckCount[playerNumber]);
    state->handCount[playerNumber] = randi(0, MAX_DECK - state->deckCount[playerNumber] - state->discardCount[playerNumber]);
    state->numPlayers = numPlayers;
    state->whoseTurn = playerNumber;

    for (i = 0; i < state->deckCount[playerNumber]; i++)
	state->deck[playerNumber][i] = randi(curse, treasure_map);

    for (i = 0; i < state->discardCount[playerNumber]; i++)
	state->discard[playerNumber][i] = randi(curse, treasure_map);

    for (i = 0; i < state->handCount[playerNumber]; i++)
	state->hand[playerNumber][i] = randi(curse, treasure_map);
}

void testCardAdventurer(struct gameState *pre, struct errors *e) {
    int handPosn;  // range 0 - state->handCount[playerNumber]
    int choice1, choice2, choice3;  // range 0 - 26 (CARD values)
    int bonus;  // 0 - MAX_DECK
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
    if (preTreasureCardsInHand + 2 != postTreasureCardsInHand) e->treasureCount++;

    /* hand should have only 2 more cards */
    if (pre->handCount[playerNumber] + 2 != post.handCount[playerNumber]) e->handCount++;

    /* post deck + discard should be 2 less than pre */
    if (pre->deckCount[playerNumber] + pre->discardCount[playerNumber] != 
	    post.deckCount[playerNumber] + post.discardCount[playerNumber] + 2) { e->totalDeckDiscardCount++; }

    /* opponent cards counts unchanged */
    for (i = 0; i < pre->numPlayers; i++) {
	if (i == playerNumber) continue;
	else {
	    if (pre->deckCount[i] != post.deckCount[i]) e->opponentCardCounts++;
	    if (pre->handCount[i] != post.handCount[i]) e->opponentCardCounts++;
	    if (pre->discardCount[i] != post.discardCount[i]) e->opponentCardCounts++;
	}
    }
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
