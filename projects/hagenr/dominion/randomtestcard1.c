#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include "rngs.h"

/*
 * adventurer card random test
 */

int main() {
    int seed = 10;
    /* variables to be assigned random values */
    struct gameState pre, post;
    int numPlayers;  // range 0 - 3
    int handPos;  // range 0 - 4
    int choice1;  // range 0 - 26 (number of CARDs
    int choice2;
    int choice3;
    int bonus;  // 0 - MAX_RAND
    int k[] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};
    // { 7, 9, 22, 14, 17, 11, 21, 25, 19, 13 }
    enum CARD {curse, estate, duchy, province,
	       copper, silver, gold,
               7: adventurer, council_room, feast, gardens, mine, remodel,
	       13: smithy, village, baron, great_hall, minion, steward,
	       19: tribute, ambassador, cutpurse, embargo, outpost, salvager,
	       25: sea_hag, treasure_map };


    return 0;
}
