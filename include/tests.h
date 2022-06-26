void generateRandomHands(struct Player *player, struct Deck* tmpCards, int currentPlayerAmount);
void generateFlop(struct Card *table, struct Deck* tmpCards);
void generateTurn(struct Card *table, struct Deck* tmpCards);
void generateRiver(struct Card *table, struct Deck* tmpCards);
void writeTestResults(struct Player *player, struct Card *tmpCards, struct Card *table);
void writeComparisonTest(int winnerIndex);
void test();