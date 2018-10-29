#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_PLAYERS 10
#define INITIAL_PLAYERS 5

typedef enum {
	GOALKEEPER, FORWARD, BACK, MIDFIELDER
} POSITION;

typedef struct {
	int jerseyNumber;
	char playerName[100];
	double playerRating;
	POSITION playerPosition;
} playerData;

playerData playerList[NUMBER_OF_PLAYERS];
int count = 0;

void sortPlayers() {
	for (int i = 0; i < count - 1; i++) {
		for (int j = i + 1; j < count; j++) {
			if (playerList[i].jerseyNumber > playerList[j].jerseyNumber) {
				playerData t = playerList[i];
				playerList[j] = playerList[i];
				playerList[i] = t;
			}
		}
	}
}

void sortJerseys(int* arr, int len) {
	for (int i = 0; i < len - 1; i++) {
		for (int j = i + 1; j < len; j++) {
			if (arr[i] > arr[j]) {
				int t = arr[i];
				arr[i] = arr[j];
				arr[j] = t;
			}
		}
	}
	sortPlayers();
}

bool jerseyValid(int playeJerseyNumber) {
	if (playeJerseyNumber < 1 || playeJerseyNumber > 99) {
		return false;
	} else {
		return true;
	}
}

bool ratingValid(double playerRating) {
	if (playerRating < -1.0 || playerRating > 10-1.0) {
		return false;
	} else {
		return true;
	}
}

int findPlayer(int whichPlayer, const int jerseyNumbers[], int maxJersyCount) {
	int index = -1;
	int l = 0;
	int h = maxJersyCount - 1;
	while (l <= h) {
		int m = (l + h) / 2;
		if (jerseyNumbers[m] == whichPlayer) {
			index = m;
			break;
		} else if (jerseyNumbers[m] < whichPlayer) {
			l = m + 1;
		} else if (jerseyNumbers[m] > whichPlayer) {
			h = m - 1;
		}
	}
	return index;
}

void printPlayer(char * prefix, char* suffix, playerData thisPlayer) {
	printf("\n%sName%s: %s", prefix, suffix, thisPlayer.playerName);
	printf("\n%sJersey%s #: %d", prefix, suffix, thisPlayer.jerseyNumber);
	printf("\n%sRating%s: %lf", prefix, suffix, thisPlayer.playerRating);
}

bool addPlayer(int jerseyNumbers[], int maxJersyCount) {
	char playerName[100];
	int jerseyNumber;
	double playerRating;
	printf("\nEnter player jersey number: ");
	scanf(" %d", &jerseyNumber);
	int index = findPlayer(jerseyNumber, jerseyNumbers, maxJersyCount);
	if (index == -1) {
		if (maxJersyCount >= NUMBER_OF_PLAYERS) {
			printf("\nMaximum player count of %d has been reached",
					maxJersyCount);
			return false;
		} else {
			playerData* player  =  malloc(sizeof(playerData));
			player->jerseyNumber = jerseyNumber;
			printf("\nEnter player first or nick name: ");
			scanf(" %[^\n]", player->playerName);
			getchar();
			printf("\nEnter player rating: ");
			scanf(" %lf", &player->playerRating);
			/*strcpy(player->playerName, playerName);
			player->playerRating = playerRating;*/
			playerList[count] = *player;
			int pos = findPlayer(0, jerseyNumbers, maxJersyCount);
			jerseyNumbers[pos] = jerseyNumber;
			sortJerseys(jerseyNumbers, maxJersyCount);
			return true;
		}
	} else {
		printf("\nPlayer with jersey number %d already exists", jerseyNumber);
		return false;
	}
}

bool updatePlayer(const int jerseyNumbers[], int maxJersyCount) {
	char playerName[100];
	//char *playerName;
	int jerseyNumber;
	double playerRating;
	printf("\nEnter player jersey number: ");
	scanf(" %d", &jerseyNumber);
	int index = findPlayer(jerseyNumber, jerseyNumbers, maxJersyCount);
	if (index != -1) {
		printf("\nEnter player first or nick name: ");
		scanf(" %[^\n]", playerName);
		getchar();
		printf("\nEnter player rating: ");
		scanf(" %lf", &playerRating);
		strcpy(playerList[index].playerName, playerName);
		playerList[index].playerRating = playerRating;
		return true;
	} else {
		printf("\nPlayer with jersey number %d does not exist", jerseyNumber);
		return false;
	}
}

