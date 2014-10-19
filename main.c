#include <stdlib.h>
#include "mylib.h"

int main() {


    u16 bgcolor = BEIGE;
    snake* mysnake = malloc(sizeof(*mysnake));
    setPixel(1, 1, WHITE);
    REG_TM0D = -0X2000;
    REG_TM0CNT = TM_FREQ_1024 | TM_ON | TM_IRQ;
    //plotLine(12, 12, 50, 50, WHITE);
    drawRect(1, 16, MAX_X - 2, MAX_Y - 2 - 15, bgcolor);
    drawHollowRect(0, 15, MAX_X - 1, MAX_Y - 1 - 15, RED);
    initSnake(mysnake, GREEN, 3);
    placeFood(mysnake->head->size, bgcolor);
    while(1) {
        /*if (REG_TM0D == 0) {
            moveSnake(mysnake);
            REG_TM0D = -0X2000;
        }*/
        if (KEY_DOWN_NOW(BUTTON_UP) && mysnake->direction != down) {
            updateSnakeDirection(mysnake, up);
        } else if (KEY_DOWN_NOW(BUTTON_DOWN) && mysnake->direction != up) {
            updateSnakeDirection(mysnake, down);
        } else if (KEY_DOWN_NOW(BUTTON_LEFT) && mysnake->direction != right) {
            updateSnakeDirection(mysnake, left);
        } else if (KEY_DOWN_NOW(BUTTON_RIGHT) && mysnake->direction != left) {
            updateSnakeDirection(mysnake, right);
        }

        for (int i = 0; i < 50; i++) {
            while(SCANLINECOUNTER > 160);
            while(SCANLINECOUNTER < 160);
        }
        moveSnake(mysnake, bgcolor);
    }
    return 0;
}
