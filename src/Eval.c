#include <stdio.h>
#include <string.h>
#include "Eval.h"

int evalKind();
void sortHand();
void makeBestHand(int offset, int isRoyal, int isSeq, int pairKind, int doublePaired, int offset2, int isFlush, int kind);
int checkSeq(); // checks whether Card are in sequense or not - i.e. straight, straight flush or royal flush
int checkPaired(); //  checks whether we have concurrent paired Card - 4 Of A Kind, Set, or Pair
int checkDoublePaired(); // check for full house or double pair
int checkFlush(); //Check for 5 of same type (kind)
int kicker(int isDouble, int highCard); // Finds kicker if there is one (Pair and Two Pair)
int checkKicker(int kicker1, int kicker2);
int compareTwoHands(struct Player playerOne, struct Player playerTwo);
int isStraight = 0;
void printHandsInPlay(struct Player* players, int maxPlayerAmount);
struct Player findQualifyingHand(int playerNum, int maxPlayerAmount, struct Player *player);


struct Card tmpTable[5];
struct Card tmp1;
struct Card tmpCard[7]; // sorted hand plus Card on table
struct Card curBestHand[5]; // current best hand (max. 5 Card) for player xs
int winners[8];


struct Player EvalHandVariable;

/*
#######################################################################################################
EVALUATION OF HAND ALGORITHM
Version 1.10
#######################################################################################################
*/
void printHandVal(int value) {
	switch (value) {
	case 1:
		printf("## High card! ##\n");
		break;
	case 2:
		printf("## Pair! ##\n");
		break;
	case 3:
		printf("## Two Pair! ##\n");
		break;
	case 4:
		printf("## Set! ##\n");
		break;
	case 5:
		printf("## Straight! ##\n");
		break;
	case 6:
		printf("## Flush ##\n");
		break;
	case 7:
		printf("## Full House! ##\n");
		break;
	case 8:
		printf("## Four Of A Kind! ##\n");
		break;
	case 9:
		printf("## Straight Flush! ##\n");
		break;
	case 10:
		printf("## Royal Flush! ##\n");
		break;
	}
}

int evaluateHand(struct Player* player, struct Card curTable[5], int simulate) {
	EvalHandVariable = *player;
	memcpy(tmpTable, curTable, sizeof(tmpTable));
	/*	printf("ERROR CHECKER: \n");
		for (int i = 0; i < 5; i++){
			printf("tmpTable %d\tcardVal: %d\n", i, tmpTable[i].cardVal);
		}*/

	isStraight = 0;
	int isSet = 0;
	int isPair = 0;
	int isTwoPair = 0;
	int tmpHandValue = 0;



	tmpHandValue = checkSeq();
	if (tmpHandValue == 10 || tmpHandValue == 9) { // in case of straight flush or royal flush - return
		if (!simulate) {
			memcpy(player->bestHand, curBestHand, sizeof(curBestHand));
		}
		return tmpHandValue;
	}
	else if (tmpHandValue == 5) {
		isStraight = 1;
	}

	tmpHandValue = checkPaired();
	if (tmpHandValue == 8) {
		if (!simulate) {
			memcpy(player->bestHand, curBestHand, sizeof(curBestHand));
		}
		return tmpHandValue;
	}
	else if (tmpHandValue == 4) {
		isSet = 1;
	}
	else if (tmpHandValue == 2) {
		isPair = 1;
	}

	tmpHandValue = checkDoublePaired();
	if (tmpHandValue == 7) {
		if (!simulate) {
			memcpy(player->bestHand, curBestHand, sizeof(curBestHand));
		}
		return tmpHandValue;
	}
	else if (tmpHandValue == 3) {
		isTwoPair = 1;
	}

	if (checkFlush() == 6) {
		if (!simulate) {
			memcpy(player->bestHand, curBestHand, sizeof(curBestHand));
		}
		return 6;
	}

	if (isStraight) {
		if (!simulate) {
			memcpy(player->bestHand, curBestHand, sizeof(curBestHand));
		}
		return 5;
	}
	else if (isSet) {
		player->kicker = EvalHandVariable.kicker;
		if (!simulate) {
			memcpy(player->bestHand, curBestHand, sizeof(curBestHand));
		}
		return 4;
	}
	else if (isTwoPair) {
		player->kicker = EvalHandVariable.kicker;
		if (!simulate) {
			memcpy(player->bestHand, curBestHand, sizeof(curBestHand));
		}
		return 3;
	}
	else if (isPair) {
		player->kicker = EvalHandVariable.kicker;
		if (!simulate) {
			memcpy(player->bestHand, curBestHand, sizeof(curBestHand));
		}
		return 2;
	}
	else {
		int j = 0;
		int check = 0;
		for (int i = 6; i >= 0; i--) {
			if (tmpCard[0].cardVal == 1 && !check) {
				curBestHand[0].cardVal = 1;
				curBestHand[0].cardType = tmpCard[0].cardType;
				j++;
				i++;
				check = 1;
			}
			else {
				curBestHand[j].cardVal = tmpCard[i].cardVal;
				curBestHand[j].cardType = tmpCard[i].cardType;
				j++;
				if (j == 5) {
					break;
				}
			}
		}
		EvalHandVariable.kicker = kicker(0, 1);
		player->kicker = EvalHandVariable.kicker;
		if (!simulate) {
			memcpy(player->bestHand, curBestHand, sizeof(curBestHand));
		}
		return 1;
	}

}

