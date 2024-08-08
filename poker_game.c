#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Define constants
#define DECK_SIZE 52
#define MAX_DECKS 4
#define PLAYER_COUNT 2
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_CARDS 12 // Maximum number of cards a player can have

// Card structure
typedef struct {
    int value;   // Card value
    char suit;   // Card suit
} Card;

// Player structure
typedef struct {
    Card hand[MAX_CARDS]; // Player hand (max 12 cards)
    int cardCount;        // Number of cards in hand
    int totalPoints;      // Total points in hand
} Player;

// Function prototypes
void initializeDeck(Card deck[], int numDecks);
void shuffleDeck(Card deck[], int numDecks);
Card drawCard(Card deck[], int *deckIndex);
int calculatePoints(Player *player);
void dealInitialCards(Player players[], Card deck[], int *deckIndex);
void displayCard(SDL_Renderer *renderer, Card card, int x, int y, TTF_Font *font);
void displayHand(SDL_Renderer *renderer, Player player, int showAll, int x, int y, TTF_Font *font);
void displayText(SDL_Renderer *renderer, const char* text, int x, int y, TTF_Font *font, SDL_Color color);
void displayHelp(SDL_Renderer *renderer, TTF_Font *font);
void gameLoop(SDL_Renderer *renderer, Player players[], Card deck[], int numDecks, TTF_Font *font);

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a window
    SDL_Window *win = SDL_CreateWindow("21-Point Poker Game", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(win);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load a font
    TTF_Font *font = TTF_OpenFont("DejaVuSans.ttf", 24);
    if (font == NULL) {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Seed random number generator
    srand(time(0));

    // Initialize deck and players
    Card deck[DECK_SIZE * MAX_DECKS];
    Player players[PLAYER_COUNT];
    int numDecks = 2; // Number of decks to use
    int deckIndex = 0;

    initializeDeck(deck, numDecks);
    shuffleDeck(deck, numDecks);

    // Initialize players
    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].cardCount = 0;
        players[i].totalPoints = 0;
    }

    // Deal initial cards
    dealInitialCards(players, deck, &deckIndex);

    // Start game loop
    gameLoop(renderer, players, deck, numDecks, font);

    // Clean up
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void initializeDeck(Card deck[], int numDecks) {
    char suits[] = {'H', 'D', 'S', 'C'};
    int index = 0;

    for (int d = 0; d < numDecks; d++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 1; j <= 13; j++) {
                deck[index].value = j;
                deck[index].suit = suits[i];
                index++;
            }
        }
    }
}

void shuffleDeck(Card deck[], int numDecks) {
    int n = DECK_SIZE * numDecks;
    for (int i = 0; i < n; i++) {
        int r = rand() % n;
        Card temp = deck[i];
        deck[i] = deck[r];
        deck[r] = temp;
    }
}

Card drawCard(Card deck[], int *deckIndex) {
    return deck[(*deckIndex)++];
}

int calculatePoints(Player *player) {
    int points = 0;
    int aceCount = 0;

    for (int i = 0; i < player->cardCount; i++) {
        int value = player->hand[i].value;
        if (value > 10) {
            points += 10;
        } else if (value == 1) {
            aceCount++;
            points += 11;
        } else {
            points += value;
        }
    }

    while (points > 21 && aceCount > 0) {
        points -= 10;
        aceCount--;
    }

    player->totalPoints = points;
    return points;
}

void dealInitialCards(Player players[], Card deck[], int *deckIndex) {
    for (int i = 0; i < PLAYER_COUNT; i++) {
        players[i].hand[0] = drawCard(deck, deckIndex);
        players[i].hand[1] = drawCard(deck, deckIndex);
        players[i].cardCount = 2;
        calculatePoints(&players[i]);
    }
}

void displayCard(SDL_Renderer *renderer, Card card, int x, int y, TTF_Font *font) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = 50;
    rect.h = 70;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    char valueStr[3];
    if (card.value == 1) {
        sprintf(valueStr, "A");
    } else if (card.value > 10) {
        if (card.value == 11) sprintf(valueStr, "J");
        if (card.value == 12) sprintf(valueStr, "Q");
        if (card.value == 13) sprintf(valueStr, "K");
    } else {
        sprintf(valueStr, "%d", card.value);
    }

    char suitStr[2] = {card.suit, '\0'};
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* valueSurface = TTF_RenderText_Solid(font, valueStr, textColor);
    SDL_Surface* suitSurface = TTF_RenderText_Solid(font, suitStr, textColor);

    SDL_Texture* valueTexture = SDL_CreateTextureFromSurface(renderer, valueSurface);
    SDL_Texture* suitTexture = SDL_CreateTextureFromSurface(renderer, suitSurface);

    SDL_Rect valueRect;
    valueRect.x = x + 5;
    valueRect.y = y + 5;
    valueRect.w = valueSurface->w;
    valueRect.h = valueSurface->h;

    SDL_Rect suitRect;
    suitRect.x = x + 5;
    suitRect.y = y + 35;
    suitRect.w = suitSurface->w;
    suitRect.h = suitSurface->h;

    SDL_RenderCopy(renderer, valueTexture, NULL, &valueRect);
    SDL_RenderCopy(renderer, suitTexture, NULL, &suitRect);

    SDL_FreeSurface(valueSurface);
    SDL_FreeSurface(suitSurface);
    SDL_DestroyTexture(valueTexture);
    SDL_DestroyTexture(suitTexture);
}

