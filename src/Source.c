#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "Eval.h"
#include "outs.h"
#include "tests.h" // used to test edge cases


void generateDeck();
void shuffleDeck();
void generatePlayer(int inputBalance);
int selectStartingPool();
int generateBlinds();
void turnStructure(int turn, int playerAmount);
void findDealer();
void setNewDealer();
void dealCard();
void turn();
void flop();
void river();
int playerTurn(int highestBet, int currentTurn);
int playerTurnStop(int highestBet);
void endTurnRemovals();
void distributePool(int pool);
void winCondition();
void clearTable();
void resetTurnCounter();

struct Card table[5];
struct Deck* head = NULL;
struct Deck* shuffledHead = NULL;
struct Deck* newCard = NULL;
struct Deck* tmpDeck = NULL;
struct Deck* tmpDeckShuffle = NULL;
struct Deck* ERRORCHECKER = NULL;
struct Deck* tmpCards = NULL;

struct Player allPlayers[8];
struct Player* newPlayer;
struct Player* tmpPlayer;
struct Player* playerHead;

int STATE = 1;
int foldCounter = 0;
int turnNumber = 1;
int pool = 0;
int maxPlayerAmount = 8;
int currentPlayerAmount = 0;
int roundWinners[8];


/*
#######################################################################################################
TEST FUNCTION
#######################################################################################################
*/

int main(void) {

	struct Card sevCardComb[7];
	generateDeck();
	shuffleDeck();	
	initializeComb();
	reInitCurrentHand();	
	clearTable();
	generatePlayer(0);
	tmpCards = shuffledHead;
	generateRandomHands(tmpPlayer, tmpCards, currentPlayerAmount);

	
	tmpPlayer = playerHead;
	FILE *fptr;
	fptr = fopen("../out/Tests.txt", "w");
	fprintf(fptr, "##########################################################################################\n");
	fprintf(fptr, "######################################## PRE-FLOP ########################################\n");
	fprintf(fptr, "##########################################################################################\n");
	fclose(fptr);
	for (int i = 0; i < currentPlayerAmount; i++){
		tmpPlayer->handVal = evaluateHand(tmpPlayer, table, 0);
		memcpy(sevCardComb, getTmpCards(), sizeof(sevCardComb));
		int outs;
		printSortedCard();
		printBestHand(tmpPlayer->bestHand);
		printHandVal(tmpPlayer->handVal);
		tmpPlayer->outs = getOuts(*tmpPlayer, table, shuffledHead);
		writeTestResults(tmpPlayer, sevCardComb, table);
		printf("ERROR CHECKER:\tChance of hitting an out: %d%% \n", calcOutsPercentage(*tmpPlayer, table));
		reInitCurrentHand();
		tmpPlayer = tmpPlayer -> next;
		tmpCards = tmpCards -> next -> next -> next; 
	}
	for (int i = 0; i < currentPlayerAmount; i++){
		tmpCards = tmpCards -> next -> next;
	}

	generateFlop(table, tmpCards);
	fptr = fopen("../out/Tests.txt", "a");
	fprintf(fptr, "##########################################################################################\n");
	fprintf(fptr, "######################################## FLOP ############################################\n");
	fprintf(fptr, "##########################################################################################\n");
	fclose(fptr);
	for (int i = 0; i < currentPlayerAmount; i++){
		tmpPlayer->handVal = evaluateHand(tmpPlayer, table, 0);
		memcpy(sevCardComb, getTmpCards(), sizeof(sevCardComb));
		int outs;
		printSortedCard();
		printBestHand(tmpPlayer->bestHand);
		printHandVal(tmpPlayer->handVal);
		tmpPlayer->outs = getOuts(*tmpPlayer, table, shuffledHead);
		writeTestResults(tmpPlayer, sevCardComb, table);
		printf("ERROR CHECKER:\tChance of hitting an out: %d%% \n", calcOutsPercentage(*tmpPlayer, table));
		reInitCurrentHand();
		tmpPlayer = tmpPlayer -> next;
	}

	tmpCards = tmpCards -> next -> next -> next;
	generateTurn(table, tmpCards);
	fptr = fopen("../out/Tests.txt", "a");
	fprintf(fptr, "##########################################################################################\n");
	fprintf(fptr, "######################################## TURN ############################################\n");
	fprintf(fptr, "##########################################################################################\n");
	fclose(fptr);
	for (int i = 0; i < currentPlayerAmount; i++){
		tmpPlayer->handVal = evaluateHand(tmpPlayer, table, 0);
		memcpy(sevCardComb, getTmpCards(), sizeof(sevCardComb));
		int outs;
		printSortedCard();
		printBestHand(tmpPlayer->bestHand);
		printHandVal(tmpPlayer->handVal);
		tmpPlayer->outs = getOuts(*tmpPlayer, table, shuffledHead);
		writeTestResults(tmpPlayer, sevCardComb, table);
		printf("ERROR CHECKER:\tChance of hitting an out: %d%% \n", calcOutsPercentage(*tmpPlayer, table));
		reInitCurrentHand();
		tmpPlayer = tmpPlayer -> next;
	}

	tmpCards = tmpCards -> next -> next;
	generateRiver(table, tmpCards);
	fptr = fopen("../out/Tests.txt", "a");
	fprintf(fptr, "##########################################################################################\n");
	fprintf(fptr, "######################################## RIVER ###########################################\n");
	fprintf(fptr, "##########################################################################################\n");
	fclose(fptr);
	for (int i = 0; i < currentPlayerAmount; i++){
		tmpPlayer->handVal = evaluateHand(tmpPlayer, table, 0);
		memcpy(sevCardComb, getTmpCards(), sizeof(sevCardComb));
		int outs;
		printSortedCard();
		printBestHand(tmpPlayer->bestHand);
		printHandVal(tmpPlayer->handVal);
		tmpPlayer->outs = getOuts(*tmpPlayer, table, shuffledHead);
		writeTestResults(tmpPlayer, sevCardComb, table);
		printf("ERROR CHECKER:\tChance of hitting an out: %d%% \n", calcOutsPercentage(*tmpPlayer, table));
		reInitCurrentHand();
		tmpPlayer = tmpPlayer -> next;
	}

	int winnerIndex;
	winnerIndex = compareHands(currentPlayerAmount, tmpPlayer);
	writeComparisonTest(winnerIndex);

}