int checkFlush() {
	int kind = 0;
	int offSet = 0;
	int hearts = 0;
	int clubs = 0;
	int diamonds = 0;
	int spades = 0;

	for (int i = 0; i < 7; i++) {
		if (tmpCard[i].cardType == 1) {
			hearts++;
		}
		else if (tmpCard[i].cardType == 2) {
			clubs++;
		}
		else if (tmpCard[i].cardType == 3) {
			diamonds++;
		}
		else if (tmpCard[i].cardType == 4) {
			spades++;
		}
		if (hearts >= 5 || clubs >= 5 || diamonds >= 5 || spades >= 5) {
			offSet = i;
		}
	}

	if (hearts >= 5) {
		kind = 1;
	}
	else if (clubs >= 5) {
		kind = 2;
	}
	else if (diamonds >= 5) {
		kind = 3;
	}
	else if (spades >= 5) {
		kind = 4;
	}

	if (offSet != 0) {
		makeBestHand(offSet, 0, 0, 0, 0, 0, 1, kind);
		return 6;
	}
	return 0;
}

int checkDoublePaired() {
	int pair1 = 0;
	int pair2 = 0;
	int pair3 = 0;
	int set = 0;
	int offsetPair1 = 0;
	int offsetPair2 = 0;
	int offsetPair3 = 0;
	int offsetHouse = 0;
	int fullHouse = 0;
	int twoPair = 0;

	int houseVal = 0;

	for (int i = 0; i < 7; i++) {
		if (tmpCard[i].cardVal == tmpCard[i + 1].cardVal) {
			if (tmpCard[i].cardVal == tmpCard[i + 2].cardVal) {
				houseVal = tmpCard[i].cardVal;
				offsetHouse = i;
				set = 1;
				if (i == 4) {
					break;
				}
				continue;
			}
			if (pair1 == 1 && pair2 == 1 && pair3 == 0 && tmpCard[i].cardVal != houseVal) {
				offsetPair3 = i;
				pair3 = 1;
			}
			else if (pair1 == 1 && pair2 == 0 && pair3 == 0 && tmpCard[i].cardVal != houseVal) {
				offsetPair2 = i;
				pair2 = 1;
			}
			else if (pair1 == 0 && pair2 == 0 && pair3 == 0 && tmpCard[i].cardVal != houseVal) {
				offsetPair1 = i;
				pair1 = 1;
			}
		}
	}



	if (set && pair3) {
		makeBestHand(offsetPair3, 0, 0, 0, 1, offsetHouse, 0, 0);
		return 7;
	}
	else if (set && pair2) {
		makeBestHand(offsetPair2, 0, 0, 0, 1, offsetHouse, 0, 0);
		return 7;
	}
	else if (set && pair1) {
		makeBestHand(offsetPair1, 0, 0, 0, 1, offsetHouse, 0, 0);
		return 7;
	}
	else if (pair2 && pair3 && !isStraight) {
		makeBestHand(offsetPair2, 0, 0, 0, 1, offsetPair3, 0, 0);
		return 3;
	}
	else if (pair1 && pair3 && !isStraight) {
		makeBestHand(offsetPair1, 0, 0, 0, 1, offsetPair3, 0, 0);
		return 3;
	}
	else if (pair1 && pair2 && !isStraight) {
		makeBestHand(offsetPair1, 0, 0, 0, 1, offsetPair2, 0, 0);
		return 3;
	}

	return 0;

}

int checkPaired() {
	int offSet1 = 0;
	int offSet2 = 0;
	int offSet3 = 0;
	int four = 0;
	int set = 0;
	int pair = 0;

	for (int i = 0; i < 7; i++) {
		if (tmpCard[i].cardVal == tmpCard[i + 1].cardVal &&
			tmpCard[i].cardVal == tmpCard[i + 2].cardVal &&
			tmpCard[i].cardVal == tmpCard[i + 3].cardVal) {
			offSet3 = i;
			four = 1;
		}
		else if (tmpCard[i].cardVal == tmpCard[i + 1].cardVal &&
			tmpCard[i].cardVal == tmpCard[i + 2].cardVal) {
			offSet2 = i;
			set = 1;
		}
		else if (tmpCard[i].cardVal == tmpCard[i + 1].cardVal) {
			offSet1 = i;
			pair = 1;
		}
	}
	if (four == 1) {
		makeBestHand(offSet3, 0, 0, 4, 0, 0, 0, 0);
		return 8;
	}
	else if (set == 1 && !isStraight) {
		makeBestHand(offSet2, 0, 0, 3, 0, 0, 0, 0);
		return 4;
	}
	else if (pair == 1 && !isStraight) {
		makeBestHand(offSet1, 0, 0, 2, 0, 0, 0, 0);
		return 2;
	}

	return 0;
}

