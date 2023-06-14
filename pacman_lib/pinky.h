//--------------------------------------------------------------
// File     : pinky.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_PINKY_H
#define __STM32F4_UB_PINKY_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "bot.h"
#include "skin.h"
#include "gui.h"


//--------------------------------------------------------------
#define  PINKY_START_X     14
#define  PINKY_START_Y     14     

//--------------------------------------------------------------
#define  PINKY_HOME_X   14
#define  PINKY_HOME_Y   14

//--------------------------------------------------------------
#define  PINKY_FRIGHTENED_BUF  30  // slow down bot
#define  PINKY_DOT_CNT_MAX     5


//--------------------------------------------------------------
#define  PINKY_SCATTER_X   0 
#define  PINKY_SCATTER_Y   0





//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void pinky_init(uint32_t mode);
void pinky_move(void);


//--------------------------------------------------------------
#endif // __STM32F4_UB_PINKY_H
