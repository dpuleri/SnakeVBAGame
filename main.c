#include <stdlib.h>
#include "mylib.h"

int main() {


    snake* mysnake = malloc(sizeof(*mysnake));
    setPixel(1, 1, WHITE);
    REG_TM0D = -0X2000;
    REG_TM0CNT = TM_FREQ_1024 | TM_ON | TM_IRQ;
    //plotLine(12, 12, 50, 50, WHITE);
    drawRect(1, 16, 240 - 2, 160 - 2 - 15, BEIGE);
    drawHollowRect(0, 15, 240 - 1, 160 - 1 - 15, RED);
    initSnake(mysnake, GREEN, 3);
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
        moveSnake(mysnake, BEIGE);
    }
    return 0;
}
