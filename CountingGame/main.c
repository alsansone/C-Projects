#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
/* Created by Adam Sansone */

/* @param current player, @return 1 or 2 */
int switchPlayers(int player_select) {
    if (player_select == 1)
        return 2;

    return 1;
}
/* prompts user to enter guess, returns the guess entered */
int getGuess() {
    int guess;
    printf("Enter your guess: ");
    scanf("%d", &guess);

    return guess;
}
/* @param the random number generated, players guess */
int checkGuess(int number, int guess) {
    if (guess > number)
        printf("Guess was too high\n");
    else if(guess < number)
        printf("Guess was too low\n");
    else
        printf("You guessed correctly, you win!!!");
}
/* main method */
int main() {
    int number; // holds the random number to be guessed
    int guess; // holds the players guess
    int player_select; // holds the current player
    int player_input = 0; // holds player turn input
    int player_one_passes = 3, player_two_passes = 3; // holds the number of passes for each player
    int passed_last_one = 0, passed_last_two = 0; // holds consecutive passes
    char pass[4]; // holds a char array for "PASS" input

    /* select which player will go first and the number to be guessed */
    srand(time(NULL)); // uses time function as a seed to change the random numbers generated each game
    player_select = 1 + rand() % 2; // randomly generates a 1 or a 2
    number = 1 + rand() % 100; // randomly generates a number from 1 to 100
    /* loop until player guesses number */
    while(guess != number) {
        /* loop until correct player number is entered */
        while(player_select != player_input) {
            printf("It is player %d's turn\n", player_select);
            printf("Enter your player number: ");
            scanf("%d", &player_input);

            if(player_input != player_select)
                printf("You have to wait your turn\n");
        }

        printf("Enter your guess, or type PASS to skip your turn: ");
        scanf("%s", &pass);
        /* checks if PASS was entered */
        if (strcmp(pass, "PASS") == 0) {
            if (player_select == 1) { // check which players turn it is
                if (player_one_passes > 0) { // checks if player has enough passes
                    if (passed_last_one == 0) { // checks if they passed last turn
                        player_one_passes--; // decrement passes
                        passed_last_one = 1; // update that player passed this turn
                        printf("You have passed %d time(s), you have %d passes left\n", 3-player_one_passes, player_one_passes); // display remaining passes
                        player_select = switchPlayers(player_select); // run player switch procedure
                    } else { // player passed last turn
                        printf("You cannot pass twice in a row\n");
                        passed_last_one = 0; // update player passing
                        guess = getGuess(); // run guess procedure
                        checkGuess(number, guess); // run check guess procedure
                        player_select = switchPlayers(player_select); // run player switch procedure
                    }
                } else { // player is out of passes
                    printf("You are out of passes\n");
                    guess = getGuess(); // run guess procedure
                    checkGuess(number, guess); // run check guess procedure
                    player_select = switchPlayers(player_select); // switch players
                }
            } else { // player 2's turn. Repeat player 1 instructions
                if (player_two_passes > 0) {
                    if (passed_last_two == 0) {
                        player_two_passes--;
                        passed_last_two = 1;
                        printf("You have passed %d time(s), you have %d passes left\n", 3-player_two_passes, player_two_passes);
                        player_select = switchPlayers(player_select);
                    } else {
                        printf("You can not pass twice in a row\n");
                        passed_last_two = 0;
                        guess = getGuess();
                        checkGuess(number, guess);
                        player_select = switchPlayers(player_select);
                    }
                } else {
                    printf("You are out of passes\n");
                    guess = getGuess();
                    checkGuess(number, guess);
                    player_select = switchPlayers(player_select);
                }
            }
        } else { // player entered a guess
            guess = atoi(pass); // convert string to int value

            checkGuess(number, guess); // run check guess procedure

            if (player_select == 1) // update player pass status
                passed_last_one = 0;
            else
                passed_last_two = 0;

            player_select = switchPlayers(player_select); //  run player switch procedure
        }
    }
}
