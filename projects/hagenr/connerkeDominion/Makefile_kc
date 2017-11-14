CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g dominion.o rngs.o interface.o $(CFLAGS)

tests.out: randomtestadventurer.c randomtestcard1.c randomtestcard2.c dominion.o rngs.o
	echo -e "Result for running Dominion tests:\n" > randomtestresults.out

	echo "randomtestadventurer.c:" >> randomtestresults.out
	gcc -o randomtestadventurer -g randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
	./randomtestadventurer >> randomtestresults.out
	gcov -f -b dominion.c >> randomtestresults.out

	echo "randomtestcard1.c:" >> randomtestresults.out
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)
	./randomtestcard1 >> randomtestresults.out
	gcov -f -b dominion.c >> randomtestresults.out

	echo "randomtestcard2.c:" >> randomtestresults.out
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)
	./randomtestcard2 >> randomtestresults.out
	gcov -f -b dominion.c >> randomtestresults.out

all: playdom player testDrawCard testBuyCard badTestDrawCard tests.out

clean:
	rm -f *.o playdom.exe playdom player player.exe randomtestadventurer randomtestcard1 randomtestcard2 randomtestresults.out *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
	rm -r *.dSYM