int checkSeq() {
	int tmpHandValue = 0;

	sortHand();

	int offset = 0;
	int start = 0;
	int isRoyal = 0;
	int seq = 0;
	int isSeq = 0;
	// first check for royal flush or straight flush
	// if not of the same kind - straight
	for (int i = 0; i < 7; i++) {
		if ((tmpCard[i].cardVal == (tmpCard[i + 1].cardVal - 1))
			|| (tmpCard[i].cardVal == (tmpCard[i + 2].cardVal - 1))
			|| (tmpCard[i].cardVal == (tmpCard[i + 3].cardVal - 1))) {
			if ((tmpCard[i].cardVal != tmpCard[i - 1].cardVal)) {
				if (!start) {
					offset = i;
					start = 1;
				}
				seq++;
				if (seq > 4 && tmpCard[i].cardVal == (tmpCard[i - 1].cardVal + 1)) {
					printf("Incremented off-set\n");
					offset++;
				}
			}
		}
		else if (i != 6 && !(seq >= 4)) {
			if (tmpCard[i].cardVal != 13) {
				start = 0;
				seq = 0;
			}
		}
	}
	if (seq >= 3 && tmpCard[0].cardVal == 1 && tmpCard[6].cardVal == 13 && !(tmpCard[5].cardVal < 12)
		&& !(tmpCard[4].cardVal < 11) && !(tmpCard[3].cardVal < 10)) {
		for (int i = 0; i < 7; i++) {
			if (tmpCard[offset].cardVal != 10 && tmpCard[offset + 1].cardVal != 11) {
				printf("Incremented ace-high off-set\n");
				offset++;
			}
			else {
				break;
			}
		}
		isStraight = 1;
		isSeq = 1;
		isRoyal = 1;
		makeBestHand(offset, isRoyal, isSeq, 0, 0, 0, 0, 0);
		if (evalKind() == 1) {
			return 10;
		}
		else {
			printf("Ace high straight\n");
			tmpHandValue = 5;
			//break;
		}
	}
	else if (seq >= 4) {
		isStraight = 1;
		isSeq = 1;
		makeBestHand(offset, isRoyal, isSeq, 0, 0, 0, 0, 0);
		if (evalKind() == 1) {
			return 9;
		}
		else {
			printf("Normal straight\n");
			tmpHandValue = 5;
			//break;
		}
	}
	if (tmpHandValue != 0) {
		return tmpHandValue;
	}


	return 0;
	//check for four of a kind
}

