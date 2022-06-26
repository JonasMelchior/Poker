#ifndef UNTITLED_EVAL_H
#define UNTITLED_EVAL_H

struct Card {
	int cardVal;
	int cardType;
};

struct Deck {

	struct Deck* last;
	struct Deck* next;

	struct Card cardInfo;
	int deckPlacement;
	int cardDrawn;
};

struct Player {
	int inPlay;
	int playerNumber;

	struct Player* next;
	struct Player* last;

	struct Deck card1;
	struct Deck card2;
	struct Card bestHand[5];
	int balance;
	int stake;
	int dealer;
	int kicker;
	int fold;
	int currentBet;
	int totalBet;
	int handVal;
	int allIn;
	int outs;
	int AIDificulty;

	int firstRoundTurn;

};


int evaluateHand(struct Player* player, struct Card curTable[5], int simulate);
void printSortedCard();
void printHandVal(int value);
void printBestHand(struct Card hand[5]);
void reInitCurrentHand();
void initializeComb();
char *getHandVal(int value);
int compareHands(int maxPlayerAmount, struct Player *player);
void reInitPlayersHands(struct Player allPlayers[8]);
int* getWinnerArr();
struct Card* getTmpCards();

#endif //UNTITLED_STATEMACHINE_H