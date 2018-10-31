#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NUMBER_OF_DOGS 9
#define NAME_PREFIX "Dog "

typedef struct DOG {
	char name[100];
	int amount;
	int odds;
} DOG;

typedef struct BET {
	int number;
	int wager;
	char betOn[100];
} BET;

typedef struct RACE {
	bool won;
	BET bet;
	DOG first;
	struct RACE *next;
} RACE;

DOG dogList[NUMBER_OF_DOGS];
RACE* start = NULL;
int funds = 0;

void swapDogs(DOG* i, DOG* j) {
	DOG t = *i;
	*i = *j;
	*j = t;
}

void sortDogs() {
	// by name field
	for (int i = 0; i < NUMBER_OF_DOGS - 1; i++) {
		for (int j = i + 1; j < NUMBER_OF_DOGS; j++) {
			if (strcmp(dogList[i].name, dogList[j].name) > 0) {
				swapDogs(&dogList[i], &dogList[j]);
			}
		}
	}
}

int searchByName(char name[]) {
	sortDogs();
	int index = -1;
	int high = NUMBER_OF_DOGS - 1;
	int low = 0;
	while (low <= high) {
		int mid = (low + high) / 2;
		if (strcmp(dogList[mid].name, name) == 0) {
			index = mid;
			break;
		} else if (strcmp(dogList[mid].name, name) > 0) {
			high = mid - 1;
		} else if (strcmp(dogList[mid].name, name) < 0) {
			low = mid + 1;
		}
	}
	return index;
}

void shuffleDogs() {
	// unique randomized shuffle
	srand(time(NULL));
	for (int i = 0; i < NUMBER_OF_DOGS - 1; i++) {
		int j = rand() % (i + 1);
		swapDogs(&dogList[i], &dogList[j]);
	}
}

bool computeWinner(BET* bet, RACE* race) {
	DOG first = race->first;
	if (strcmp(bet->betOn, first.name) == 0) {
		int index = searchByName(bet->betOn);
		DOG d = dogList[index];
		int wager = d.amount * bet->wager;
		funds = funds + wager;
		printf("\n\nYay! %s came 1st in the race. You won %d", bet->betOn,
				wager);
		printf("\nUpdated funds: %d", funds);
		return true;
	} else {
		int index = searchByName(bet->betOn);
		DOG d = dogList[index];
		int wager = d.amount * bet->wager;
		funds = funds - wager;
		printf("\n\nBad luck! %s didn't win the race. You lost %d", bet->betOn,
				wager);
		printf("\nUpdated funds: %d", funds);
		return false;
	}
}

void race(BET bet) {
	shuffleDogs();
	RACE* ndptr = (RACE*) malloc(sizeof(RACE));
	ndptr->first = dogList[0];
	ndptr->bet = bet;
	ndptr->next = NULL;
	ndptr->won = computeWinner(&bet, ndptr);
	// store outcome
	if (start == NULL) {
		start = ndptr;
	} else {
		RACE* ptr = start;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = ndptr;
		free(ptr);
	}
}

void results() {
	printf("\n\nRACE RESULTS SO FAR");
	printf(
			"\n-------------------------------------------------------------------------------------------");
	RACE* ptr = start;
	while (ptr->next != NULL) {
		BET bet = ptr->bet;
		DOG dog = ptr->first;
		printf("\nRace number %d", bet.number);
		printf("\nRace status %s", ptr->won ? "WON" : "LOST");
		printf("\nBet on %s", dog.name);
		printf(
				"\n-------------------------------------------------------------------------------------------");
		ptr = ptr->next;
	}
	BET bet = ptr->bet;
	DOG dog = ptr->first;
	printf("\nRace number %d", bet.number);
	printf("\nRace status %s", ptr->won ? "WON" : "LOST");
	printf("\nBet on %s", dog.name);
	printf(
			"\n-------------------------------------------------------------------------------------------");
}

