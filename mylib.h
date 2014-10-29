#ifndef MYLIB_H_
#define MYLIB_H_

typedef unsigned short u16; //shortcut for 16bit unsigned

// *** Display Stuff ===================================================

#define REG_DISPCTL (* (u16*) (0x4000000))
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define VID_BFFR ((u16*) (0x6000000))
extern u16* videoBuffer;
#define MAX_X (240)
#define MAX_Y (160)

// *** Colors =========================================================

#define RGB(r,g,b) ((r)|((g)<<5)|((b)<<10))
#define WHITE RGB(31,31,31)
#define RED RGB(31,0,0)
#define GREEN RGB(0,31,0)
#define BLUE RGB(0,0,31)
#define BLACK RGB(0,0,0)
#define PURPLE RGB(12, 0, 25)
#define ORANGE RGB(31, 12, 0)
//rgb for beige 245, 241, 222
#define BEIGE RGB(23, 21, 15)
#define GRAY RGB(23, 23, 23)
#define FOOD_COLOR RGB(31, 1, 1)

// *** Timer stuff ====================================================

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006
#define TM_ON (1<<7)
#define TM_FREQ_1024  3
#define REG_TM0CNT *(volatile u16*)0x4000102
#define REG_TM0D *(volatile u16*)0x4000100
#define TM_IRQ (1<<6)

// *** Input =========================================================

// Buttons

#define BUTTON_A	(1<<0)
#define BUTTON_B	(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT	(1<<5)
#define BUTTON_UP	(1<<6)
#define BUTTON_DOWN	(1<<7)
#define BUTTON_R	(1<<8)
#define BUTTON_L	(1<<9)

#define BUTTONS *(volatile unsigned int *)0x4000130

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

// *** Other =========================================================
extern int num2lengthen;
#define OFFSET(x, y, width) (y * width + x)
#define SIGN(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0)) 


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



// *** Prototypes =========================================================
void initGame(snake*, u16);
void initSnake(snake*, u16, int);
void printSnakeNode(node*);
void setPixel(int, int, u16);
u16 getPixelColor(int, int);
void drawRect(int, int, int, int, u16);
void drawHollowRect(int, int, int, int, u16);
void plotLine(int, int, int, int, u16);
void moveSnake(snake*, u16);
int hasEatenFood(node*);
int isCollided(node*, u16);
void updateSnakeDirection(snake*, direction);
void placeFood(int, u16);
void updateScore(int);
char* int2str(int);

#endif