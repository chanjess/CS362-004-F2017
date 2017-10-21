#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define MAXLEN 5

char str[MAXLEN];

/*
 * char inputChar()
 * produces a random int in the range [33, 126) and returns the
 * associated ascii char
 * based on a random number generator from CS 475
 */
char inputChar()
{
    // TODO: rewrite this function
    /* 32 is ascii ' ', 125 is '}' */
    float r = rand(), low = 32, high = 126;
    int tmp = (int)(low + r * (high - low) / (float)RAND_MAX);
    return (char)tmp;
}


char *inputString()
{
    // TODO: rewrite this function
    memset(str, '\0', MAXLEN);
    /* int len = (rand() % MAXLEN) + 1; */
    int len = 5;
    int i;

    float r;
    float low = 97, high = 123;
    int tmp;
    for (i = 0; i < len; i++) {
	r = rand();
	tmp = (int)(low + r * (high - low) / (float)RAND_MAX);
	str[i] = (char)tmp;
    }

    return str;
}

void testme()
{
    int tcCount = 0;
    char *s;
    char c;
    int state = 0;
    while (1)
    {
	tcCount++;
	c = inputChar();
	s = inputString();
	printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

	if (c == '[' && state == 0) state = 1;
	if (c == '(' && state == 1) state = 2;
	if (c == '{' && state == 2) state = 3;
	if (c == ' '&& state == 3) state = 4;
	if (c == 'a' && state == 4) state = 5;
	if (c == 'x' && state == 5) state = 6;
	if (c == '}' && state == 6) state = 7;
	if (c == ')' && state == 7) state = 8;
	if (c == ']' && state == 8) state = 9;
	if (s[0] == 'r' && s[1] == 'e'
		&& s[2] == 's' && s[3] == 'e'
		&& s[4] == 't' && s[5] == '\0'
		&& state == 9)
	{
	    printf("error ");
	    exit(200);
	}
    }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