void generatePlayer(int inputBalance) {

	int userInput = 0;

	printf("\nChoose the amount of players for the Poker Game. \nThe amount must be between 2-8 players.\n");
	printf("Input: ");
	scanf("%d", &userInput);

	playerHead = (struct Player*)malloc(sizeof(struct Player)); // GENERATE HEAD FOR DOUBLELINKED LIST
	playerHead->last = NULL;
	playerHead->next = NULL;
	playerHead->inPlay = 1;
	playerHead->playerNumber = 1;
	playerHead->balance = inputBalance;
	playerHead->dealer = 1;
	playerHead->card1.cardDrawn = 0;
	playerHead->card2.cardDrawn = 0;
	playerHead->firstRoundTurn = 1;
	playerHead->currentBet = 0;
	playerHead->totalBet = 0;
	playerHead->fold = 0;
	playerHead->kicker = 0;
	playerHead->outs = 0;

	tmpPlayer = playerHead;

	printf("\n\tGenerated Player: %d\n", tmpPlayer->playerNumber);

	for (int i = 1; i < userInput; i++) {

		newPlayer = (struct Player*)malloc(sizeof(struct Player));

		newPlayer->last = tmpPlayer;
		newPlayer->next = NULL;
		newPlayer->inPlay = 1;
		newPlayer->playerNumber = i + 1;
		newPlayer->last->next = newPlayer;
		newPlayer->balance = inputBalance;
		newPlayer->card1.cardDrawn = 0;
		newPlayer->card2.cardDrawn = 0;
		newPlayer->firstRoundTurn = 1;
		newPlayer->currentBet = 0;
		newPlayer->totalBet = 0;
		newPlayer->fold = 0;
		newPlayer->kicker = 0;
		newPlayer->outs = 0;
		newPlayer->dealer = 0;

		tmpPlayer = newPlayer;
		currentPlayerAmount = i + 1;

		printf("\n\tGenerated ASD Player: %d\n", newPlayer->playerNumber);
	}

	tmpPlayer = playerHead;

	newPlayer->next = playerHead;
	playerHead->last = tmpPlayer;
}



