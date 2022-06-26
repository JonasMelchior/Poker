#include "Eval.h"
#include "outs.h"
#include "tests.h"
#include <string.h>
#include <stdio.h>

void generateRandomHands(struct Player *player, struct Deck* tmpCards, int currentPlayerAmount){
	for (int i = 0; i < currentPlayerAmount; i++){
		player -> card1.cardInfo.cardVal = tmpCards -> cardInfo.cardVal;
		player -> card1.cardInfo.cardType = tmpCards -> cardInfo.cardType;
		tmpCards -> cardDrawn = 1;
		tmpCards = tmpCards -> next;
		player -> card2.cardInfo.cardVal = tmpCards -> cardInfo.cardVal;
		player -> card2.cardInfo.cardType = tmpCards -> cardInfo.cardType;
		tmpCards -> cardDrawn = 1;
		tmpCards = tmpCards -> next;

		player = player -> next;
	}
}

void generateFlop(struct Card *table, struct Deck* tmpCards){
	table -> cardVal = tmpCards -> cardInfo.cardVal;
	table -> cardType = tmpCards -> cardInfo.cardType;
	tmpCards -> cardDrawn = 1;
	tmpCards = tmpCards -> next;
	(table + 1) -> cardVal = tmpCards -> cardInfo.cardVal;
	(table + 1) -> cardType = tmpCards -> cardInfo.cardType;
	tmpCards = tmpCards -> next;
	(table + 2) -> cardVal = tmpCards -> cardInfo.cardVal;
	(table + 2) -> cardType = tmpCards -> cardInfo.cardType;
	tmpCards = tmpCards -> next;
}

void generateTurn(struct Card *table, struct Deck* tmpCards){
	(table + 3) -> cardVal = tmpCards -> cardInfo.cardVal;
	(table + 3) -> cardType = tmpCards -> cardInfo.cardType;
	tmpCards -> cardDrawn = 1;
	tmpCards = tmpCards -> next;
}

void generateRiver(struct Card *table, struct Deck* tmpCards){
	(table + 4) -> cardVal = tmpCards -> cardInfo.cardVal;
	(table + 4) -> cardType = tmpCards -> cardInfo.cardType;
	tmpCards -> cardDrawn = 1;
	tmpCards = tmpCards -> next;	
}

void writeTestResults(struct Player *player, struct Card *tmpCards, struct Card *table){
	FILE *fptr;
	fptr = fopen("../out/Tests.txt", "a");

	fprintf(fptr, "## Player nr. %d ##\n", player -> playerNumber);
	fprintf(fptr, "7-card sorted sequense:\n");
	for (int j = 0; j < 7; j++){
		fprintf(fptr, "Card %d:\t##VALUE: %d\t##TYPE: %d\n", j + 1, tmpCards[j].cardVal, tmpCards[j].cardType);
	}
	fprintf(fptr, "Best hand combination:\n");
	for (int j = 0; j < 5; j++){
		fprintf(fptr, "Card %d:\t##VALUE: %d\t##TYPE: %d\n", j + 1, player -> bestHand[j].cardVal, player -> bestHand[j].cardType);
	}
	fprintf(fptr, "%s\n", getHandVal(player -> handVal));
	fprintf(fptr, "Outs: %d\n", player -> outs);
	fprintf(fptr, "Chance of hitting an out: %d%%\n", calcOutsPercentage(*player, table));
	fprintf(fptr, "-------------------------------------------\n");
	fclose(fptr);
}

void writeComparisonTest(int winnerIndex){
	FILE *fptr;
	fptr = fopen("../out/Tests.txt", "a");
	if (winnerIndex > 0){
	    fprintf(fptr, "#############################\n");
		fprintf(fptr, "### Winner is Player %d!! ###\n", winnerIndex);
		fprintf(fptr, "#############################\n");
	}
	else{
		int roundWinners[8];
		memcpy(roundWinners, getWinnerArr(), sizeof(roundWinners));
		fprintf(fptr, "Multiple winners!\n");
		int j = 1;
		for (int i = 0; i < 8; i++){
			if (roundWinners[i] != -1){
				fprintf(fptr, "Winner nr. %d: Player %d\n", j++, roundWinners[i]);
			}
		}
	}
	fclose(fptr);
}