#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_GUESSES 20
#define PHRASE_LENGTH 17
#define SPACE_COUNT 2
const char* secretPhrase = "today hello world";
bool guessStatus[PHRASE_LENGTH];

int guessAttempt = 0;
int charactersUnveiled = 0;
char lastGuess = '\0';

bool displayMessage() {
	bool cont = true;
	//printf("\nCharacters unveiled %d\n", charactersUnveiled);
	if(guessAttempt > MAX_GUESSES) {
		printf("\nSorry you have exhausted all attempts! The correct phrase is: ");
		for(int i = 0 ; i < PHRASE_LENGTH ; i++) {
			if(!guessStatus[i]) {
				guessStatus[i] = true;
			}
		}
		cont = false;
	} else if((charactersUnveiled + SPACE_COUNT) < PHRASE_LENGTH) {
		printf("\nPhrase is: ");
	} else if((charactersUnveiled + SPACE_COUNT) == PHRASE_LENGTH) {
		printf("\nCongratulations! In %d attempts you have guessed correctly the phrase: ", guessAttempt);
		cont = false;
	}
	return cont;
}

bool displayGuess() {
	bool cont = displayMessage();
	for (int i = 0; i < PHRASE_LENGTH ; i++) {
		if (secretPhrase[i] == 32) {
			printf(" ");
		} else if (guessStatus[i]) {
			printf("%c", secretPhrase[i]);
		} else {
			printf("-");
		}
	}
	printf("\n");
	return cont;
}

void makeGuess(char c) {
	for (int i = 0; i < PHRASE_LENGTH ; i++) {
		if(secretPhrase[i] == 32) {
			guessStatus[i] = true;
		} else if (c == secretPhrase[i] && !guessStatus[i]) {
			guessStatus[i] = true;
			charactersUnveiled++;
		}
	}
	lastGuess = c;
}

char attemptGuess() {
	displayGuess(lastGuess);
	printf("\nGuess a character: ");
	char g = '\0';
	scanf(" %c", &g);
	printf("\n");
	guessAttempt++;
	return g;
}

/*int main(void) {
	while (guessAttempt <= MAX_GUESSES) {
		char c = attemptGuess();
		makeGuess(c);
		if(!displayGuess()) {
			break;
		}
	}
	return 0;
}
*/
