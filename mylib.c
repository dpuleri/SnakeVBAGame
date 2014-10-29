
#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "text.h"
#include "endScreen.h"

u16* videoBuffer = VID_BFFR; //define global var
int num2lengthen = 0;


void initSnake(snake* mysnake, u16 color, int snakeSize) {
    node* head = malloc(sizeof(*head));;
    node* prevNode = head;
    head->x = MAX_X / 2 - 1;
    head->y = MAX_Y / 2 - 1;
    head->color = color;
    head->size = snakeSize;
    printSnakeNode(head);
    for (int i = 1; i < 4; i++) {
        node* newNode = malloc(sizeof(*newNode));
        prevNode->next = newNode;
        newNode->previous = prevNode;
        newNode->x = head->x - (i * snakeSize);
        newNode->y = head->y;
        newNode->color = color;
        newNode->size = snakeSize;
        prevNode = newNode;
        printSnakeNode(prevNode);
    }
    mysnake->head = head;
    mysnake->tail = prevNode;
    mysnake->color = color;
    mysnake->direction = right;
    mysnake->length = 4;
}

void printSnakeNode(node* node) {
    //setPixel(node->x, node->y, node->color);
    drawRect(node->x, node->y, node->size, node->size, node->color);
}

//sets a pixel
void setPixel(int x, int y, u16 color) {
    int offset = OFFSET(x, y, MAX_X);
    //int offset = (y * MAX_X) + x; //old code
    //*((u16*) (VID_BFFR + offset)) = color;
    videoBuffer[offset] = color;
}

//gets the color of a pixel
u16 getPixelColor(int x, int y) {
    int offset = (y * 240) + x;
    return *((u16*) (VID_BFFR + offset));
}

void drawRect(int x, int y, int width, int height, u16 color) {
    for (int i = x; i < (x + width); i++) {
        for (int j = y; j < (y + height); j++) {
            setPixel(i, j, color);
        }
    }
}

void drawHollowRect(int x, int y, int width, int height, u16 color) {
    for (int i = x; i < (x + width); i++) {
        setPixel(i, y, color);
        setPixel(i, y + height, color);
    }
    for (int j = y; j < (y + height); j++) {
        setPixel(x, j, color);
        setPixel(x + width, j, color);
    }
}

