#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

/*
 * test suite 
 * function: int isGameOver(struct gameState *state);
 * function description: returns 1 (true) if there are:
 *    no province cards left
 *    3 or more empty supply piles
 * else returns 0 (false)
 * test setup: initialize an empty gameState struct with one of each card
 *   test 1: should return 0 since there's one province card and one in each pile
 *   test 2: should return 1 since there's no province card but one in each pile
 *   test 3: should return 1 since there are 3 card piles empty, but 1 province card
 */
int main (int argc, char** argv) {
    struct gameState g;
    int i;
    int numSupplyPiles = 25;
    int actual, expected;

    /* initialize supply piles with one card */
    for (i = 0; i < numSupplyPiles; i++)
	g.supplyCount[i] = 1;

    printf("test suite for isGameOver\n");

    printf("test 1: ");
    expected = 0;
    actual = isGameOver(&g);
    printf("%s", (actual == expected) ? "PASS" : "FAIL");
    printf(" - should return 0 since there's a province card and no empty piles\n");

    printf("test 2: ");
    expected = 1;
    g.supplyCount[province] = 0;
    actual = isGameOver(&g);
    printf("%s", (actual == expected) ? "PASS" : "FAIL");
    printf(" - should return 1 since there's no province card\n");

    printf("test 3: ");
    expected = 1;
    g.supplyCount[province] = 1;
    g.supplyCount[baron] = g.supplyCount[remodel] = g.supplyCount[feast] = 0;
    actual = isGameOver(&g);
    printf("%s", (actual == expected) ? "PASS" : "FAIL");
    printf(" - should return 1 since there are 3 empty piles, but 1 province card\n");

    return 0;
}
