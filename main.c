#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 50

// ************************
//  Donal Burke - G00337729
//  Bingo Game Assignment
// ************************

// Create 1 card
int* create_card() {
	int *card = malloc(27 * sizeof(int));
	int blanks, i, j, random;
	for (i = 0; i<27; i++) { 
		card[i] = -1; // filling up the card with -1 for blanks
	}
	for (i = 0; i<3; i++) {
		blanks = 4;
		while (blanks != 0) {
			random = rand() % 9;
			if (!((i == 0 || i == 2) && (random == 0 || random == 8))) {
				if (card[random + i * 9] != 0) { //blank spaces are -1
					card[random + i * 9] = 0;
					blanks--;
				}
			}
		}
	}
	for (i = 0; i<9; i++) {
		blanks = 0;
		for (j = 0; j<3; j++) {
			if (card[i + 9 * j] == 0)
				blanks++;
		}
		// generating the random numbers based on index 
		while (blanks<3) {
			random = i * 10 + rand() % 9 + 1;
			if (random != card[i] && random != card[i + 9] && random != card[i + 18]) {
				if (card[i] == -1) {
					card[i] = random;
				}
				else {
					if (card[i + 9] == -1) {
						if (card[i] < random) {
							card[i + 9] = random;
						}
						else {
							card[i + 9] = card[i];
							card[i] = random;
						}
					}
					else {
						if (card[i] < random && card[i + 9] < random) {
							card[i + 18] = random;
						}
						else {
							if (card[i] < random) {
								card[i + 18] = card[i + 9];
								card[i + 9] = random;
							}
							else {
								card[i + 18] = card[i];
								card[i] = random;
								if (card[i + 9]>card[i + 18]) {
									random = card[i + 9];
									card[i + 9] = card[i + 18];
									card[i + 18] = random;
								}
							}
						}
					}
				}
				blanks++;
			}
		}
	}
	return card;

}

int* create_blankcard() {
	// allocating memory for a blank card or when loading card from a file
	int *card = malloc(27 * sizeof(int));
	int blanks, i, j, random;
	for (i = 0; i<27; i++) { 
		card[i] = -1;
	}
	return card;
}

void print_card(int* card) {
	// nice formatting for printing out the card (copied from the output pdf)
	int i, j, k;
	printf("\tCol 1\t- Col 2\t- Col 3\t- Col 4\t- Col 5\t- Col 6\t- Col 7\t- Col 8\t- Col9\n");
	for (i = 0; i<3; i++) {
		printf("Row %d\t", i);
		for (j = 0; j<9; j++) {
			k = card[i * 9 + j];
			if (k == 0) {
				printf("*");
			}
			else {
				printf("%d", card[i * 9 + j]);
			}
			if (j != 8) {
				printf("\t- ");
			}
		}
		printf("\n");
	}
}
// crossing out numbers on the card
int cross(int number, int* card) { //1 if the number is in the card
	int collom = number / 10;
	int i;
	for (i = 0; i<3; i++) {
		if (card[i * 9 + collom] == number) {
			card[i * 9 + collom] = 0;
			return 1;
		}
	}
	return 0;
}

int corners(int* card) { //left for corners
	int number = 0;
	if (card[0] != 0)
		number++;
	if (card[8] != 0)
		number++;
	if (card[18] != 0)
		number++;
	if (card[26] != 0)
		number++;
	return number;
}

int* left(int* card) { //array with left for 1, 2 and 3 lines
	int *row = malloc(3 * sizeof(int));
	int i, j, aux;
	for (i = 0; i<3; i++) {
		aux = 0;
		for (j = 0; j<9; j++) {
			if (card[i * 9 + j] != 0)
				aux++;
		}
		row[i] = aux;
	}
	if (row[0]>row[1]) {
		aux = row[0];
		row[0] = row[1];
		row[1] = aux;
	}
	if (row[0]>row[2]) {
		aux = row[0];
		row[0] = row[2];
		row[2] = aux;
	}
	if (row[1]>row[2]) {
		aux = row[1];
		row[1] = row[2];
		row[2] = aux;
	}
	row[1] += row[0];
	row[2] += row[1];
	return row;
}