void makeBestHand(int offset, int isRoyal, int isSeq, int pairKind, int doublePaired, int offset2, int isFlush, int kind) {
	//printf("makeBestHand() called\n");
	if (isSeq) {
		int j = 0;
		if (isRoyal) {
			curBestHand[j].cardVal = tmpCard[0].cardVal;
			curBestHand[j].cardType = tmpCard[0].cardType;
			j++;
			for (int i = offset; i < (offset + 4); i++) {
				if ((tmpCard[i].cardVal == tmpCard[i + 1].cardVal)
					&& (tmpCard[i].cardVal == tmpCard[i + 2].cardVal)) {
					if (tmpCard[i].cardType == tmpCard[i + 3].cardType) { //if we have 3 of a kind, adjust offset
						//in case of sequense
						curBestHand[j].cardVal = tmpCard[i].cardVal;
						curBestHand[j].cardType = tmpCard[i].cardType;
						offset += 2;
						i += 2;
						j++;
						continue;
					}
					else if (tmpCard[i + 1].cardType == tmpCard[i + 3].cardType) {
						curBestHand[j].cardVal = tmpCard[i + 1].cardVal;
						curBestHand[j].cardType = tmpCard[i + 1].cardType;
						offset += 2;
						i += 2;
						j++;
						continue;
					}
					else {
						curBestHand[j].cardVal = tmpCard[i + 2].cardVal;
						curBestHand[j].cardType = tmpCard[i + 2].cardType;
						offset += 2;
						i += 2;
						j++;
						continue;
					}
				}

				else if (tmpCard[i].cardVal == tmpCard[i + 1].cardVal) { // if we have pair, adjust offset
					//in case of sequense
					if (tmpCard[i].cardType == tmpCard[i + 2].cardType) {
						curBestHand[j].cardVal = tmpCard[i].cardVal;
						curBestHand[j].cardType = tmpCard[i].cardType;
						offset++;
						i++;
						j++;
						continue;
					}
					else {
						curBestHand[j].cardVal = tmpCard[i + 1].cardVal;
						curBestHand[j].cardType = tmpCard[i + 1].cardType;
						offset++;
						i++;
						j++;
						continue;
					}
				}
				curBestHand[j].cardVal = tmpCard[i].cardVal;
				curBestHand[j].cardType = tmpCard[i].cardType;
				j++;
			}
			return;

		}

		for (int i = offset; i < (offset + 5); i++) {
			if ((tmpCard[i].cardVal == tmpCard[i + 1].cardVal)
				&& (tmpCard[i].cardVal == tmpCard[i + 2].cardVal)) {
				if (tmpCard[i].cardType == tmpCard[i + 3].cardType) { //if we have 3 of a kind, adjust offset
					//in case of sequense
					curBestHand[j].cardVal = tmpCard[i].cardVal;
					curBestHand[j].cardType = tmpCard[i].cardType;
					offset += 2;
					i += 2;
					j++;
					continue;
				}
				else if (tmpCard[i + 1].cardType == tmpCard[i + 3].cardType) {
					curBestHand[j].cardVal = tmpCard[i + 1].cardVal;
					curBestHand[j].cardType = tmpCard[i + 1].cardType;
					offset += 2;
					i += 2;
					j++;
					continue;
				}
				else {
					curBestHand[j].cardVal = tmpCard[i + 2].cardVal;
					curBestHand[j].cardType = tmpCard[i + 2].cardType;
					offset += 2;
					i += 2;
					j++;
					continue;
				}
			}

			else if (tmpCard[i].cardVal == tmpCard[i + 1].cardVal) { // if we have pair, adjust offset
				//in case of sequense
				if (tmpCard[i].cardType == tmpCard[i + 2].cardType) {
					curBestHand[j].cardVal = tmpCard[i].cardVal;
					curBestHand[j].cardType = tmpCard[i].cardType;
					offset++;
					i++;
					j++;
					continue;
				}
				else {
					curBestHand[j].cardVal = tmpCard[i + 1].cardVal;
					curBestHand[j].cardType = tmpCard[i + 1].cardType;
					offset++;
					i++;
					j++;
					continue;
				}
			}

			curBestHand[j].cardVal = tmpCard[i].cardVal;
			curBestHand[j].cardType = tmpCard[i].cardType;
			j++;
		}
	}

	// check for 4OfAKind or Set or Pair
	else if (pairKind != 0) {
		switch (pairKind) {
		case 4:
			curBestHand[0].cardVal = tmpCard[offset].cardVal;
			curBestHand[0].cardType = tmpCard[offset].cardType;

			curBestHand[1].cardVal = tmpCard[offset + 1].cardVal;
			curBestHand[1].cardType = tmpCard[offset + 1].cardType;

			curBestHand[2].cardVal = tmpCard[offset + 2].cardVal;
			curBestHand[2].cardType = tmpCard[offset + 2].cardType;

			curBestHand[3].cardVal = tmpCard[offset + 3].cardVal;
			curBestHand[3].cardType = tmpCard[offset + 3].cardType;

			EvalHandVariable.kicker = kicker(0, 0);

			break;
		case 3:
			curBestHand[0].cardVal = tmpCard[offset].cardVal;
			curBestHand[0].cardType = tmpCard[offset].cardType;

			curBestHand[1].cardVal = tmpCard[offset + 1].cardVal;
			curBestHand[1].cardType = tmpCard[offset + 1].cardType;

			curBestHand[2].cardVal = tmpCard[offset + 2].cardVal;
			curBestHand[2].cardType = tmpCard[offset + 2].cardType;

			EvalHandVariable.kicker = kicker(0, 0);

			break;
		case 2:
			curBestHand[0].cardVal = tmpCard[offset].cardVal;
			curBestHand[0].cardType = tmpCard[offset].cardType;

			curBestHand[1].cardVal = tmpCard[offset + 1].cardVal;
			curBestHand[1].cardType = tmpCard[offset + 1].cardType;

			EvalHandVariable.kicker = kicker(0, 0);

			break;
		default:
			break;
		}
	}
	else if (doublePaired) {
		if (tmpCard[offset].cardVal == tmpCard[offset + 1].cardVal) {
			curBestHand[0].cardVal = tmpCard[offset].cardVal;
			curBestHand[0].cardType = tmpCard[offset].cardType;

			curBestHand[1].cardVal = tmpCard[offset + 1].cardVal;
			curBestHand[1].cardType = tmpCard[offset + 1].cardType;
		}

		if (tmpCard[offset2].cardVal == tmpCard[offset2 + 1].cardVal) {
			if (tmpCard[offset2].cardVal == tmpCard[offset2 + 2].cardVal) {
				curBestHand[2].cardVal = tmpCard[offset2].cardVal;
				curBestHand[2].cardType = tmpCard[offset2].cardType;

				curBestHand[3].cardVal = tmpCard[offset2 + 1].cardVal;
				curBestHand[3].cardType = tmpCard[offset2 + 1].cardType;

				curBestHand[4].cardVal = tmpCard[offset2 + 2].cardVal;
				curBestHand[4].cardType = tmpCard[offset2 + 2].cardType;

				//break;				
			}
			curBestHand[2].cardVal = tmpCard[offset2].cardVal;
			curBestHand[2].cardType = tmpCard[offset2].cardType;

			curBestHand[3].cardVal = tmpCard[offset2 + 1].cardVal;
			curBestHand[3].cardType = tmpCard[offset2 + 1].cardType;

			EvalHandVariable.kicker = kicker(1, 0);
			//break;				
		}
	}
	else if (isFlush) {
		int check = 0;
		int j = 0;
		int counter = 0;
		for (int i = offset; i >= 0; i--) {
			if (tmpCard[0].cardType == kind && tmpCard[0].cardVal == 1 && !check) {
				curBestHand[4].cardVal = tmpCard[0].cardVal;
				curBestHand[4].cardType = tmpCard[0].cardType;
				counter++;
				i++;
				check = 1;
				continue;
			}
			if (tmpCard[1].cardType == kind && tmpCard[1].cardVal == 1 && !check) {
				curBestHand[4].cardVal = tmpCard[1].cardVal;
				curBestHand[4].cardType = tmpCard[1].cardType;
				counter++;
				i++;
				check = 1;
				continue;
			}
			if (tmpCard[2].cardType == kind && tmpCard[2].cardVal == 1 && !check) {
				curBestHand[4].cardVal = tmpCard[2].cardVal;
				curBestHand[4].cardType = tmpCard[2].cardType;
				counter++;
				i++;
				check = 1;
				continue;
			}
			if (tmpCard[i].cardType == kind) {
				curBestHand[j].cardVal = tmpCard[i].cardVal;
				curBestHand[j].cardType = tmpCard[i].cardType;
				counter++;
				if (counter == 5) {
					break;
				}
				j++;
			}
		}
	}
}

