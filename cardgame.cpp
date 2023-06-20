#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>




typedef struct Card {
    int num;
    char suit;
} Card;

char name[4] = { 'J', 'Q', 'K', 'A' };


Card* create_deck(Card* Deck, int size) {
    int cur = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 4; j++) {
            do { cur = rand() % size; } while (Deck[cur].num != 0);
            Deck[cur].num = i + 6;
            Deck[cur].suit = j + 3;
        }
    }
    return Deck;
}
Card* create_hand_pl(Card* Deck, Card* Player, int size) {

    for (int i = 0; i < 6; i++) {
        Player[i].num = Deck[size - 1].num;
        Player[i].suit = Deck[size - 1].suit;
        size--;
        Deck = (Card*)realloc((void*)Deck, size * sizeof(Card));
    }
    return Player;
}
Card* create_hand_bot(Card* Deck, Card* Bot, int size) {

    for (int i = 0; i < 6; i++) {
        Bot[i].num = Deck[size - 1].num;
        Bot[i].suit = Deck[size - 1].suit;
        size--;
        Deck = (Card*)realloc((void*)Deck, size * sizeof(Card));
    }
    return Bot;
}
int move_result() { return -1; }

void printDeck(Card* Deck, int size) {
   
    for (int i = 0; i < size; i++) {
        Deck[i].num < 11? printf("%d) [%d %c]\n", i + 1, Deck[i].num, Deck[i].suit): printf("%d) [%c %c]\n", i + 1, name[Deck[i].num - 11], Deck[i].suit);
    }

}
void printHand(Card* Hand) {
    for (int i = 0; i < 6; i++) {
        Hand[i].num < 11 ? printf("%d) [%d %c]\n", i + 1, Hand[i].num, Hand[i].suit) : printf("%c %c\n", name[Hand[i].num - 11], Hand[i].suit);
    }
    printf("All done!\n");
}
int start(Card* Bot, Card* Player, char Trump) {
    int min = 15;
    int s = 0;// 0 - бот, 1 - игрок
    for (int i = 0; i < 6; i++) {
        if (Bot[i].suit == Trump && Bot[i].num < min) { min = Bot[i].num; s = 1; }
        if (Player[i].suit == Trump && Player[i].num < min) { min = Player[i].num; s = 0; }
    }
    return s;
}

int arr_size(Card* Hand) {
    int i = 0;
    while (Hand[i].num && Hand[i].suit) i++;
    return i;
}


int game(int move, Card* Player, Card* Bot, Card* Deck, int* size, char Trump, int* bot_size, int* player_size) {
   
    Card* Table = (Card*)calloc(*player_size + *bot_size, sizeof(Card));
    printf("Player size: %d; Bot size:%d\n", *player_size, *bot_size);
    printf("Deck size is %d\n", *size);
    printf("Trump - %c\n", Trump);
    while (!(*size == 0 && (player_size == 0 || bot_size == 0))) {
        int i = 0;
        if (move == 1) {
            bool Flag = false;
            printf("Player: \n");
            printDeck(Player, *player_size);
            printf("\n");
            printf("Bot: \n");
            printDeck(Bot, *bot_size);

            printf("Выберите номер карты, которой будете ходить: ");
            int n = 0;
            scanf_s("%d", &n);
            if (n > *player_size) { printf("Неправильно введено число"); system("cls");  continue; }
            n--;
            Table[i] = Player[n];
            for (int j = 0; j < *bot_size; j++) {
                if (Table[i].suit == Trump) {
                    if (Bot[j].suit == Trump && Bot[j].num > Table[i].num) {
                        Flag = true;
                        Table[++i] = Bot[j];
                        Bot[j] = Bot[*bot_size - 1];
                        if (*size == 0) {
                            Bot = (Card*)realloc((void*)Bot, --(*bot_size) * sizeof(Card));
                            break;
                        }
                        else {
                            Bot[(*bot_size) - 1] = Deck[*size - 1];
                            Deck = (Card*)realloc((void*)Deck, -- * size * sizeof(Card));
                            break;
                        }
                    }
                }
                else if (Table[i].suit != Trump) {
                    if ((Bot[j].suit == Trump) || ((Bot[j].num > Table[i].num) && (Bot[j].suit == Table[i].suit))) {
                        Flag = true;
                        Table[++i] = Bot[j];
                        Bot[j] = Bot[(*bot_size) - 1];
                        if (*size == 0) {
                            Bot = (Card*)realloc((void*)Bot, --(*bot_size) * sizeof(Card));
                            break;
                        }
                        else {
                            Bot[(*bot_size) - 1] = Deck[*size - 1];
                            Deck = (Card*)realloc((void*)Deck, -- * size * sizeof(Card));
                            break;
                        }

                    }
                }

            }
            if (Flag) {
                printf("Бот ответил\n");
                system("pause");
                Player[n] = Player[*player_size - 1];
                if (*size != 0) {
                    Player[*player_size - 1] = Deck[*size - 1];
                    Deck = (Card*)realloc((void*)Deck, -- * size * sizeof(Card));

                }
                else {
                    Player = (Card*)realloc((void*)Player, --(*player_size) * sizeof(Card));
                }

                return 1;
            }
            else {
                printf("Бот взял\n");
                
                Bot = (Card*)realloc((void*)Bot, ++(*bot_size) * sizeof(Card));
                
                Bot[*bot_size - 1 ] = Table[n];
                //*bot_size++;
                
                Player[n] = Player[*player_size - 1];
                if (*size != 0) {
                    Player[*player_size - 1] = Deck[*size - 1];
                    Deck = (Card*)realloc((void*)Deck, -- * size * sizeof(Card));

                }
                else {
                    Player = (Card*)realloc((void*)Player, --(*player_size) * sizeof(Card));
                }
                system("pause");
                return 1;
            }
        }


    }
    system("pause");
    return -1;
}



int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    printf("Hello!\n");
    int size = 36;
    Card* Player = (Card*)calloc(6, sizeof(Card));
    Card* Bot = (Card*)calloc(6, sizeof(Card));
    Card* Deck = (Card*)calloc(size, sizeof(Card));
    //printDeck(Deck, size);
    Deck = create_deck(Deck, size);
    Player = create_hand_pl(Deck, Player, size);
    size -= 6;
    Bot = create_hand_bot(Deck, Bot, size);
    size -= 6;
    // Карты на руках
    char Trump = (rand() % 4) + 3;
    int bot_size = 6;
    int player_size = 6;
    int move = start(Player, Bot, Trump);
    while (true) {
        system("cls");
        move = game(move, Player, Bot, Deck, &size, Trump, &bot_size, &player_size);
        if (move == -1) break;
    }
    system("cls");
    printf("Game over!");

}