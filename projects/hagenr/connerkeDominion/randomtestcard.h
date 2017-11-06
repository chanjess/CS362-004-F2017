//
//  randomtestcard.h
//  assignment-4
//
//  Created by Kevin Conner on 10/28/17.
//  Copyright © 2017 Kevin Conner. All rights reserved.
//

#ifndef randomtestcard_h
#define randomtestcard_h

#include "dominion.h"

int kcardcheck(int [], int []);
void randomDrawDiscard(struct gameState *);
void retvaluecheck(int);
void handcheck(struct gameState*, struct gameState*);
void deckcheck(struct gameState*, struct gameState*);
void discardcheck(struct gameState*, struct gameState*);
void opponenthandcheck(struct gameState*, struct gameState*);

#endif /* randomtestcard_h */