//we only need to identify a unique kicker hand in case of Pair og Two Pair
int kicker(int isDouble, int highCard) {
	int check = 0;

	if (highCard) {
		if (EvalHandVariable.card1.cardInfo.cardVal == 1) {
			return EvalHandVariable.card1.cardInfo.cardVal;
		}
		else if (EvalHandVariable.card2.cardInfo.cardVal == 1) {
			return EvalHandVariable.card2.cardInfo.cardVal;
		}
		else if (EvalHandVariable.card1.cardInfo.cardVal > EvalHandVariable.card2.cardInfo.cardVal) {
			return EvalHandVariable.card1.cardInfo.cardVal;
		}
		else if (EvalHandVariable.card1.cardInfo.cardVal < EvalHandVariable.card2.cardInfo.cardVal) {
			return EvalHandVariable.card2.cardInfo.cardVal;
		}
	}
	else if (!isDouble) {
		if (EvalHandVariable.card1.cardInfo.cardVal != curBestHand[0].cardVal && EvalHandVariable.card2.cardInfo.cardVal != curBestHand[0].cardVal) {
			if (EvalHandVariable.card1.cardInfo.cardVal == 1) {
				return 1;
			}
			else if (EvalHandVariable.card2.cardInfo.cardVal == 1) {
				return 1;
			}
			else if (EvalHandVariable.card1.cardInfo.cardVal > EvalHandVariable.card2.cardInfo.cardVal) {
				return EvalHandVariable.card1.cardInfo.cardVal;
			}
			else {
				return EvalHandVariable.card2.cardInfo.cardVal;
			}
		}
		if (EvalHandVariable.card1.cardInfo.cardVal == curBestHand[0].cardVal && EvalHandVariable.card2.cardInfo.cardVal != curBestHand[0].cardVal) {
			return EvalHandVariable.card2.cardInfo.cardVal;
		}
		if (EvalHandVariable.card1.cardInfo.cardVal != curBestHand[0].cardVal && EvalHandVariable.card2.cardInfo.cardVal == curBestHand[0].cardVal) {
			return EvalHandVariable.card1.cardInfo.cardVal;
		}
	}
	else if (isDouble) {
		if ((EvalHandVariable.card1.cardInfo.cardVal != curBestHand[0].cardVal && EvalHandVariable.card1.cardInfo.cardVal != curBestHand[2].cardVal)
			&& (EvalHandVariable.card2.cardInfo.cardVal != curBestHand[0].cardVal && EvalHandVariable.card2.cardInfo.cardVal != curBestHand[2].cardVal)) {
			if (EvalHandVariable.card1.cardInfo.cardVal == 1) {
				return 1;
			}
			else if (EvalHandVariable.card2.cardInfo.cardVal == 1) {
				return 1;
			}
			else if (EvalHandVariable.card1.cardInfo.cardVal > EvalHandVariable.card2.cardInfo.cardVal) {
				return EvalHandVariable.card1.cardInfo.cardVal;
			}
			else {
				return EvalHandVariable.card2.cardInfo.cardVal;
			}
		}
		if ((EvalHandVariable.card1.cardInfo.cardVal == curBestHand[0].cardVal || EvalHandVariable.card1.cardInfo.cardVal == curBestHand[2].cardVal)
			&& (EvalHandVariable.card2.cardInfo.cardVal != curBestHand[0].cardVal && EvalHandVariable.card2.cardInfo.cardVal != curBestHand[2].cardVal)) {
			return EvalHandVariable.card2.cardInfo.cardVal;
		}
		if ((EvalHandVariable.card1.cardInfo.cardVal != curBestHand[0].cardVal && EvalHandVariable.card1.cardInfo.cardVal != curBestHand[2].cardVal)
			&& (EvalHandVariable.card2.cardInfo.cardVal == curBestHand[0].cardVal || EvalHandVariable.card2.cardInfo.cardVal == curBestHand[2].cardVal)) {
			return EvalHandVariable.card1.cardInfo.cardVal;
		}
	}

	return 0;
}

int evalKind() {
	int counter = 0;
	for (int i = 0; i < 5; i++) {
		if (curBestHand[i].cardType == curBestHand[i + 1].cardType) {
			counter++;
		}
	}
	if (counter == 4) {
		return 1;
	}
	return 0;
}

void sortHand() {
	int size = 7; // hand plus all Card on board after flop, turn and river

	tmpCard[0].cardVal = EvalHandVariable.card1.cardInfo.cardVal;
	tmpCard[0].cardType = EvalHandVariable.card1.cardInfo.cardType;

	tmpCard[1].cardVal = EvalHandVariable.card2.cardInfo.cardVal;
	tmpCard[1].cardType = EvalHandVariable.card2.cardInfo.cardType;

	for (int i = 0; i < 5; i++) {
		tmpCard[i + 2].cardVal = tmpTable[i].cardVal;
		tmpCard[i + 2].cardType = tmpTable[i].cardType;
		if (tmpCard[i + 2].cardVal == 0) {
			size--;
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			if ((tmpCard[j].cardVal > tmpCard[j + 1].cardVal) && ((tmpCard[j].cardVal > 0)
				|| (tmpCard[j + 1].cardVal > 0))) {
				tmp1 = tmpCard[j + 1];
				tmpCard[j + 1] = tmpCard[j];
				tmpCard[j] = tmp1;
			}
		}
	}
}

