#ifndef DISPLAY_H
#define DISPLAY_H
#include "globals.h"
extern String relay_state[4];
extern void initDisplay();
void drawRelayState(int i);
void drawRelayFrame(int i);
#endif