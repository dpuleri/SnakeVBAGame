
#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

u16* videoBuffer = (u16*) VID_BFFR; //define global var
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
}

void printSnakeNode(node* node) {
    //setPixel(node->x, node->y, node->color);
    drawRect(node->x, node->y, node->size, node->size, node->color);
}

//sets a pixel
void setPixel(int x, int y, u16 color) {
    int offset = (y * 240) + x;
    REG_DISPCTL	= MODE3 | BG2_ENABLE;
    *((u16*) (VID_BFFR + offset)) = color;
    //*VID_BFFR = color;
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
    int dx = x1 - x0;
    int dy = y1 - y0;

    int D = (dy << 1) -dx;
    setPixel(x0, y0, color);
    int y = y0;

    for (int x = x0 + 1; x <= x1; x++) {
        if (D > 0) {
            y = y + 1;
            setPixel(x, y, color);
            D += (dy << 1) - (dx << 1);
        } else {
            setPixel(x, y, color);
            D += (dy << 1);
        }
    }
}

//var to count if the snake is being lengthened
void moveSnake(snake* mysnake, u16 bgcolor) {
    static int hasCollided = 0;

    node* nodeMoving;

    if (hasCollided) {
        return;
    } else if (num2lengthen) {
        nodeMoving = malloc(sizeof(*nodeMoving));
        nodeMoving->size = mysnake->head->size;
        num2lengthen--;
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
    } else if (isCollided(nodeMoving, bgcolor)) {
        nodeMoving->color = GRAY;
        hasCollided = 1;
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



/*int main() {
    printf("hello world");
    return 0;
}*/