void printBestHand(struct Card hand[5]) {
	printf("Final best hand:\n");
	for (int i = 0; i < 5; i++) {
		printf("Card %d: ##VALUE: %d\t ##TYPE: %d\n", i + 1, hand[i].cardVal, hand[i].cardType);
	}
}

void printSortedCard() {
	printf("Sorted Card:\n");
	for (int i = 0; i < 7; i++) {
		printf("Card %d: ##VALUE: %d\t ##TYPE: %d\n", i + 1, tmpCard[i].cardVal, tmpCard[i].cardType);
	}
}

void reInitCurrentBestHand() {
	for (int i = 0; i < 5; i++) {
		curBestHand[i].cardVal = 0;
		curBestHand[i].cardType = 0;
	}
}

char *getHandVal(int value){
		switch (value) {
	case 1:
		return "## High card! ##\n";
		break;
	case 2:
		return "## Pair! ##\n";
		break;
	case 3:
		return "## Two Pair! ##\n";
		break;
	case 4:
		return "## Set! ##\n";
		break;
	case 5:
		return "## Straight! ##\n";
		break;
	case 6:
		return "## Flush ##\n";
		break;
	case 7:
		return "## Full House! ##\n";
		break;
	case 8:
		return "## Four Of A Kind! ##\n";
		break;
	case 9:
		return "## Straight Flush! ##\n";
		break;
	case 10:
		return "## Royal Flush! ##\n";
		break;
	}
}

struct Card* getTmpCards(){
	return tmpCard;
}


/*
#######################################################################################################
COMPARISON OF HANDS
Version 1.4
#######################################################################################################
*/
int compareHands(int maxPlayerAmount, struct Player *player) {
	int bestHandIndex;
	int inPlay[8];
	int contestors[8];
	int maxHandVal = 0;
	int j = 0;

	printHandsInPlay(player, maxPlayerAmount);

	for (int i = 0; i < 8; i++)
	{
		inPlay[i] = -1;
		contestors[i] = -1;
		winners[i] = -1;
	}

	// First find all players still in play and find maximum hand value
	for (int i = 1; i <= maxPlayerAmount; i++) {
		if (player -> fold != 1) {
			printf("Player: %d - Fold value: %d - inPlay value: %d\n", player -> playerNumber, player -> fold, player -> inPlay);
			if (maxHandVal == 0) {
				maxHandVal = player -> handVal;
			}
			if (player -> handVal > maxHandVal) {
				maxHandVal = player -> handVal;
			}
			inPlay[j] = player -> playerNumber;
			j++;
		}
		player = player -> next;
	}

	// Find player(s) who have same max. hand values
	int g = 0;
	for (int i = 0; i < maxPlayerAmount; i++) {
		if (player -> playerNumber == inPlay[i] && player -> handVal == maxHandVal){
			contestors[g] = player -> playerNumber;
			g++;
		}
/*		if (allPlayers[inPlay[i]].handVal == maxHandVal) {
			contestors[g] = inPlay[i];
			g++;
		}*/
		player = player -> next;
	}


	printf("Number of contestors: %d\n", g);

	//if Only one max. hand - we already have the winner
	if (g == 1) {
		printf("Wins on:\t");
		for (int i = 0; i < maxPlayerAmount; i++){
			if (player -> playerNumber == contestors[0]){
				break;
			}
			player = player -> next;
		}
		printHandVal(player -> handVal);
		return contestors[0];
	}

	//Otherwise, we have to compare hands
	j = 0;
	int retValue = 0;
	int winnerIndex = -2;
	for (int i = 0; i < g - 1; i++) {
		if (contestors[i] != -1 && contestors[i + 1] != -1) {
/*			if (winnerIndex < 0) {
				retValue = compareTwoHands(allPlayers[contestors[i]], allPlayers[contestors[i + 1]]);
			}
			if (winnerIndex >= 0) {
				retValue = compareTwoHands(allPlayers[winnerIndex], allPlayers[contestors[i + 1]]);
			}*/
			if (winnerIndex < 0){
				retValue = compareTwoHands(findQualifyingHand(contestors[i], maxPlayerAmount, player), 
					findQualifyingHand(contestors[i+1], maxPlayerAmount, player));
			}
			if (winnerIndex >= 0) {
				retValue = compareTwoHands(findQualifyingHand(winnerIndex, maxPlayerAmount, player), 
					findQualifyingHand(contestors[i+1], maxPlayerAmount, player));
			}
			if (retValue == 1 && (winnerIndex == -2)) {
				winnerIndex = contestors[i];
			}
			else if (retValue == 1 && (winnerIndex >= -1)) {
				continue;
			}
			else if (retValue == 2) {
				winnerIndex = contestors[i + 1];
			}

			else {
				if (i == 0) {
					winners[j++] = contestors[i];
				}
				if (winnerIndex < 0) {
					if (contestors[i] != winners[j - 1]) {
						winners[j++] = contestors[i];
					}
					winners[j++] = contestors[i + 1];
				}
				else if (winnerIndex >= 0) {
					winners[j++] = winnerIndex;
					winners[j++] = contestors[i + 1];
				}
				winnerIndex = -1;
			}
		}
	}

	return winnerIndex; // return index of winning player
}

