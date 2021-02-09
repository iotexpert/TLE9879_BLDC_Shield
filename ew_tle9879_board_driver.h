#pragma once

// #define EW_TLE9879_DEBUG_PRINTS

#ifdef EW_TLE9879_DEBUG_PRINTS
#include <stdio.h>
#define DEBUG_PRINTS(fmt, args...) printf(fmt, ##args)
#else
#define DEBUG_PRINTS(fmt, args...)
#endif

bool ew_tle9879_LEDOn(ew_tle9879_board_t* obj, uint8_t led);
bool ew_tle9879_LEDOff(ew_tle9879_board_t* obj, uint8_t led);
bool ew_tle9879_boardControl(ew_tle9879_board_t* obj);
// ew_tle9879_setMode(&tle9879_board2, FOC);
//             ew_tle9879_setMotorSpeed(&tle9879_board2, 500);
//             ew_tle9879_setMotorMode(&tle9879_board2, START_MOTOR);
bool ew_tle9879_motorControl(ew_tle9879_board_t* obj, uint8_t command);
bool ew_tle9879_setMotorspeed(ew_tle9879_board_t* obj, float motorspeed);
bool ew_tle9879_modeControl(ew_tle9879_board_t* obj, uint8_t requestedmode);
uint8_t ew_tle9879_readDataset(ew_tle9879_board_t* obj);
bool ew_tle9879_writeDataset(ew_tle9879_board_t* obj);
bool ew_tle9879_changeParameter(ew_tle9879_board_t*obj, uint8_t index, float data);
bool ew_tle9879_LEDOn(ew_tle9879_board_t* obj, uint8_t led);
bool ew_tle9879_LEDOff(ew_tle9879_board_t* obj, uint8_t led);
