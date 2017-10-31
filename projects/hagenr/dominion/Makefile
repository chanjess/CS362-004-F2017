CFLAGS = -Wall -fpic -coverage -lm -g

rngs.o: rngs.h rngs.c
	gcc -c rngs.c $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard testDrawCard.c dominion.o rngs.o $(CFLAGS)

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 unittest1.c dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 unittest2.c dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 unittest3.c dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 unittest4.c dominion.o rngs.o $(CFLAGS)

cardtest1: cardtest1.c dominion.o rngs.o
	gcc -o cardtest1 cardtest1.c dominion.o rngs.o $(CFLAGS)

cardtest2: cardtest2.c dominion.o rngs.o
	gcc -o cardtest2 cardtest2.c dominion.o rngs.o $(CFLAGS)

cardtest3: cardtest3.c dominion.o rngs.o
	gcc -o cardtest3 cardtest3.c dominion.o rngs.o $(CFLAGS)

cardtest4: cardtest4.c dominion.o rngs.o
	gcc -o cardtest4 cardtest4.c dominion.o rngs.o $(CFLAGS)

randtest: randtest.c dominion.o rngs.o
	gcc -o randtest randtest.c dominion.o rngs.o $(CFLAGS)

randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
	gcc -o randomtestadventurer randomtestadventurer.c dominion.o rngs.o $(CFLAGS)

randomtestresults.out: randomtestadventurer dominion.o rngs.o
	@echo "random tests starting"
	
	@echo "Dominion Random Test Results" > randomtestresults.out
	@echo "--------------------------" >> randomtestresults.out

	@echo "randomtestadventurer.c" >> randomtestresults.out
	@./randomtestadventurer >> randomtestresults.out
	@gcov dominion.c >> randomtestresults.out

	@echo "random tests complete"

unittestresults.out: unittest1 unittest2 unittest3 unittest4 cardtest1 \
	cardtest2 cardtest3 cardtest4 dominion.o rngs.o
	@echo "unit tests starting"

	@echo "Dominion Unit Test Results" > unittestresults.out
	@echo "--------------------------" >> unittestresults.out

	@echo "unittest1.c" >> unittestresults.out
	@./unittest1 >> unittestresults.out
	@gcov dominion.c >> unittestresults.out

	@echo "unittest2.c" >> unittestresults.out
	@./unittest2 >> unittestresults.out
	@gcov dominion.c >> unittestresults.out

	@echo "unittest3.c" >> unittestresults.out
	@./unittest3 >> unittestresults.out
	@gcov dominion.c >> unittestresults.out

	@echo "unittest4.c" >> unittestresults.out
	@./unittest4 >> unittestresults.out
	@gcov dominion.c >> unittestresults.out

	@echo "cardtest1.c" >> unittestresults.out
	@./cardtest1 >> unittestresults.out
	@gcov dominion.c >> unittestresults.out

	@echo "cardtest2.c" >> unittestresults.out
	@./cardtest2 >> unittestresults.out
	@gcov dominion.c >> unittestresults.out

	@echo "cardtest3.c" >> unittestresults.out
	@./cardtest3 >> unittestresults.out
	@gcov dominion.c >> unittestresults.out

	@echo "cardtest4.c" >> unittestresults.out
	@./cardtest4 >> unittestresults.out
	@gcov dominion.c >> unittestresults.out

	@echo "unit tests complete"

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player testDrawCard testBuyCard badTestDrawCard unittest1 unittest2 \
	unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 tags \
	randomtestadventurer randtest

.PHONY: tags clean

tags:
	ctags --c-kinds=+egms `find . -name "*.[ch]" -print`

clean:
	rm -f *.o playdom player *.gcov *.gcda *.gcno *.so *.out testDrawCard \
		badTestDrawCard unittest1 unittest2 unittest3 unittest4 \
		cardtest1 cardtest2 cardtest3 cardtest4 tags randomtestadventurer \
		randtest

