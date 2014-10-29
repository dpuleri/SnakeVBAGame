#include <stdlib.h>
#include "mylib.h"
#include "text.h"
#include "titleScreen.h"

void startTitleScreen();

int main() {

    //set mode
    REG_DISPCTL = MODE3 | BG2_ENABLE;
    startTitleScreen();

    snake* mysnake = malloc(sizeof(*mysnake));
    u16 bgcolor = BEIGE;
    initGame(mysnake, bgcolor); 
    while(1) {
        //default 50
        for (int i = 0; i < 50; i++) {
            while(SCANLINECOUNTER > 160);
            while(SCANLINECOUNTER < 160);
            if (KEY_DOWN_NOW(BUTTON_UP) && mysnake->direction != down) {
                updateSnakeDirection(mysnake, up);
            } else if (KEY_DOWN_NOW(BUTTON_DOWN) && mysnake->direction != up) {
                updateSnakeDirection(mysnake, down);
            } else if (KEY_DOWN_NOW(BUTTON_LEFT) && mysnake->direction != right) {
                updateSnakeDirection(mysnake, left);
            } else if (KEY_DOWN_NOW(BUTTON_RIGHT) && mysnake->direction != left) {
                updateSnakeDirection(mysnake, right);
            }
        }
        moveSnake(mysnake, bgcolor);
    }
    return 0;
}

void startTitleScreen() {
    /*for (int x = 0; x < MAX_X; x++) {
        for (int y = 0; y < MAX_Y; y++) {
            setPixel(x, y, titleScreen[OFFSET(x, y, MAX_X)]);
        }
    }*/
    printScreen(titleScreen);
    int pressedStart = 0;
    while (!pressedStart) {
        if (KEY_DOWN_NOW(BUTTON_START)) {
            pressedStart++;
        }
    }
    waitForVBlank();
    drawRect(0, 0, MAX_X - 1, MAX_Y - 1, BLACK);
}