bool displayPlayer(const int jerseyNumbers[], int maxJersyCount) {
	int jerseyNumber;
	printf("\nEnter player jersey number: ");
	scanf("%d", &jerseyNumber);
	int index = findPlayer(jerseyNumber, jerseyNumbers, maxJersyCount);
	if (index != -1) {
		playerData thisPlayer = playerList[index];
		printPlayer("", "", thisPlayer);
		return true;
	} else {
		printf("\nPlayer with jersey number %d does not exist", jerseyNumber);
		return false;
	}
}

bool removePlayer(int jerseyNumbers[], int maxJersyCount) {
	int jerseyNumber;
	printf("\nEnter player jersey number: ");
	scanf(" %d", &jerseyNumber);
	int index = findPlayer(jerseyNumber, jerseyNumbers, maxJersyCount);
	if (index != -1) {
		playerData* player  =  malloc(sizeof(playerData));
		player->jerseyNumber = 0;
		strcpy(player->playerName, "");
		player->playerRating = -1.0;
		playerList[index] = *player;
		jerseyNumbers[index] = 0;
		count--;
		return true;
	} else {
		printf("\nPlayer with jersey number %d does not exist", jerseyNumber);
		return false;
	}
}

void displayMenu() {
	printf("\nMENU");
	printf("\na - Add a new player");
	printf("\nu - Update player information");
	printf("\nr - Remove a player from the roster");
	printf("\nd - Display player information");
	printf("\np - Print the full roaster");
	printf("\ns - Print \"Star\" players");
	printf("\nq - Quit");
	printf("\nPlease make a selection:");
}

void printRoster() {
	int j = 1;
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
		playerData thisPlayer = playerList[i];
		if (thisPlayer.jerseyNumber != 0) {
			char index[10];
			sprintf(index, "%d", j++);
			char blank[] = " ";
			strcat(blank, index);
			printPlayer("Player ", blank, thisPlayer);
		}
	}
}

void printStarPlayers() {
	double starRating = -1.0;
	printf("\nEnter \"STAR\" rating: ");
	scanf(" %lf", &starRating);
	printf("\nMY STARS\n-------------");
	int j = 1;
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
		playerData thisPlayer = playerList[i];
		if (thisPlayer.jerseyNumber != 0 && thisPlayer.playerRating > starRating) {
			char index[10];
			sprintf(index, "%d", j++);
			char blank[] = " ";
			strcat(blank, index);
			printPlayer("Player ", blank, thisPlayer);
		}
	}
}

int main(void) {
	int upcomingPlayerCount = INITIAL_PLAYERS;
	int len = INITIAL_PLAYERS * 2;
	char* names[INITIAL_PLAYERS * 2] = { "mehtab", "sunil", "gurpreet",
			"jeje", "sanju", NULL, NULL, NULL, NULL, NULL };
	int jerseyNumbers[INITIAL_PLAYERS * 2] = { 5, 9, 1, 11, 6, 0, 0, 0, 0, 0 };
	double ratings[INITIAL_PLAYERS * 2] = { 55.0, 89.1, 83.7, 85.0, 77.8, -1.0, -1.0, -1.0, -1.0, -1.0 };
	sortJerseys(jerseyNumbers, len);
	for(int i = 0 ; i < INITIAL_PLAYERS ; i++) {
		playerData* player  =  malloc(sizeof(playerData));
		player->jerseyNumber = jerseyNumbers[i];
		strcpy(player->playerName, names[i]);
		player->playerRating = ratings[i];
		playerList[i] = *player;
	}
	bool flag = true;
	while (flag) {
		sortJerseys(jerseyNumbers, upcomingPlayerCount);
		displayMenu();
		char option;// = getc(stdin);
		scanf(" %c", &option);
		//getchar();
		printf(
				"\n-------------------------------------------------------------------------------------------");
		switch (option) {
		case 'a':
			if (addPlayer(jerseyNumbers, upcomingPlayerCount)) {
				upcomingPlayerCount++;
				sortJerseys(jerseyNumbers, upcomingPlayerCount);
				printf("\nPlayer added!");
			}
			break;
		case 'u':
			if (updatePlayer(jerseyNumbers, upcomingPlayerCount)) {
				printf("\nPlayer updated!");
			}
			break;
		case 'r':
			if (removePlayer(jerseyNumbers, upcomingPlayerCount)) {
				printf("\nPlayer removed!");
			}
			break;
		case 'd':
			displayPlayer(jerseyNumbers, upcomingPlayerCount);
			break;
		case 'p':
			printRoster();
			break;
		case 's':
			printStarPlayers();
			break;
		case 'q':
			printf("\nExiting program!");
			flag = false;
			break;
		default:
			printf("\nUnrecognized selection %c", option);
			break;
		}
	}
	return 0;
}