void displayDogs() {
	//sortDogs();
	printf("\n\n%-20s | %-20s | %-20s", "Dog Name", "Payout",
			"Odds of Winning");
	char payoutSuffix[] = " to 1";
	char oddsSuffix[] = " %";
	for (int i = 0; i < NUMBER_OF_DOGS; i++) {
		char payout[10];
		char odds[10];
		sprintf(payout, "%d", dogList[i].amount);
		strcat(payout, payoutSuffix);
		sprintf(odds, "%d", dogList[i].odds);
		strcat(odds, oddsSuffix);
		printf("\n%-20s | ", dogList[i].name);
		printf("%-20s | ", payout);
		printf("%-20s", odds);
	}
}

void displayBankingMenu() {
	printf("\n\nBANKING MENU");
	printf("\n[W]ithdraw funds");
	printf("\n[D]eposit funds");
	printf("\n[E]xit bank");
	printf("\nPlease make a selection: ");
}

void bank() {
	bool flag = true;
	while (flag) {
		displayBankingMenu();
		char option;
		int amount;
		scanf(" %c", &option);
		printf(
				"\n-------------------------------------------------------------------------------------------");
		switch (option) {
		case 'W':
			// withdraw funds
			printf("\nAmount: ");
			scanf(" %d", &amount);
			if (amount > funds) {
				printf("\nInsufficient funds %d for withdrawing %d", funds,
						amount);
			} else {
				funds = funds - amount;
				printf("\nUpdated funds: %d", funds);
			}
			break;
		case 'D':
			// deposit funds
			printf("\nAmount: ");
			scanf(" %d", &amount);
			funds = funds + amount;
			printf("\nUpdated funds: %d", funds);
			break;
		case 'E':
			flag = false;
			break;
		default:
			printf("\nUnrecognized selection %c", option);
			break;
		}
	}
}

void gamble(int count) {
	int wager = 0;
	//char* name = "";
	char name[100];
	printf("\nEnter wager: ");
	scanf(" %d", &wager);
	printf("\nDogs on race track: ");
	displayDogs();
	printf("\n\nEnter a dog's name: ");
	scanf(" %[^\n]s", &name);
	getchar();
	int index = searchByName(name);
	if (index == -1) {
		printf("\n\n %s is not racing!", name);
	} else {
		BET* bet = (BET*) malloc(sizeof(BET));
		strcpy(bet->betOn, name);
		bet->number = count;
		bet->wager = wager;
		race(*bet);
		free(bet);
	}

}

void init(int amounts[], int odds[]) {
	for (int i = 0; i < NUMBER_OF_DOGS; i++) {
		char index[10];
		sprintf(index, "%d", i + 1);
		char name[] = "Dog ";
		strcat(name, index);
		dogList[i].amount = amounts[i];
		dogList[i].odds = odds[i];
		strcpy(dogList[i].name, name);
	}
}

void displayTrackMenu() {
	printf("\n\nDOG TRACK MENU");
	printf("\n[G]amble");
	printf("\n[B]anking");
	printf("\n[R]esults of race");
	printf("\n[L]eave");
	printf("\nPlease make a selection: ");
}

void track() {
	bool flag = true;
	int count = 0;
	while (flag) {
		displayTrackMenu();
		char option;
		scanf(" %c", &option);
		printf(
				"\n-------------------------------------------------------------------------------------------");
		switch (option) {
		case 'G':
			gamble(++count);
			break;
		case 'B':
			bank();
			break;
		case 'R':
			// display race outcomes
			results();
			break;
		case 'L':
			printf("\nLeaving the dog track!");
			flag = false;
			break;
		default:
			printf("\nUnrecognized selection %c", option);
			break;
		}
	}
}

int main(void) {
	int amounts[] = { 2, 5, 10, 15, 50, 20, 10, 5, 3 };
	int odds[] = { 40, 10, 8, 6, 1, 4, 8, 10, 13 };
	init(amounts, odds);
	track();
	return 0;
}