void plotLine(int x0, int y0, int x1, int y1, u16 color) {
    int dx = abs(x1 - x0);
    int sx = SIGN(x1 - x0);
    int dy = abs(y1 - y0);
    int sy = SIGN(y1 - y0); 
    int err = (dx > dy ? dx : (~dy + 1)) >> 1;
    int e2;
 
    while (x0 != x1 || y0 != y1) {
        setPixel(x0, y0, color);
        e2 = err;
        if (e2 > (~dx + 1)) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
    setPixel(x1, y1, color);
}

void initGame(snake* mysnake, u16 bgcolor) {
    drawString(1, 5, "Score", GRAY);
    updateScore(0);

    /*//draw two lines in two different colors
    //do this so the TAs give me da points...
    plotLine(80, 0, MAX_X - 1, 15, PURPLE);
    plotLine(80, 15, MAX_X - 1, 0, ORANGE);
*/
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
}

//var to count if the snake is being lengthened
void moveSnake(snake* mysnake, u16 bgcolor) {
    static int hasCollided = 0;
    static int score = 0;

    node* nodeMoving;

    if (hasCollided) {
        return;
    } else if (num2lengthen) {
        nodeMoving = malloc(sizeof(*nodeMoving));
        nodeMoving->size = mysnake->head->size;
        num2lengthen--;
        mysnake->length = mysnake->length + 1;
        if (!num2lengthen) {
            placeFood(mysnake->head->size, 0);
        }
    } else {
        nodeMoving = mysnake->tail;
        mysnake->tail = mysnake->tail->previous;
        mysnake->tail->next = NULL;
        //take care of turning off the tail
        nodeMoving->color = bgcolor;
        printSnakeNode(nodeMoving);
    }

    nodeMoving->color = mysnake->color;
    nodeMoving->x = mysnake->head->x;
    nodeMoving->y = mysnake->head->y;

    if (mysnake->direction == up) {
        nodeMoving->y = mysnake->head->y - mysnake->head->size;
    } else if (mysnake->direction == down) {
        nodeMoving->y = mysnake->head->y + mysnake->head->size;
    } else if (mysnake->direction == left) {
        nodeMoving->x = mysnake->head->x - mysnake->head->size;
    } else {
        nodeMoving->x = mysnake->head->x + mysnake->head->size;
    }

    if (hasEatenFood(nodeMoving)) {
        placeFood(mysnake->head->size, bgcolor);
        num2lengthen += 4;
        score++;
        updateScore(score);
    } else if (isCollided(nodeMoving, bgcolor)) {
        nodeMoving->color = GRAY;
        score = 0;
        gameOver(mysnake); 
        initGame(mysnake, bgcolor);
        return;
    }
    printSnakeNode(nodeMoving);

    mysnake->head->previous = nodeMoving;
    nodeMoving->next = mysnake->head;
    mysnake->head = nodeMoving;
}

int hasEatenFood(node* curNode) {
    for (int i = curNode->x; i < (curNode->x + curNode->size); i++) {
        for (int j = curNode->y; j < (curNode->y + curNode->size); j++) {
            if (getPixelColor(i, j) == FOOD_COLOR) {
                return 1;
            }
        }
    }
    return 0;
}

int isCollided(node* curNode, u16 bgcolor) {
    for (int i = curNode->x; i < (curNode->x + curNode->size); i++) {
        for (int j = curNode->y; j < (curNode->y + curNode->size); j++) {
            if (getPixelColor(i, j) != bgcolor) {
                return 1;
            }
        }
    }
    return 0;
}

//updates the direction in which the snake is moving
void updateSnakeDirection(snake* mysnake, direction newdirection) {
    mysnake->direction = newdirection;
}

//places the food in a random place on the map
//if the bgcolor != 0 then undo last food palcement
void placeFood(int nodeSize, u16 bgcolor) {
    static int x = 0;
    static int y = 0;
    if (bgcolor) {
        drawRect(x, y, nodeSize, nodeSize, bgcolor);
    } else {
        x = (rand() % (MAX_X - nodeSize - 1)) + 1;
        y = (rand() % (MAX_Y - nodeSize - 15 - 1)) + 15;
        drawRect(x, y, nodeSize, nodeSize, FOOD_COLOR);
    }
}

void updateScore(int score) {
    drawRect(40, 5, 8 * 5, 7, BLACK);
    char* scoreStrPtr = int2str(score);
    drawString(40, 5, scoreStrPtr, WHITE);
    free(scoreStrPtr);
}

char* int2str(int number) {
    int buffsize = 2; //account for /0 and first char
    int tmpNum = number;
    while (tmpNum > 9) {
        buffsize++;
        tmpNum /= 10;
    }
    char* buffer = malloc(buffsize);
    sprintf(buffer, "%d", number);
    return buffer;
}

void printScreen(const u16* screen) {
    waitForVBlank();
    for (int x = 0; x < MAX_X; x++) {
        for (int y = 0; y < MAX_Y; y++) {
            setPixel(x, y, screen[OFFSET(x, y, MAX_X)]);
        }
    }
}

void waitForVBlank() {
    while(SCANLINECOUNTER >= 160);
    while(SCANLINECOUNTER < 160);
}

void gameOver(snake* mysnake) {
    printScreen(endScreen);
    int pressedKey = 0;
    while (!pressedKey) {
        if (KEY_DOWN_NOW(BUTTON_START)
            || KEY_DOWN_NOW(BUTTON_L)
            || KEY_DOWN_NOW(BUTTON_R)
            || KEY_DOWN_NOW(BUTTON_SELECT)
            || KEY_DOWN_NOW(BUTTON_UP)
            || KEY_DOWN_NOW(BUTTON_DOWN)
            || KEY_DOWN_NOW(BUTTON_LEFT)
            || KEY_DOWN_NOW(BUTTON_RIGHT)
            || KEY_DOWN_NOW(BUTTON_A)
            || KEY_DOWN_NOW(BUTTON_B)) {
            pressedKey++;
        }
    }

    waitForVBlank();
    drawRect(0, 0, MAX_X - 1, MAX_Y - 1, BLACK);
}