#ifndef DISPLAY_H
#define DISPLAY_H

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

bool displayInit();

void displayClear();

void displayPrint(const char *text, int x, int y, int size);

void displayShow();

#endif