// TURN FUNCTIONS
void generateDeck() {

	int valueOfCard = 1;
	int typeOfCard = 1;
	int order = 1;

	printf("\nGenerating new deck!\n\n");

	head = (struct Deck*)malloc(sizeof(struct Deck)); // GENERATE HEAD FOR DOUBLELINKED LIST
	head->last = NULL;
	head->next = NULL;
	head->cardInfo.cardVal = valueOfCard;
	head->cardInfo.cardType = typeOfCard;
	head->deckPlacement = order;

	order++;
	valueOfCard++;
	tmpDeck = head;
	printf("Cardtype: %d Cardvalue: %d\n", tmpDeck->cardInfo.cardType, tmpDeck->cardInfo.cardVal);

	for (int i = 0; i < 51; i++) { // GENERATE THE REST OF THE DECK

		newCard = (struct Deck*)malloc(sizeof(struct Deck));

		newCard->last = tmpDeck;
		newCard->next = NULL;
		newCard->cardInfo.cardVal = valueOfCard;
		newCard->cardInfo.cardType = typeOfCard;
		newCard->cardDrawn = 0;
		newCard->deckPlacement = order;
		valueOfCard++;
		order++;

		newCard->last->next = newCard;

		tmpDeck = newCard;
		printf("Placement: %d Cardtype: %d Cardvalue: %d\n", tmpDeck->deckPlacement, tmpDeck->cardInfo.cardType, tmpDeck->cardInfo.cardVal);


		if (valueOfCard == 14) {
			valueOfCard = 1;
			typeOfCard++;
		}
	}

	printf("\nDeck complete!\n\n");
}

void shuffleDeck() {

	int counter = 1;
	int shufflesLeft = 52;
	int randomNumber;
	time_t t;
	srand((unsigned)time(&t));

	randomNumber = rand() % shufflesLeft;
	shufflesLeft = shufflesLeft - 1;

	printf("Shuffling the new Deck!\n\n");

	tmpDeck = head;

	if (randomNumber == 0) {
		head = head->next;
		shuffledHead = tmpDeck;

		tmpDeck->next->last = NULL;
		tmpDeck->next = NULL;

	}
	else {
		for (int i = 1; i <= randomNumber; i++) {
			tmpDeck = tmpDeck->next;
		}

		if (tmpDeck->next != NULL) {
			tmpDeck->next->last = tmpDeck->last;
			tmpDeck->last->next = tmpDeck->next;
		}
		else {
			tmpDeck->last->next = tmpDeck->next;
		}

		tmpDeck->next = NULL;
		tmpDeck->last = NULL;
		shuffledHead = tmpDeck;

	}

	shuffledHead->deckPlacement = counter;
	counter++;

	tmpDeck = head;
	tmpDeckShuffle = shuffledHead;

	while (shufflesLeft != 0) {

		randomNumber = rand() % shufflesLeft;
		shufflesLeft = shufflesLeft - 1;

		if (randomNumber == 0 && shufflesLeft == 0) {

			tmpDeck->next = NULL;
			tmpDeck->last = tmpDeckShuffle;
			tmpDeck->last->next = tmpDeck;
			tmpDeck->deckPlacement = counter;
		}

		if (randomNumber == 0 && shufflesLeft != 0) {
			head = head->next;

			tmpDeck->next->last = NULL;
			tmpDeck->next = NULL;

			tmpDeckShuffle->next = tmpDeck;
			tmpDeck->last = tmpDeckShuffle;

			tmpDeck->deckPlacement = counter;

		}
		else if (shufflesLeft != 0) {
			for (int i = 1; i <= randomNumber; i++) {
				tmpDeck = tmpDeck->next;
			}

			if (tmpDeck->next != NULL) {
				tmpDeck->next->last = tmpDeck->last;
				tmpDeck->last->next = tmpDeck->next;
			}
			else {
				tmpDeck->last->next = tmpDeck->next;
			}

			tmpDeckShuffle->next = tmpDeck;
			tmpDeck->last = tmpDeckShuffle;
			tmpDeck->next = NULL;
		}
		tmpDeck->deckPlacement = counter;

		tmpDeckShuffle = tmpDeckShuffle->next;
		tmpDeck = head;
		counter++;
	}

	ERRORCHECKER = shuffledHead;
	printf("SHUFFLED DECK - Placement: %d Cardtype: %d Cardvalue: %d nextPTR: %p\n", ERRORCHECKER->deckPlacement, ERRORCHECKER->cardInfo.cardType, ERRORCHECKER->cardInfo.cardVal, ERRORCHECKER->next);
	while (ERRORCHECKER->next != NULL) {

		ERRORCHECKER = ERRORCHECKER->next;
		printf("SHUFFLED DECK - Placement: %d Cardtype: %d Cardvalue: %d nextPTR: %p\n", ERRORCHECKER->deckPlacement, ERRORCHECKER->cardInfo.cardType, ERRORCHECKER->cardInfo.cardVal, ERRORCHECKER->next);

	}

	printf("\nShuffling deck complete!\n\n");

}

void clearTable() {
	int val = -2;
	for (int i = 0; i < 5; i++) {
		table[i].cardVal = val;
		table[i].cardType = val;
		val -= 2;
	}
}