void displayHand(SDL_Renderer *renderer, Player player, int showAll, int x, int y, TTF_Font *font) {
    for (int i = 0; i < player.cardCount; i++) {
        if (showAll || i == 0) {
            displayCard(renderer, player.hand[i], x + i * 55, y, font);
        } else {
            SDL_Rect rect;
            rect.x = x + i * 55;
            rect.y = y;
            rect.w = 50;
            rect.h = 70;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void displayText(SDL_Renderer *renderer, const char* text, int x, int y, TTF_Font *font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void displayHelp(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    displayText(renderer, "21-Point Poker Game Instructions", 50, 50, font, textColor);
    displayText(renderer, "1. Each player is dealt two cards initially.", 50, 100, font, textColor);
    displayText(renderer, "2. Players take turns to either 'Hit' or 'Stand'.", 50, 150, font, textColor);
    displayText(renderer, "3. 'Hit' means to take another card.", 50, 200, font, textColor);
    displayText(renderer, "4. 'Stand' means to keep your current hand.", 50, 250, font, textColor);
    displayText(renderer, "5. Cards 2-10 are worth their face value.", 50, 300, font, textColor);
    displayText(renderer, "6. J, Q, K are worth 10 points each.", 50, 350, font, textColor);
    displayText(renderer, "7. Aces can be worth 1 or 11 points.", 50, 400, font, textColor);
    displayText(renderer, "8. If a player's total exceeds 21, they bust and lose.", 50, 450, font, textColor);
    displayText(renderer, "9. The player with the highest total <= 21 wins.", 50, 500, font, textColor);
    displayText(renderer, "Press any key to return to the game.", 50, 550, font, textColor);

    SDL_RenderPresent(renderer);

    // Wait for key press to return to game
    SDL_Event e;
    int wait = 1;
    while (wait) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                wait = 0;
                exit(0);
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                wait = 0;
            }
        }
    }
}

void gameLoop(SDL_Renderer *renderer, Player players[], Card deck[], int numDecks, TTF_Font *font) {
    int deckIndex = numDecks * DECK_SIZE;
    int playerIndex = 0;
    int running = 1;
    SDL_Event e;
    SDL_Color textColor = {255, 255, 255, 255};

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Check if "Hit" button was clicked
                if (x >= 50 && x <= 150 && y >= 500 && y <= 550) {
                    if (players[playerIndex].cardCount < MAX_CARDS) {
                        players[playerIndex].hand[players[playerIndex].cardCount++] = drawCard(deck, &deckIndex);
                        int points = calculatePoints(&players[playerIndex]);

                        if (points > 21) {
                            printf("Player %d busts with %d points!\n", playerIndex + 1, points);
                            running = 0;
                            break;
                        }
                    } else {
                        printf("Player %d cannot draw more cards.\n", playerIndex + 1);
                    }
                }

                // Check if "Stand" button was clicked
                if (x >= 200 && x <= 300 && y >= 500 && y <= 550) {
                    playerIndex++;
                    if (playerIndex >= PLAYER_COUNT) {
                        running = 0;
                        break;
                    }
                }

                // Check if "Help" button was clicked
                if (x >= 350 && x <= 450 && y >= 500 && y <= 550) {
                    displayHelp(renderer, font);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
        SDL_RenderClear(renderer);

        displayHand(renderer, players[0], playerIndex > 0, 50, 50, font);
        displayHand(renderer, players[1], 1, 50, 200, font);

        // Draw buttons
        SDL_Rect hitButton = {50, 500, 100, 50};
        SDL_Rect standButton = {200, 500, 100, 50};
        SDL_Rect helpButton = {350, 500, 100, 50};

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &hitButton);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &standButton);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &helpButton);

        displayText(renderer, "Hit", 75, 515, font, textColor);
        displayText(renderer, "Stand", 215, 515, font, textColor);
        displayText(renderer, "Help", 375, 515, font, textColor);

        SDL_RenderPresent(renderer);
    }

    // Compare points
    int winner = -1;
    int maxPoints = 0;

    for (int i = 0; i < PLAYER_COUNT; i++) {
        printf("Player %d's final hand: ", i + 1);
        displayHand(renderer, players[i], 1, 50, 50 + i * 150, font);
        SDL_RenderPresent(renderer);
        printf("Total points: %d\n", players[i].totalPoints);

        if (players[i].totalPoints > maxPoints && players[i].totalPoints <= 21) {
            maxPoints = players[i].totalPoints;
            winner = i;
        }
    }

    if (winner != -1) {
        printf("Player %d wins with %d points!\n", winner + 1, maxPoints);
    } else {
        printf("No winner!\n");
    }
}