struct Player findQualifyingHand(int playerNum, int maxPlayerAmount, struct Player *player){
	for (int i = 0; i < maxPlayerAmount; i++){
		if (player -> playerNumber == playerNum){
			return *player;
		}
		player = player -> next;
	}
}

int* getWinnerArr() {
	return winners;
}

int compareTwoHands(struct Player playerOne, struct Player playerTwo) {
	//If players have different hands
	// This line may be redundant
	if (playerOne.handVal > playerTwo.handVal) {
		return 1;
	}
	else if (playerOne.handVal < playerTwo.handVal) {
		return 2;
	}

	//If players have same hand

	//## HIGH-CARD #################
	if (playerOne.handVal == 1) {
		printf("Comparing high card\n");
		for (int i = 0; i < 5; i++) {
			if (playerOne.bestHand[0].cardVal == 1 && playerTwo.bestHand[0].cardVal != 1) {
				return 1;
			}
			else if (playerOne.bestHand[0].cardVal != 1 && playerTwo.bestHand[0].cardVal == 1) {
				return 2;
			}
			else if (playerOne.bestHand[i].cardVal > playerTwo.bestHand[i].cardVal) {
				return 1;
			}
			else if (playerOne.bestHand[i].cardVal < playerTwo.bestHand[i].cardVal) {
				return 2;
			}
		}
	}



	//## PAIR #######################
	if (playerOne.handVal == 2) {
		printf("Comparing pair\n");
		if (playerOne.bestHand[0].cardVal == 1 && playerTwo.bestHand[0].cardVal != 1) {
			return 1;
		}
		else if (playerOne.bestHand[0].cardVal != 1 && playerTwo.bestHand[0].cardVal == 1) {
			return 2;
		}
		else if (playerOne.bestHand[0].cardVal > playerTwo.bestHand[0].cardVal) {
			return 1;
		}
		else if (playerOne.bestHand[0].cardVal < playerTwo.bestHand[0].cardVal) {
			return 2;
		}
		else {
			printf("Comparing kicker\n");
			return checkKicker(playerOne.kicker, playerTwo.kicker);
		}
	}


	//## TWO PAIR #####################
	else if (playerOne.handVal == 3) {
		printf("Comparing two pair\n");
		if (playerOne.bestHand[0].cardVal == 1 && playerTwo.bestHand[0].cardVal != 1) {
			return 1;
		}
		else if (playerOne.bestHand[0].cardVal != 1 && playerTwo.bestHand[0].cardVal == 1) {
			return 2;
		}
		else if (playerOne.bestHand[2].cardVal > playerTwo.bestHand[2].cardVal) {
			printf("%d\n", playerOne.bestHand[2].cardVal);
			printf("%d\n", playerTwo.bestHand[2].cardVal);
			return 1;
		}
		else if (playerOne.bestHand[2].cardVal < playerTwo.bestHand[2].cardVal) {
			//printf("hello\n");
			return 2;
		}
		else if (playerOne.bestHand[2].cardVal == playerTwo.bestHand[2].cardVal) {
			if (playerOne.bestHand[0].cardVal > playerTwo.bestHand[0].cardVal) {
				return 1;
			}
			else if (playerOne.bestHand[0].cardVal < playerTwo.bestHand[0].cardVal) {
				return 2;
			}
			else {
				printf("Comparing kicker\n");
				return checkKicker(playerOne.kicker, playerTwo.kicker);
			}
		}
	}


	//## SET ################################
	else if (playerOne.handVal == 4) {
		printf("Comparing set\n");
		if (playerOne.bestHand[0].cardVal == 1 && playerTwo.bestHand[0].cardVal != 1) {
			return 1;
		}
		else if (playerOne.bestHand[0].cardVal != 1 && playerTwo.bestHand[0].cardVal == 1) {
			return 2;
		}
		else if (playerOne.bestHand[0].cardVal > playerTwo.bestHand[0].cardVal) {
			return 1;
		}
		else if (playerOne.bestHand[0].cardVal < playerTwo.bestHand[0].cardVal) {
			return 2;
		}
		else {
			printf("Comparing kicker\n");
			return checkKicker(playerOne.kicker, playerTwo.kicker);
		}
	}


	// ## STRAIGHT ############################
	else if (playerOne.handVal == 5) {
		printf("Comparing Straight\n");
		if ((playerOne.bestHand[4].cardVal == 13 && playerOne.bestHand[0].cardVal == 1)
			&& !(playerTwo.bestHand[4].cardVal == 13 && playerTwo.bestHand[0].cardVal == 1)) {
			printf("Comparing ace-high straight\n");
			return 1;
		}
		else if ((playerTwo.bestHand[4].cardVal == 13 && playerTwo.bestHand[0].cardVal == 1)
			&& !(playerOne.bestHand[4].cardVal == 13 && playerOne.bestHand[0].cardVal == 1)) {
			printf("Comparing ace-high straight\n");
			return 2;
		}
		else if (playerOne.bestHand[4].cardVal > playerTwo.bestHand[4].cardVal) {
			return 1;
		}
		else if (playerOne.bestHand[4].cardVal < playerTwo.bestHand[4].cardVal) {
			return 2;
		}
		else {
			return 0;
		}
	}

	// ## FLUSH #################################
	else if (playerOne.handVal == 6) {
		int maxCard1 = 0;
		int maxCard2 = 0;

		printf("Comparing flush\n");
		if (playerOne.bestHand[4].cardVal == 1 && playerTwo.bestHand[4].cardVal != 1) {
			return 1;
		}
		else if (playerOne.bestHand[4].cardVal != 1 && playerTwo.bestHand[4].cardVal == 1) {
			return 2;
		}
		else {
			for (int i = 0; i < 5; i++) {
				if (playerOne.bestHand[i].cardVal > playerTwo.bestHand[i].cardVal) {
					return 1;
				}
				else if (playerOne.bestHand[i].cardVal < playerTwo.bestHand[i].cardVal) {
					return 2;
				}
			}
		}
		return 0;
	}

	//## FULL-HOUSE #########################
	if (playerOne.handVal == 7) {
		printf("Comparing full house\n");
		if (playerOne.bestHand[2].cardVal == 1 && playerTwo.bestHand[2].cardVal != 1) {
			return 1;
		}
		else if (playerOne.bestHand[2].cardVal != 1 && playerTwo.bestHand[2].cardVal == 1) {
			return 2;
		}
		else if (playerOne.bestHand[2].cardVal > playerTwo.bestHand[2].cardVal) {
			return 1;
		}
		else if (playerOne.bestHand[2].cardVal < playerTwo.bestHand[2].cardVal) {
			return 2;
		}
		else {
			if (playerOne.bestHand[0].cardVal == 1 && playerTwo.bestHand[0].cardVal != 1) {
				return 1;
			}
			else if (playerOne.bestHand[0].cardVal != 1 && playerTwo.bestHand[0].cardVal == 1) {
				return 2;
			}
			else if (playerOne.bestHand[0].cardVal > playerTwo.bestHand[0].cardVal) {
				return 1;
			}
			else if (playerOne.bestHand[0].cardVal < playerTwo.bestHand[0].cardVal) {
				return 2;
			}
			else {
				return 0;
			}
		}
	}

	// ## FOUR OF A KIND #########################
	else if (playerOne.handVal == 8) {
		printf("Comparing four of a kind\n");
		if (playerOne.bestHand[0].cardVal == 1 && playerTwo.bestHand[0].cardVal != 1) {
			return 1;
		}
		else if (playerOne.bestHand[0].cardVal != 1 && playerTwo.bestHand[0].cardVal == 1) {
			return 2;
		}
		else if (playerOne.bestHand[0].cardVal > playerTwo.bestHand[0].cardVal) {
			return 1;
		}
		else if (playerOne.bestHand[0].cardVal < playerTwo.bestHand[0].cardVal) {
			return 2;
		}
		else {
			return 0;
		}
	}

	// ## STRAIGH FLUSH ############################
	else if (playerOne.handVal == 9) {
		printf("Comparing straight flush\n");
		if (playerOne.bestHand[4].cardVal > playerTwo.bestHand[4].cardVal) {
			return 1;
		}
		else if (playerOne.bestHand[4].cardVal < playerTwo.bestHand[4].cardVal) {
			return 2;
		}
		else {
			return 0;
		}
	}


	return 0;
}

