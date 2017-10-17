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
 * test setup: call playGame in 'silent' mode with 2 players and random seed 2.
 * test 1: compare score from dominion.c to score using a struct of card counts
 *   in each pile
 */

/* struct to hold victory card counts in a player's deck */
struct cardCounts {
    int curses;
    int estates;
    int duchies;
    int provinces;
    int great_halls;
    int gardens;
};

/* function prototypes */
struct gameState playGame(int randomSeed);
int getScores(int player, struct gameState *state);
void getCardCounts(int player, int cardCount, int cards[], struct cardCounts *myDeck);

int main (int argc, char** argv) {
    struct gameState results;
    int randomSeed = 2;
    int expectedPlayer1, expectedPlayer2;
    int actualPlayer1, actualPlayer2; 

    results = playGame(randomSeed);

    printf("test suite for isGameOver\n");

    printf("test 1: ");
    expectedPlayer1 = getScores(0, &results);
    expectedPlayer2 = getScores(1, &results);
    actualPlayer1 = scoreFor(0, &results);
    actualPlayer2 = scoreFor(1, &results);
    printf("%s", (expectedPlayer1 == actualPlayer1 && expectedPlayer2 == actualPlayer2) ? "PASS" : "FAIL");
    printf(" - expected player 1: %d, got %d; expected player 2: %d, got %d\n",
	    expectedPlayer1, actualPlayer1, expectedPlayer2, actualPlayer2);

    /* confirm that scoreFor uses the wrong limit on the 'score from deck' for loop */
    /* printf("p0 discards: %d, deck: %d, hand: %d\n", results.discardCount[0], results.deckCount[0], results.deckCount[0]); */
    /* printf("p1 discards: %d, deck: %d, hand: %d\n", results.discardCount[1], results.deckCount[1], results.deckCount[1]); */

    return 0;
}

/* silent game play */
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
    // struct with all card counts of zero
    struct cardCounts myDeck = {0, 0, 0, 0, 0, 0};

    // get the number of each type of victory card in each 'pile'
    getCardCounts(player, state->handCount[player], state->hand[player], &myDeck);
    getCardCounts(player, state->discardCount[player], state->discard[player], &myDeck);
    getCardCounts(player, state->deckCount[player], state->deck[player], &myDeck);
    /* comment out previous line, uncomment out next line to verify functions get same _wrong_ score */
    /* getCardCounts(player, state->discardCount[player], state->deck[player], &myDeck); */

    score -= (myDeck.curses); 			// curse is -1 
    score += (myDeck.estates); 			// estates is +1
    score += (myDeck.great_halls); 		// great hall is +1
    score += (myDeck.duchies * 3); 		// duchy is +3
    score += (myDeck.provinces * 6); 		// province is +6
    score += (myDeck.gardens * gardenBonus); 	// garden is 0.10 * total card

    return score;
}

// count the number of victory cards in a player's deck
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

