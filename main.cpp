#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

#define TOTAL_ROUND 5
#define TOTAL_PLAYERS 4
#define TOTAL_CARDS 40
#define sameColor(num1, num2) (((num1-1)/10 + (num2-1)/10) == 3)

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
    string cardToStr(int num) {
        string suits[] = {"♣", "♦", "♥", "♠"};
        return suits[(num - 1) / 10] + to_string((num - 1) %10 + 1);
    }
    struct Player {
        int cards[2];
        int score, larger, WL, gotPair;

        void evaluate() {
            int p1 = (cards[0] - 1) % 10 + 1;
            int p2 = (cards[1] - 1) % 10 + 1;
            gotPair = (p1 == p2) && (sameColor(cards[0], cards[1]));
            score = gotPair ? p1 : (p1 + p2) % 10;
            larger = max(p1, p2);
        }
    } players[TOTAL_PLAYERS];

    Gongjong() : index(0) {
        for(int i = 0; i < TOTAL_CARDS; ++i) {
            decks[i] = 0;
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
            decks[i] = i + 1;
        }
        for (int i = 0; i < TOTAL_CARDS; i++) {
            int r = rand() % TOTAL_CARDS;
            // Swap number of decks[i] and decks[r]
            int temp = decks[i];
            decks[i] = decks[r];
            decks[r] = temp;
        }
    }

    void dealCards() {
        cout << "Dealing cards..." << endl;
        for (int i = 0; i < TOTAL_PLAYERS; i++) {
            if (index + 1 < TOTAL_CARDS) {
                players[i].cards[0] = decks[index++];
                players[i].cards[1] = decks[index++];
            }
        }
    }
    bool doesPlayerWin(Player& p, Player& m) {
        if (p.gotPair != m.gotPair) return p.gotPair;
        if (p.score != m.score) return (p.score > m.score);
        return p.larger > m.larger;
    }

    void playRound() {
        cout << "Playing round..." << endl;
        for (int i = 0; i < TOTAL_PLAYERS; i++) {
            cout << "Player " << i + 1 << " holds: ";
            for (int j = 0; j < 2; j++) {
                cout << cardToStr(players[i].cards[j]) << " ";
            }
            cout << endl;
        }
        for (auto& p : players) p.evaluate();
        for (int i = 1; i < TOTAL_PLAYERS; i++) {
            if (players[i].score == 0) {
                players[i].WL = 0;
            } else {
                players[i].WL  = doesPlayerWin(players[i], players[0]);
            }
        }
    }

    void printResults() {
        cout << "--- Results ---" << endl;
        for (int i = 0; i < TOTAL_PLAYERS; i++) {
            if (i == 0) cout << "[Master  ] ";
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
    int decks[TOTAL_CARDS];
    int index;
};

int main() {
    Gongjong game;
    game.Game();

    return 0;
}