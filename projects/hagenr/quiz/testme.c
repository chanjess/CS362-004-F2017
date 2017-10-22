#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define CHARS "[({ ax})]"
#define TERMSTRING "reset"
#define TERMSTRINGLEN strlen(TERMSTRING)

char str[TERMSTRINGLEN+1];

/*
 * char inputChar()
 * returns: a random character from a pre-defined string
 */
char inputChar()
{
    // TODO: rewrite this function
    int len = strlen(CHARS);
    return CHARS[rand() % len];
}

/*
 * char *inputString()
 * returns a string composed of randomly chosen characters 
 * from a predefined string
 */
char *inputString()
{
    // TODO: rewrite this function
    memset(str, '\0', TERMSTRINGLEN+1);
    int i;
    char tmp;

    for (i = 0; i < TERMSTRINGLEN; i++) {
	tmp = TERMSTRING[rand() % TERMSTRINGLEN];
	str[i] = tmp;
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
	if (c == ' ' && state == 3) state = 4;
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
