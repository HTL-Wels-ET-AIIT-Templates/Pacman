//--------------------------------------------------------------
// File     : main.c
// Datum    : 22.11.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F429
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Game : Pac-Man
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
// In Configuration diese Define hinzuf�gen :
// "STM32F429_439xx" , "__ASSEMBLY__" , "USE_STDPERIPH_DRIVER"
//--------------------------------------------------------------
/* Der Plan: Aufgaben von Schülern:
 *   Steuerung programmieren: Touch auswerten und Wert zurückliefern
 *   Punkte zählen, Array für Werte je Level, Durchschnittspunkte berechnen var: Player.score
 *     Möglicherweise mit erkennen ob Punkt gesammelt wurde? (Vergleich von Array)
 *
 */

#include "main.h"
#include "pacman.h"
#include "gui.h"


int main(void)
{
	SystemInit(); // Quarz Einstellungen aktivieren

	pacman_start();

	while(1)
	{

	}
}

//--------------------------------------------------------------
// check 4 buttons on the touch
//--------------------------------------------------------------
uint32_t gui_check_touch(void)
{
	uint32_t event=GUI_JOY_NONE;
	uint32_t x, y;
	static uint32_t old_button=999;

	UB_Touch_Read();
	x=Touch_Data.xp;
	y=Touch_Data.yp;

	if(Touch_Data.status==TOUCH_PRESSED) {
		// ========================================================================
		// TODO: check touch coordinates and set variable event
		// to one of the values: GUI_JOY_UP GUI_JOY_LEFT GUI_JOY_DOWN GUI_JOY_RIGHT








		// ========================================================================
	}
	else {
		event=GUI_JOY_NONE;
	}

	if(old_button!=event) {
		old_button=event;
		GUI.refresh_buttons=GUI_REFRESH_VALUE;
	}

	return(event);
}
