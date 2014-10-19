#ifndef TEXT_H_
#define TEXT_H_

//array that holds all of the character maps
extern const unsigned char fontdata_6x8[12288];
typedef unsigned char u8;

void drawChar(int row, int col, u8 ch, unsigned short color);
void drawString(int row, int col, char* str, unsigned short color);

#endif