void print_status(int* card) { // printing the game status
	printf("%d numbers for the four corners\n", corners(card));
	int* aux;
	aux = left(card);
	printf("%d numbers for the one line\n", aux[0]);
	printf("%d numbers for the two lines\n", aux[1]);
	printf("%d numbers for the full house\n", aux[2]);
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	// variables
	int i, j, k, l, read, cornersbefore, cornersafter;
	int* before;
	int* after;
	int nplayers, nremoved;
	FILE* fp;
	int removed[90];
	int *card[50];
	char file[20];
	while (1) {
		// First Menu
		printf("Please enter 1 to Start a new Game \nPlease enter 2 to Reload an old Game \nPlease enter 3 to exit the application\n");
		scanf("%d", &read);
		if (read == 1) { // New Game
			printf("Number of players: ");
			scanf("%d", &nplayers);
			nremoved = 0;
			for (i = 0; i<nplayers; i++) {
				card[i] = create_card();
				printf("\nPlayer %d\n", i + 1);
				print_card(card[i]);
				printf("\n");
			}
		}
		if (read == 2) { // Load game from specified file
			char fname[128];
			printf("Enter .txt file name\n");
			scanf("%123s", fname);
			strcat(fname, ".txt");
			FILE *inputf;
			inputf = fopen(fname, "r");
			while (inputf == NULL) {
				printf("That file doesnt exit!");
				printf("Please enter the filename: ");
				scanf("%s ", fname);
				inputf = fopen(fname, "r");
			}
			fscanf(inputf, "%d", &nplayers);
			fscanf(inputf, "%d", &nremoved);
			for (i = 0; i<nremoved; i++) {
				fscanf(inputf, "%d", &removed[i]);
			}
			for (i = 0; i<nplayers; i++) {
				card[i] = create_blankcard();
				for (j = 0; j<27; j++) {
					fscanf(inputf, "%d", &card[i][j]);
				}
			}
			fclose(inputf);
		}
		if (read == 3) { //Exit
			exit(0);
		}
		// Second Menu
		while (read != 4) {
			printf("Please enter 1 to Draw the next number \nPlease enter 2 to Save the game\nPlease enter 3 to Output the game's status \nPlease enter 4 to Exit the game\n");
			scanf("%d", &read);
			if (read == 1) { //Draw the numbers
				i = 0;
				while (i == 0) {
					j = rand() % 90 + 1;
					i = 1;
					for (k = 0; k<nremoved; k++) {
						if (j == removed[k]) {
							i = 0;
							break;
						}
					}
					if (i == 1) {
						removed[nremoved] = j;
						nremoved++;
					}
				}
				// Checking for winners
				printf("The number %d was drawn\n", j);
				for (i = 0; i<nplayers; i++) {
					cornersbefore = corners(card[i]);
					before = left(card[i]);
					cross(j, card[i]);
					cornersafter = corners(card[i]);
					after = left(card[i]);
					if (cornersafter == 0 && cornersbefore>0) {
						printf("The player %d has got the 4 corners.\n", i + 1);
					}
					if (after[0] == 0 && before[0]>0) {
						printf("The player %d has got 1 line.\n", i + 1);
					}
					if (after[1] == 0 && before[0]>0) {
						printf("The player %d has got 2 lines.\n", i + 1);
					}
					if (after[2] == 0 && before[0]>0) {
						printf("The player %d has full house.\nPLAYER %d WINS\n", i + 1, i + 1);
						read = 4;
					}
				}

			}
			if (read == 2) { // Save the game
				printf("Please enter the filename: ");
				scanf("%s", file);
				fp = fopen(file, "w+");
				while (fp == NULL) {
					printf("That file doesnt exit!");
					printf("Please enter the filename: ");
					scanf("%s", file);
					fp = fopen(file, "w+");
				}
				fprintf(fp, "%d\n%d\n", nplayers, nremoved);
				for (i = 0; i<nremoved; i++) {
					fprintf(fp, "%d ", removed[i]);
				}
				fprintf(fp, "\n");
				for (i = 0; i<nplayers; i++) {
					for (j = 0; j<27; j++) {
						fprintf(fp, "%d ", card[i][j]);
					}
					fprintf(fp, "\n");
				}
			}
			if (read == 3) { // Game Status
				printf("The following numbers were drawn:\n");
				for (i = 0; i<nremoved; i++) {
					printf("%d ", removed[i]);
				}
				printf("\n");
				for (i = 0; i<nplayers; i++) {
					printf("Player %d requires:\n", i);
					print_status(card[i]);
				}
			}
		}
	}
	exit(0);
}