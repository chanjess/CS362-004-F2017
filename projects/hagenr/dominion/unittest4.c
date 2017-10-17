#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

/*
 * test suite 
 * function: int scoreFor (int player, struct gameState *state)
 * function description: iterates through player's deck (hand, discard, deck),
 *   accumulating points for each victory card according to these rules (verified
 *   on the site http://wiki.dominionstrategy.com/index.php/Victory_point
 *     curse: -1
 *     estate: 1
 *     duchy: 3
 *     province: 6
 *     great hall: 1
 *     gardens: 1 point for every 10 cards in deck
 * test setup: call initializeGame with 2 players, random seed of 10.
 *   test 1: call update coins with standard initial game state for player 0,
 *           no bonus. initial hand has 4 coppers, coins should be 4 * 1 = 4
 *   test 2: change player 0 hand to have 2 coppers, 1 silver, 1 gold. call
 *           updateCoins with bonus of 1. coins should be 2 + 2 + 3 + 1 = 8
 */

struct cardCounts {
    int curses;
    int estates;
    int duchies;
    int provinces;
    int great_halls;
    int gardens;
};

struct gameState playGame(int randomSeed);
int getScore(int player, struct gameState *state);
int calcScore(int player, int cardCount, int cards[], int numCards);

int getScores(int player, struct gameState *state);
void getCardCounts(int player, int cardCount, int cards[], struct cardCounts *myDeck);

int main (int argc, char** argv) {
    struct gameState results;
    int randomSeed = 2;
    results = playGame(randomSeed);
    printf("Player 0: %d\nPlayer 1: %d\n", scoreFor(0, &results), scoreFor(1, &results));
    printf("P0: %d P1: %d\n", getScore(0, &results), getScore(1, &results));
    printf("P0: %d P1: %d\n", getScores(0, &results), getScores(1, &results));
    /* printf("p0 discards: %d, deck: %d, hand: %d\n", results.discardCount[0], results.deckCount[0], results.deckCount[0]); */
    /* printf("p1 discards: %d, deck: %d, hand: %d\n", results.discardCount[1], results.deckCount[1], results.deckCount[1]); */

    return 0;
}

// silent game play
struct gameState playGame(int randomSeed) {
    struct gameState G;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};

    initializeGame(2, k, randomSeed, &G);

    int money = 0;
    int smithyPos = -1;
    int adventurerPos = -1;
    int i=0;

    int numSmithies = 0;
    int numAdventurers = 0;

    while (!isGameOver(&G)) {
	money = 0;
	smithyPos = -1;
	adventurerPos = -1;
	for (i = 0; i < numHandCards(&G); i++) {
	    if (handCard(i, &G) == copper)
		money++;
	    else if (handCard(i, &G) == silver)
		money += 2;
	    else if (handCard(i, &G) == gold)
		money += 3;
	    else if (handCard(i, &G) == smithy)
		smithyPos = i;
	    else if (handCard(i, &G) == adventurer)
		adventurerPos = i;
	}

	if (whoseTurn(&G) == 0) {
	    if (smithyPos != -1) {
		playCard(smithyPos, -1, -1, -1, &G);
		money = 0;
		i=0;
		while(i<numHandCards(&G)){
		    if (handCard(i, &G) == copper){
			playCard(i, -1, -1, -1, &G);
			money++;
		    }
		    else if (handCard(i, &G) == silver){
			playCard(i, -1, -1, -1, &G);
			money += 2;
		    }
		    else if (handCard(i, &G) == gold){
			playCard(i, -1, -1, -1, &G);
			money += 3;
		    }
		    i++;
		}
	    }

	    if (money >= 8) {
		buyCard(province, &G);
	    }
	    else if (money >= 6) {
		buyCard(gold, &G);
	    }
	    else if ((money >= 4) && (numSmithies < 2)) {
		buyCard(smithy, &G);
		numSmithies++;
	    }
	    else if (money >= 3) {
		buyCard(silver, &G);
	    }

	    endTurn(&G);
	}
	else {
	    if (adventurerPos != -1) {
		playCard(adventurerPos, -1, -1, -1, &G);
		money = 0;
		i=0;
		while(i<numHandCards(&G)){
		    if (handCard(i, &G) == copper){
			playCard(i, -1, -1, -1, &G);
			money++;
		    }
		    else if (handCard(i, &G) == silver){
			playCard(i, -1, -1, -1, &G);
			money += 2;
		    }
		    else if (handCard(i, &G) == gold){
			playCard(i, -1, -1, -1, &G);
			money += 3;
		    }
		    i++;
		}
	    }

	    if (money >= 8) {
		buyCard(province, &G);
	    }
	    else if ((money >= 6) && (numAdventurers < 2)) {
		buyCard(adventurer, &G);
		numAdventurers++;
	    }else if (money >= 6){
		buyCard(gold, &G);
	    }
	    else if (money >= 3){
		buyCard(silver, &G);
	    }

	    endTurn(&G);
	}
    }

    return G;
}

int getScores(int player, struct gameState *state) {
    int score = 0;
    int numCards = fullDeckCount(player, 0, state);
    int gardenBonus = numCards / 10;
    struct cardCounts myDeck = {0, 0, 0, 0, 0, 0};

    getCardCounts(player, state->handCount[player], state->hand[player], &myDeck);
    getCardCounts(player, state->discardCount[player], state->discard[player], &myDeck);
    /* getCardCounts(player, state->deckCount[player], state->deck[player], &myDeck); */
    /* comment out previous line, uncomment out next line to verify functions get same _wrong_ score */
    getCardCounts(player, state->discardCount[player], state->deck[player], &myDeck);
    score -= (myDeck.curses);
    score += (myDeck.estates);
    score += (myDeck.great_halls);
    score += (myDeck.duchies * 3);
    score += (myDeck.provinces * 6);
    score += (myDeck.gardens * gardenBonus);
    return score;
}

void getCardCounts(int player, int cardCount, int cards[], struct cardCounts *myDeck) {
    int i;
    
    for (i = 0; i < cardCount; i++) {
	switch (cards[i]) {
	    case curse:
		myDeck->curses++;
		break;
	    case estate:
		myDeck->estates++;
		break;
	    case great_hall:
		myDeck->great_halls++;
		break;
	    case duchy:
		myDeck->duchies++;
		break;
	    case province:
		myDeck->provinces++;
		break;
	    case gardens:
		myDeck->gardens++;
		break;
	}
    }
}

int getScore(int player, struct gameState *state) {
    int score = 0;
    int numCards = fullDeckCount(player, 0, state);
    score = calcScore(player, state->handCount[player], state->hand[player], numCards);
    score += calcScore(player, state->discardCount[player], state->discard[player], numCards);
    score += calcScore(player, state->deckCount[player], state->deck[player], numCards);
    /* comment out previous line, uncomment out next line to verify functions get same _wrong_ score */
    /* score += calcScore(player, state->discardCount[player], state->deck[player], numCards); */
    return score;
}

int calcScore(int player, int cardCount, int cards[], int numCards) {
    int i;
    int score = 0;
    
    for (i = 0; i < cardCount; i++) {
	switch (cards[i]) {
	    case curse:
		score -= 1;
		break;
	    case estate:
	    case great_hall:
		score += 1;
		break;
	    case duchy:
		score += 3;
		break;
	    case province:
		score += 6;
		break;
	    case gardens:
		score += (numCards / 10);
		break;
	}
    }
    return score;
}

