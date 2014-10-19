
#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"




void initSnake(snake* mysnake, u16 color) {
    node* head = malloc(sizeof(*head));;
    node* prevNode = head;
    head->x = 240 / 2;
    head->y = 160 / 2;
    head->color = color;
    printSnakeNode(head);
    for (int i = 1; i < 4; i++) {
        node* newNode = malloc(sizeof(*newNode));
        prevNode->next = newNode;
        newNode->previous = prevNode;
        newNode->x = head->x - i;
        newNode->y = head->y;
        newNode->color = color;
        prevNode = newNode;
        printSnakeNode(prevNode);
    }
    mysnake->head = head;
    mysnake->tail = prevNode;
    mysnake->color = color;
    mysnake->direction = right;
}

void printSnakeNode(node* node) {
    setPixel(node->x, node->y, node->color);
}

//sets a pixel
void setPixel(int x, int y, u16 color) {
    int offset = (y * 240) + x;
    REG_DISPCTL	= MODE3 | BG2_ENABLE;
    *((u16*) (VID_BFFR + offset)) = color;
    //*VID_BFFR = color;
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

void moveSnake(snake* mysnake, u16 bgcolor) {
    node* nodeMoving = mysnake->tail;
    mysnake->tail = mysnake->tail->previous;
    mysnake->tail->next = NULL;
    //take care of turning off the tail
    nodeMoving->color = bgcolor;
    printSnakeNode(nodeMoving);
    nodeMoving->color = mysnake->color;

    if (mysnake->direction == up) {
        nodeMoving->y = mysnake->head->y + 1;
    } else if (mysnake->direction == down) {
        nodeMoving->y = mysnake->head->y - 1;
    } else if (mysnake->direction == left) {
        nodeMoving->x = mysnake->head->x - 1;
    } else {
        nodeMoving->x = mysnake->head->x + 1;
    }
    printSnakeNode(nodeMoving);

    mysnake->head->previous = nodeMoving;
    nodeMoving->next = mysnake->head;
    mysnake->head = nodeMoving;
}



/*int main() {
    printf("hello world");
    return 0;
}*/
