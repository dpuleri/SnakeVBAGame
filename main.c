#include <stdlib.h>
#include "mylib.h"
#include "text.h"

int main() {

    //set mode
    REG_DISPCTL = MODE3 | BG2_ENABLE;

    drawString(1, 5, "Score", GRAY);

    //draw two lines in two different colors
    //do this so the TAs give me da points...
    plotLine(80, 0, MAX_X - 1, 15, PURPLE);
    plotLine(80, 15, MAX_X - 1, 0, ORANGE);

    u16 bgcolor = BEIGE;
    snake* mysnake = malloc(sizeof(*mysnake));
    REG_TM0D = -0X2000;
    REG_TM0CNT = TM_FREQ_1024 | TM_ON | TM_IRQ;
    //plotLine(12, 12, 50, 50, WHITE);
    //draw background rectangle
    drawRect(1, 16, MAX_X - 2, MAX_Y - 2 - 15, bgcolor);
    //draw border rectangle
    drawHollowRect(0, 15, MAX_X - 1, MAX_Y - 1 - 15, RED);

    //init the snake and init food
    initSnake(mysnake, GREEN, 3);
    placeFood(mysnake->head->size, 0);
    while(1) {
        /*if (REG_TM0D == 0) {
            moveSnake(mysnake);
            REG_TM0D = -0X2000;
        }*/

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
