#pragma once

// #define TLE9879_DEBUG_PRINTS

#ifdef TLE9879_DEBUG_PRINTS
#include <stdio.h>
#define DEBUG_PRINTS(fmt, args...) printf(fmt, ##args)
#else
#define DEBUG_PRINTS(fmt, args...)
#endif

bool tle9879_LEDOn(tle9879_board_t* obj, uint8_t led);
bool tle9879_LEDOff(tle9879_board_t* obj, uint8_t led);
bool tle9879_boardControl(tle9879_board_t* obj);
// tle9879_setMode(&tle9879_board2, FOC);
//             tle9879_setMotorSpeed(&tle9879_board2, 500);
//             tle9879_setMotorMode(&tle9879_board2, START_MOTOR);
bool tle9879_motorControl(tle9879_board_t* obj, uint8_t command);
bool tle9879_setMotorspeed(tle9879_board_t* obj, float motorspeed);
bool tle9879_modeControl(tle9879_board_t* obj, uint8_t requestedmode);
uint8_t tle9879_readDataset(tle9879_board_t* obj);
bool tle9879_writeDataset(tle9879_board_t* obj);
bool tle9879_changeParameter(tle9879_board_t*obj, uint8_t index, float data);
bool tle9879_LEDOn(tle9879_board_t* obj, uint8_t led);
bool tle9879_LEDOff(tle9879_board_t* obj, uint8_t led);