int checkKicker(int kicker1, int kicker2) {
	if (kicker1 == 1 && !(kicker2 == 1)) {
		return 1;
	}
	else if (kicker2 == 1 && !(kicker1 == 1)) {
		return 2;
	}
	if (kicker1 > kicker2) {
		return 1;
	}
	else if (kicker2 > kicker1) {
		return 2;
	}
	else {
		return 0;
	}
}


/*
#######################################################################################################
INITIALIZATION FUNCTIONS
#######################################################################################################
*/
void initializeComb() {
	int val = -2;
	for (int i = 0; i < 7; i++) {
		tmpCard[i].cardVal = val;
		tmpCard[i].cardType = val;
		val -= 2;
	}
}

void reInitCurrentHand() {
	for (int i = 0; i < 5; i++) {
		curBestHand[i].cardVal = 0;
		curBestHand[i].cardType = 0;
	}
}

void reInitPlayersHands(struct Player allPlayers[8]) {
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 5; j++) {
			allPlayers[i].bestHand[j].cardVal = 0;
			allPlayers[i].bestHand[j].cardType = 0;
		}
	}
}

void printHandsInPlay(struct Player* players, int maxPlayerAmount) {
	printf("Hands to compare:\n");
	for (int i = 0; i < maxPlayerAmount; i++) {
		if (players -> fold != 1 && players -> inPlay == 1) {
			printf("Player %d\n", players -> playerNumber);
			for (int j = 0; j < 5; j++) {
				printf("Card %d:\t##VALUE: %d\t##TYPE: %d\n", j + 1, players -> bestHand[j].cardVal, players -> bestHand[j].cardType);
			}
		}
		players = players -> next;
	}
}