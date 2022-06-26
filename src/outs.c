#include <stdio.h>
#include <string.h>
#include "Eval.h"
#include "outs.h"

void restoreCards(struct Player* player, struct Card restoreTable[5], struct Card table[5], 
	struct Card restoreCard1, struct Card restoreCard2);

/*
#######################################################################################################
ALGORITHM TO FIND OUTS IN DECK
#######################################################################################################
*/

int getOuts(struct Player player, struct Card table[5], struct Deck* shuffledHead) {
	printf("#### IN EVALUATION OF OUTS ####\n");

	struct Deck* tmpOuts = NULL;
	tmpOuts = shuffledHead;
	struct Card restoreCard1;
	struct Card restoreCard2;
	struct Card restoreTable[5];
	int potHandVal = 0;
	int outs = 0;
	memcpy(restoreTable, table, sizeof(restoreTable));
	restoreCard1.cardVal = player.card1.cardInfo.cardVal;
	restoreCard1.cardType = player.card1.cardInfo.cardType;
	restoreCard2.cardVal = player.card2.cardInfo.cardVal;
	restoreCard2.cardType = player.card2.cardInfo.cardType;

	while (tmpOuts != NULL) {

		//printf("Random card from deck:\n");
		//printf("##VALUE: %d\t##TYPE: %d\n", tmp -> cardInfo.cardVal, tmp -> cardInfo.cardType);

		for (int i = 0; i < 7; i++) {
			if ((tmpOuts->cardDrawn) == 0) {
				if (i == 0) {
					//printf("player.card1: ##VALUE: %d\t##TYPE: %d\n", player.card1.cardInfo.cardVal, player.card1.cardInfo.cardType);
					//printf("player.card2: ##VALUE: %d\t##TYPE: %d\n", player.card2.cardInfo.cardVal, player.card2.cardInfo.cardType);
					//printf("tmpCard: ##VALUE: %d\t##TYPE: %d ##CARD DRAWN: %d\n", tmpOuts -> cardInfo.cardVal, tmpOuts -> cardInfo.cardType, tmpOuts -> cardDrawn);
					player.card1.cardInfo.cardVal = tmpOuts->cardInfo.cardVal;
					player.card1.cardInfo.cardType = tmpOuts->cardInfo.cardType;
				}
				else if (i == 1) {
					player.card2.cardInfo.cardVal = tmpOuts->cardInfo.cardVal;
					player.card2.cardInfo.cardType = tmpOuts->cardInfo.cardType;
				}
				else if (i == 2) {
					table[0].cardVal = tmpOuts->cardInfo.cardVal;
					table[0].cardType = tmpOuts->cardInfo.cardType;
				}
				else if (i == 3) {
					table[1].cardVal = tmpOuts->cardInfo.cardVal;
					table[1].cardType = tmpOuts->cardInfo.cardType;
				}
				else if (i == 4) {
					table[2].cardVal = tmpOuts->cardInfo.cardVal;
					table[2].cardType = tmpOuts->cardInfo.cardType;
				}
				else if (i == 5) {
					table[3].cardVal = tmpOuts->cardInfo.cardVal;
					table[3].cardType = tmpOuts->cardInfo.cardType;
				}
				else if (i == 6) {
					table[4].cardVal = tmpOuts->cardInfo.cardVal;
					table[4].cardType = tmpOuts->cardInfo.cardType;
				}
				potHandVal = evaluateHand(&player, table, 1);
				//printf("potHandVal: %d\n", potHandVal);

				//printf("Player handVal: %d\tpotHandVal: %d\n", player.handVal, potHandVal);

				if (player.handVal < potHandVal) {
					outs++;
					restoreCards(&player, restoreTable, table, restoreCard1, restoreCard2);
					potHandVal = 0;
					break;
				}
				restoreCards(&player, restoreTable, table, restoreCard1, restoreCard2);
				potHandVal = 0;
			}
		}
		tmpOuts = tmpOuts->next;
	}
	restoreCards(&player, restoreTable, table, restoreCard1, restoreCard2);
	printf("ERROR CHECKER:\touts: %d\n", outs);
	tmpOuts = shuffledHead;
	return outs;
}

void restoreCards(struct Player* player, struct Card restoreTable[5], struct Card table[5], struct Card restoreCard1, struct Card restoreCard2) {
	memcpy(table, restoreTable, (sizeof(struct Card)) * 5);
	player->card1.cardInfo.cardVal = restoreCard1.cardVal;
	player->card1.cardInfo.cardType = restoreCard1.cardType;
	player->card2.cardInfo.cardVal = restoreCard2.cardVal;
	player->card2.cardInfo.cardType = restoreCard2.cardType;
}


int calcOutsPercentage(struct Player player, struct Card table[5]) {
	if (player.card1.cardInfo.cardVal > 0 && player.card2.cardInfo.cardVal > 0
		&& (table[0].cardVal < 0 && table[1].cardVal < 0 && table[2].cardVal < 0 && table[3].cardVal < 0 && table[4].cardVal < 0)) {
		return (float)player.outs / 50 * 100;
	}
	else if (player.card1.cardInfo.cardVal > 0 && player.card2.cardInfo.cardVal > 0
		&& (table[0].cardVal > 0 && table[1].cardVal > 0 && table[2].cardVal > 0 && table[3].cardVal < 0 && table[4].cardVal < 0)) {
		return (float)player.outs / 47 * 100;
	}
	else if (player.card1.cardInfo.cardVal > 0 && player.card2.cardInfo.cardVal > 0
		&& (table[0].cardVal > 0 && table[1].cardVal > 0 && table[2].cardVal > 0 && table[3].cardVal > 0 && table[4].cardVal < 0)) {
		return (float)player.outs / 46 * 100;
	}
	else if (player.card1.cardInfo.cardVal > 0 && player.card2.cardInfo.cardVal > 0
		&& (table[0].cardVal > 0 && table[1].cardVal > 0 && table[2].cardVal > 0 && table[3].cardVal > 0 && table[4].cardVal > 0)) {
		return (float)player.outs / 45 * 100;
	}
	else {
		return 0;
	}
}