#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

#define TOTAL_ROUND 5
#define TOTAL_PLAYERS 4
#define TOTAL_CARDS 40
#define sameColor(num1, num2) (((num1-1)/10 + (num2-1)/10) == 3)
#define sameNumber(num1, num2) ((num1-1)%10 == (num2-1)%10)
#define isPair(num1, num2) (sameNumber(num1, num2) && sameColor(num1, num2))
/*
【遊戲規則】
1. 牌組：40張牌，1莊3閒，共5輪，每輪每人2張牌。
2. 等級：對子 > 點數。
3. 對子：必須同色（紅磚+紅心 或 梅花+黑桃）。
   - 大小：10對 > 9對 > ... > A對。
4. 點數：(牌面1+牌面2)%10。
   - 大小：9 > 8 > ... > 1 > 0(目賊)。
5. 平手判定：
   - 點數相同時，比手中單張最大點數者勝（10最大）。
   - 若點數與最大單張皆相同（強碰），則莊家贏。
6. 特殊規則：閒家0點（目賊）一律莊家贏。

【花色編號】
C1~C10 (1~10), D1~D10 (11~20), H1~H10 (21~30), S1~S10 (31~40)
*/

class Gongjong {
public:
    struct Card {
        int number;
    };
    struct Player {
        Card hold[2];
        int score;
        int larger;
        int WL;
        int gotPair;
    };

    Gongjong() : index(0) {
        for(int i = 0; i < TOTAL_CARDS; ++i) {
            cards[i].number = 0;
        }
    }

    void Game() {
        initializeGame();
        for (int i = 0; i < TOTAL_ROUND; i++) {
            cout << "Round " << i + 1 << endl;
            dealCards();
            playRound();
            printResults();
        }
    }

    void initializeGame() {
        srand((unsigned int)time(NULL));
        cout << "Initializing game..." << endl;
        index = 0;
        for (int i = 0; i < TOTAL_CARDS; i++) {
            cards[i].number = i + 1;
        }
        for (int i = 0; i < TOTAL_CARDS; i++) {
            int r = rand() % TOTAL_CARDS;
            // Swap number of cards[i] and cards[r]
            int temp = cards[i].number;
            cards[i].number = cards[r].number;
            cards[r].number = temp;
        }
    }

    void dealCards() {
        cout << "Dealing cards..." << endl;
        for (int i = 0; i < TOTAL_PLAYERS; i++) {
            if (index + 1 < TOTAL_CARDS) {
                players[i].hold[0] = cards[index++];
                players[i].hold[1] = cards[index++];
            }
        }
    }

    void playRound() {
        int p1 = 0, p2 = 0;
        cout << "Playing round..." << endl;
        
        for (int i = 0; i < TOTAL_PLAYERS; i++) {
            cout << "Player " << i + 1 << " holds: ";
            for (int j = 0; j < 2; j++) {
                int val = players[i].hold[j].number;
                if (val >= 1 && val <= 10) cout << "♣" << val;
                else if (val >= 11 && val <= 20) cout << "♦" << val - 10;
                else if (val >= 21 && val <= 30) cout << "♥" << val - 20;
                else if (val >= 31 && val <= 40) cout << "♠" << val - 30;
                if (j == 0) cout << ", ";
            }
            cout << endl;
        }
        for (int i = 0; i < TOTAL_PLAYERS; i++) {
            p1 = (players[i].hold[0].number)%10;
            if (p1 == 0) p1 = 10;
            p2 = (players[i].hold[1].number)%10;
            if (p2 == 0) p2 = 10;
            if (isPair(players[i].hold[0].number, players[i].hold[1].number)) {
                players[i].gotPair = 1;
                players[i].score = p1;
            } else {
                players[i].gotPair = 0;
                players[i].score = (p1 + p2) % 10;
                players[i].larger = (p1 > p2) ? p1 : p2;
            }
        }
        if (players[0].gotPair) {
            for (int i = 1; i < TOTAL_PLAYERS; i++) {
                if (players[i].gotPair) {
                    if (players[i].score > players[0].score) {
                        players[i].WL = 1;
                    } else if (players[i].score <= players[0].score) {
                        players[i].WL = 0;
                    }
                } else {
                    players[i].WL = 0;
                }
            }
        } else {
            for (int i = 1; i < TOTAL_PLAYERS; i++) {
                if (players[i].gotPair) {
                    players[i].WL = 1;
                } else {
                    if (players[i].score == 0) {//閒家目賊
                        players[i].WL = 0;
                    } else if (players[i].score > players[0].score) {
                        players[i].WL = 1;
                    } else if (players[i].score == players[0].score) {
                        if (players[i].larger > players[0].larger) {
                            players[i].WL = 1;
                        } else {
                            players[i].WL = 0;
                        }
                    } else {
                        players[i].WL = 0;
                    }
                }
            }
        }
    }

    void printResults() {
        cout << "--- Results ---" << endl;
        for (int i = 0; i < TOTAL_PLAYERS; i++) {
            if (i == 0) cout << "[Master]   ";
            else cout << "[Player " << i + 1 << "] ";

            cout << "Score: " << players[i].score;
            if (players[i].gotPair) cout << " (PAIR!)";
            
            if (i > 0) {
                cout << " | " << (players[i].WL ? "WIN" : "LOSS");
            }
            cout << endl;
        }
        cout << "---------------" << endl << endl;
    }

private:
    Card cards[TOTAL_CARDS];
    Player players[TOTAL_PLAYERS];
    int index;
};

int main() {
    Gongjong game;
    game.Game();

    return 0;
}