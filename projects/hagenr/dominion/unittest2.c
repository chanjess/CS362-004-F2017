#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

/*
 * test suite 
 * function: int getCost(int cardNumber);
 * function description: returns card cost based on enum value passed as argument
 *   else returns -1 if invalid card number
 * test setup: use values from http://wiki.dominionstrategy.com/index.php/List_of_cards
 *   to create an array where indexes are parallel to the CARD enum with card values
 * card:  curse estate duchy province copper silver gold adventurer council_room feast gardens mine remodel smithy village baron great_hall minion steward tribute ambassador cutpurse embargo outpost salvager sea_hag treasure_map
 * index: 0     1      2     3        4      5      6    7          8            9     10      11   12      13     14      15    16         17     18      19      20         21       22      23      24       25      26
 * value: 0     2      5     8        0      3      6    6          5            4     4       5    4       4      3       4     3          5      3       5       3          4        2       5       4        4       4
 * test passes if the getCost function returns the expected value, otherwise the test fails and reports the first mismatched card
 */
int main (int argc, char** argv) {
    int costs[] = { 0,2,5,8,0,3,6,6,5,4,4,5,4,4,3,4,3,5,3,5,3,4,2,5,4,4,4 };
    int expected, actual, i;
    int numberCards = treasure_map + 1;
    int mismatch = 0;

    for (i = 0; i < numberCards; i++) {
	expected = getCost(i);
	actual = costs[i];
	if (expected != actual) {
	    mismatch = 1;
	    break;
	}
    }

    if (mismatch) {
	printf("test 1: FAIL for card number %d, expected %d but actual value is %d\n", i, expected, actual);
    } else {
	printf("test 1: PASS for all cards\n");
    }

    return 0;
}
