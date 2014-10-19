#ifndef MYLIB_H_
#define MYLIB_H_

#define REG_DISPCTL (* (u16*) (0x4000000))
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define VID_BFFR ((u16*) (0x6000000))

#define RGB(r,g,b) ((r)|((g)<<5)|((b)<<10))
#define WHITE RGB(31,31,31)
#define RED RGB(31,0,0)
#define GREEN RGB(0,31,0)
#define BLUE RGB(0,0,31)
#define BLACK RGB(0,0,0)
//rgb for beige 245, 241, 222
#define BEIGE RGB(23, 21, 15)

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006
#define TM_ON (1<<7)
#define TM_FREQ_1024  3
#define REG_TM0CNT *(volatile u16*)0x4000102
#define REG_TM0D *(volatile u16*)0x4000100
#define TM_IRQ (1<<6)


typedef unsigned short u16; //shortcut for 16bit unsigned


/* Direction Enum */
typedef enum {
    up,
    down,
    left,
    right,
} direction;


/* Node structure that is a node of the snake body*/
typedef struct thisNode {
    int x;
    int y;
    int size; //note this is edge length of square
    u16 color;
    struct thisNode* previous;
    struct thisNode* next;
} node;

/* Snake structure that describes the snake
 * note that the snake is a queue */
typedef struct {
    node* head;
    node* tail;
    direction direction;
    int length;
    u16 color;
} snake;



void initSnake(snake*, u16, int);
void printSnakeNode(node*);
void setPixel(int, int, u16);
void drawRect(int, int, int, int, u16);
void drawHollowRect(int, int, int, int, u16);
void plotLine(int, int, int, int, u16);
void moveSnake(snake*, u16);

#endif