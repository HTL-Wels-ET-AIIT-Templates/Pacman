//--------------------------------------------------------------
// File     : gui.c
// Datum    : 22.11.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F429
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : keine
// Funktion : GUI Functions
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "gui.h"
#include <stdio.h>

Touch_Data_t Touch_Data;
GUI_t GUI;

//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void gui_draw_player(void);
void gui_clear_player(void);
void gui_draw_blinky(void);
void gui_clear_blinky(void);
void gui_draw_pinky(void);
void gui_clear_pinky(void);
void gui_clear_inky(void);
void gui_draw_inky(void);
void gui_clear_clyde(void);
void gui_draw_clyde(void);


//--------------------------------------------------------------
// clear screen
//--------------------------------------------------------------
void gui_clear_screen(void)
{
  UB_LCD_SetLayer_2();
  UB_LCD_SetTransparency(255);
  UB_LCD_FillLayer(BACKGROUND_COL);
}



//--------------------------------------------------------------
// clear player and all ghosts
//--------------------------------------------------------------
void gui_clear_bots(void)
{
  gui_clear_player();
  gui_clear_blinky();
  gui_clear_pinky();
  gui_clear_inky();
  gui_clear_clyde();
}


//--------------------------------------------------------------
// draw player and all ghosts
//--------------------------------------------------------------
void gui_draw_bots(void)
{
  gui_draw_player();
  gui_draw_blinky();
  gui_draw_pinky();
  gui_draw_inky();
  gui_draw_clyde();
}



//--------------------------------------------------------------
// draw maze for debug
// with standard skins for wall and path
//--------------------------------------------------------------
void gui_draw_debugmaze(void)
{
  uint32_t x,y;
  Image2LCD_t koord;

  gui_clear_screen();

  koord.w=ROOM_WIDTH;
  koord.h=ROOM_HEIGHT;

  for(y=0;y<ROOM_CNT_Y;y++) {
    for(x=0;x<ROOM_CNT_X;x++) {
      koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX;
      koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY;
      // check if wall or path
      if(Maze.Room[x][y].typ==ROOM_TYP_WALL) {
        koord.source_xp=Room_Skin[ROOM_SKIN_WALL_STD].xp;
        koord.source_yp=Room_Skin[ROOM_SKIN_WALL_STD].yp;
      }
      else {
        koord.source_xp=Room_Skin[ROOM_SKIN_PATH_STD].xp;
        koord.source_yp=Room_Skin[ROOM_SKIN_PATH_STD].yp;
      }
      koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX;
      koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY;
      // draw room
      UB_Graphic2D_DrawImageRect(koord);
    }
  }

  UB_Font_DrawString(10,290,"Debug view",&Arial_7x10,FONT_COL,BACKGROUND_COL);
  // endlessloop
  while(1);

}

//--------------------------------------------------------------
// draw maze in gameplay mode
//--------------------------------------------------------------
void gui_draw_maze(void)
{
  uint32_t x,y,s;
  Image2LCD_t koord;

  gui_clear_screen();

  koord.w=ROOM_WIDTH;
  koord.h=ROOM_HEIGHT;

  for(y=0;y<ROOM_CNT_Y;y++) {
    for(x=0;x<ROOM_CNT_X;x++) {
      koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX;
      koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY;
      s=Maze.Room[x][y].skin;
      koord.source_xp=Room_Skin[s].xp;
      koord.source_yp=Room_Skin[s].yp;
      UB_Graphic2D_DrawImageRect(koord);
    }
  }
}


//--------------------------------------------------------------
// draw maze in error mode
// color all rooms with error in red
//--------------------------------------------------------------
void gui_draw_errmaze(void)
{
  uint32_t x,y;
  uint32_t xp,yp;

  gui_clear_screen();

  for(y=0;y<ROOM_CNT_Y;y++) {
    for(x=0;x<ROOM_CNT_X;x++) {
      xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX;
      yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY;
      if(Maze.Room[x][y].deb_err==ROOM_DEB_OK) {
        if(Maze.Room[x][y].typ==ROOM_TYP_WALL) {       
          UB_Graphic2D_DrawFullRectDMA(xp,yp,ROOM_WIDTH,ROOM_HEIGHT,WALL_COL);
        }
        else {
          UB_Graphic2D_DrawFullRectDMA(xp,yp,ROOM_WIDTH,ROOM_HEIGHT,ROOM_COL);
        }
      }
      else {
        UB_Graphic2D_DrawFullRectDMA(xp,yp,ROOM_WIDTH,ROOM_HEIGHT,ERR_COL);
      }
    }
  }
}


//--------------------------------------------------------------
// draw player
//--------------------------------------------------------------
void gui_draw_player(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;

  x=Player.xp;
  y=Player.yp;

  if(Player.port!=PORT_DONE) {
    // dont draw while porting
    // its easier not to draw than calc the port animation
  }
  else {
    koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX+Player.delta_x+BOTS_DIFF_X;
    koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY+Player.delta_y+BOTS_DIFF_Y;
    if(koord.dest_xp < GUI_MAZE_STARTX) koord.dest_xp=GUI_MAZE_STARTX;
    if(koord.dest_yp < GUI_MAZE_STARTY) koord.dest_yp=GUI_MAZE_STARTY;
    koord.w=BOTS_WIDTH;
    koord.h=BOTS_HEIGHT;
    s=Player.skin;
    koord.source_xp=Player_Skin[s].xp;
    koord.source_yp=Player_Skin[s].yp;
    UB_Graphic2D_DrawImageRect(koord);
  }
}

//--------------------------------------------------------------
// clear player (repaint all rooms around the player pos)
//--------------------------------------------------------------
void gui_clear_player(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;
  uint32_t xp,yp;
  int16_t xmin,xmax; // signed int !!
  int16_t ymin,ymax; // signed int !!

  xp=Player.xp;
  yp=Player.yp;

  xmin=xp-1;
  xmax=xp+1;
  ymin=yp-1;
  ymax=yp+1;

  if(Player.move==MOVE_UP) ymax=yp+2;
  if(Player.move==MOVE_RIGHT) xmin=xp-2;
  if(Player.move==MOVE_DOWN) ymin=yp-2;
  if(Player.move==MOVE_LEFT) xmax=xp+2;

  if(xmin<0) xmin=0;
  if(xmax>=ROOM_CNT_X) xmax=ROOM_CNT_X-1;
  if(ymin<0) ymin=0;
  if(ymax>=ROOM_CNT_Y) ymax=ROOM_CNT_Y-1;

  koord.w=ROOM_WIDTH;
  koord.h=ROOM_HEIGHT;

  for(y=ymin;y<=ymax;y++) {
    for(x=xmin;x<=xmax;x++) {
      koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX;
      koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY;
      s=Maze.Room[x][y].skin;
      koord.source_xp=Room_Skin[s].xp;
      koord.source_yp=Room_Skin[s].yp;
      UB_Graphic2D_DrawImageRect(koord);
    }
  }
}

//--------------------------------------------------------------
// draw bot : Blinky
//--------------------------------------------------------------
void gui_draw_blinky(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;

  x=Blinky.xp;
  y=Blinky.yp;

  if(Blinky.port!=PORT_DONE) {
    // dont draw while porting
    // its easier not to draw than calc the port animation
  }
  else {
    koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX+Blinky.delta_x+BOTS_DIFF_X;
    koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY+Blinky.delta_y+BOTS_DIFF_Y;
    if(koord.dest_xp < GUI_MAZE_STARTX) koord.dest_xp=GUI_MAZE_STARTX;
    if(koord.dest_yp < GUI_MAZE_STARTY) koord.dest_yp=GUI_MAZE_STARTY;
    koord.w=BOTS_WIDTH;
    koord.h=BOTS_HEIGHT;
    s=Blinky.skin;
    koord.source_xp=Blinky_Skin[s].xp;
    koord.source_yp=Blinky_Skin[s].yp;
    UB_Graphic2D_DrawImageRect(koord);
  }
}

//--------------------------------------------------------------
// clear bot : Blinky (repaint all rooms around the bot pos)
//--------------------------------------------------------------
void gui_clear_blinky(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;
  uint32_t xp,yp;
  int16_t xmin,xmax; // signed int !!
  int16_t ymin,ymax; // signed int !!

  xp=Blinky.xp;
  yp=Blinky.yp;
 
  xmin=xp-1;
  xmax=xp+1;
  ymin=yp-1;
  ymax=yp+1;
  
  if(xmin<0) xmin=0;
  if(xmax>=ROOM_CNT_X) xmax=ROOM_CNT_X-1;  
  if(ymin<0) ymin=0;  
  if(ymax>=ROOM_CNT_Y) ymax=ROOM_CNT_Y-1;

  koord.w=ROOM_WIDTH;
  koord.h=ROOM_HEIGHT;

  for(y=ymin;y<=ymax;y++) {
    for(x=xmin;x<=xmax;x++) {
      koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX;
      koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY;
      s=Maze.Room[x][y].skin;
      koord.source_xp=Room_Skin[s].xp;
      koord.source_yp=Room_Skin[s].yp;
      UB_Graphic2D_DrawImageRect(koord);
    }
  }
}


//--------------------------------------------------------------
// draw bot : Pinky
//--------------------------------------------------------------
void gui_draw_pinky(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;

  x=Pinky.xp;
  y=Pinky.yp;

  if(Pinky.port!=PORT_DONE) {
    // dont draw while porting
    // its easier not to draw than calc the port animation
  }
  else {
    koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX+Pinky.delta_x+BOTS_DIFF_X;
    koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY+Pinky.delta_y+BOTS_DIFF_Y;
    if(koord.dest_xp < GUI_MAZE_STARTX) koord.dest_xp=GUI_MAZE_STARTX;
    if(koord.dest_yp < GUI_MAZE_STARTY) koord.dest_yp=GUI_MAZE_STARTY;
    koord.w=BOTS_WIDTH;
    koord.h=BOTS_HEIGHT;
    s=Pinky.skin;
    koord.source_xp=Pinky_Skin[s].xp;
    koord.source_yp=Pinky_Skin[s].yp;
    UB_Graphic2D_DrawImageRect(koord);
  }
}


//--------------------------------------------------------------
// clear bot : Pinky (repaint all rooms around the bot pos)
//--------------------------------------------------------------
void gui_clear_pinky(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;
  uint32_t xp,yp;
  int16_t xmin,xmax; // signed int !!
  int16_t ymin,ymax; // signed int !!

  xp=Pinky.xp;
  yp=Pinky.yp;
 
  xmin=xp-1;
  xmax=xp+1;
  ymin=yp-1;
  ymax=yp+1;
  
  if(xmin<0) xmin=0;
  if(xmax>=ROOM_CNT_X) xmax=ROOM_CNT_X-1;  
  if(ymin<0) ymin=0;  
  if(ymax>=ROOM_CNT_Y) ymax=ROOM_CNT_Y-1;

  koord.w=ROOM_WIDTH;
  koord.h=ROOM_HEIGHT;

  for(y=ymin;y<=ymax;y++) {
    for(x=xmin;x<=xmax;x++) {
      koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX;
      koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY;
      s=Maze.Room[x][y].skin;
      koord.source_xp=Room_Skin[s].xp;
      koord.source_yp=Room_Skin[s].yp;
      UB_Graphic2D_DrawImageRect(koord);
    }
  }
}


//--------------------------------------------------------------
// draw bot : Inky
//--------------------------------------------------------------
void gui_draw_inky(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;

  x=Inky.xp;
  y=Inky.yp;

  if(Inky.port!=PORT_DONE) {
    // dont draw while porting
    // its easier not to draw than calc the port animation
  }
  else {
    koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX+Inky.delta_x+BOTS_DIFF_X;
    koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY+Inky.delta_y+BOTS_DIFF_Y;
    if(koord.dest_xp < GUI_MAZE_STARTX) koord.dest_xp=GUI_MAZE_STARTX;
    if(koord.dest_yp < GUI_MAZE_STARTY) koord.dest_yp=GUI_MAZE_STARTY;
    koord.w=BOTS_WIDTH;
    koord.h=BOTS_HEIGHT;
    s=Inky.skin;
    koord.source_xp=Inky_Skin[s].xp;
    koord.source_yp=Inky_Skin[s].yp;
    UB_Graphic2D_DrawImageRect(koord);
  }
}


//--------------------------------------------------------------
// clear bot : Inky (repaint all rooms around the bot pos)
//--------------------------------------------------------------
void gui_clear_inky(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;
  uint32_t xp,yp;
  int16_t xmin,xmax;  // signed int !!
  int16_t ymin,ymax;  // signed int !!

  xp=Inky.xp;
  yp=Inky.yp;
 
  xmin=xp-1;
  xmax=xp+1;
  ymin=yp-1;
  ymax=yp+1;
  
  if(xmin<0) xmin=0;
  if(xmax>=ROOM_CNT_X) xmax=ROOM_CNT_X-1;  
  if(ymin<0) ymin=0;  
  if(ymax>=ROOM_CNT_Y) ymax=ROOM_CNT_Y-1;

  koord.w=ROOM_WIDTH;
  koord.h=ROOM_HEIGHT;

  for(y=ymin;y<=ymax;y++) {
    for(x=xmin;x<=xmax;x++) {
      koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX;
      koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY;
      s=Maze.Room[x][y].skin;
      koord.source_xp=Room_Skin[s].xp;
      koord.source_yp=Room_Skin[s].yp;
      UB_Graphic2D_DrawImageRect(koord);
    }
  }
}

//--------------------------------------------------------------
// draw bot : Clyde
//--------------------------------------------------------------
void gui_draw_clyde(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;

  x=Clyde.xp;
  y=Clyde.yp;

  if(Clyde.port!=PORT_DONE) {
    // dont draw while porting
    // its easier not to draw than calc the port animation
  }
  else {
    koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX+Clyde.delta_x+BOTS_DIFF_X;
    koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY+Clyde.delta_y+BOTS_DIFF_Y;
    if(koord.dest_xp < GUI_MAZE_STARTX) koord.dest_xp=GUI_MAZE_STARTX;
    if(koord.dest_yp < GUI_MAZE_STARTY) koord.dest_yp=GUI_MAZE_STARTY;
    koord.w=BOTS_WIDTH;
    koord.h=BOTS_HEIGHT;
    s=Clyde.skin;
    koord.source_xp=Clyde_Skin[s].xp;
    koord.source_yp=Clyde_Skin[s].yp;
    UB_Graphic2D_DrawImageRect(koord);
  }
}


//--------------------------------------------------------------
// clear bot : Clyde (repaint all rooms around the bot pos)
//--------------------------------------------------------------
void gui_clear_clyde(void)
{
  Image2LCD_t koord;
  uint32_t x,y,s;
  uint32_t xp,yp;
  int16_t xmin,xmax; // signed int !!
  int16_t ymin,ymax; // signed int !!

  xp=Clyde.xp;
  yp=Clyde.yp;
 
  xmin=xp-1;
  xmax=xp+1;
  ymin=yp-1;
  ymax=yp+1;
  
  if(xmin<0) xmin=0;
  if(xmax>=ROOM_CNT_X) xmax=ROOM_CNT_X-1;  
  if(ymin<0) ymin=0;  
  if(ymax>=ROOM_CNT_Y) ymax=ROOM_CNT_Y-1;

  koord.w=ROOM_WIDTH;
  koord.h=ROOM_HEIGHT;

  for(y=ymin;y<=ymax;y++) {
    for(x=xmin;x<=xmax;x++) {
      koord.dest_xp=(x*ROOM_WIDTH)+GUI_MAZE_STARTX;
      koord.dest_yp=(y*ROOM_HEIGHT)+GUI_MAZE_STARTY;
      s=Maze.Room[x][y].skin;
      koord.source_xp=Room_Skin[s].xp;
      koord.source_yp=Room_Skin[s].yp;
      UB_Graphic2D_DrawImageRect(koord);
    }
  }
}

//--------------------------------------------------------------
// draw gui (buttons, text ...)
//--------------------------------------------------------------
void gui_draw_gui(uint32_t joy)
{
  char buf[20];

  if(GUI.refresh_value>0) {
    GUI.refresh_value--;

    sprintf(buf,"level : %d",(int)(Player.level));
    UB_Font_DrawString(10,260,buf,&Arial_7x10,FONT_COL3,BACKGROUND_COL);

    sprintf(buf,"score : %d",(int)(Player.score));
    UB_Font_DrawString(10,275,buf,&Arial_7x10,FONT_COL3,BACKGROUND_COL);

    sprintf(buf,"lives : %d",(int)(Player.lives));
    UB_Font_DrawString(10,290,buf,&Arial_7x10,FONT_COL3,BACKGROUND_COL);

    if(Player.status==PLAYER_STATUS_WIN) {
      UB_Font_DrawString(10,305,"level complete",&Arial_7x10,FONT_COL2,BACKGROUND_COL);
    }
    else if(Player.status==PLAYER_STATUS_DEAD) {
   	  if(Player.lives>=2) {
        UB_Font_DrawString(10,305,"player lose",&Arial_7x10,FONT_COL2,BACKGROUND_COL);
   	  }
   	  else {
        UB_Font_DrawString(10,305,"GAME OVER",&Arial_7x10,FONT_COL2,BACKGROUND_COL);
   	  }
    }
  }


  if(GUI.refresh_buttons>0) {
    GUI.refresh_buttons--;
    gui_draw_buttons(joy);
  }
}

//--------------------------------------------------------------
// draw all buttons
//--------------------------------------------------------------
void gui_draw_buttons(uint32_t joy)
{
  Image2LCD_t koord;
  uint32_t su,sd,sr,sl;

  su=BUTTON_SKIN1;
  sd=BUTTON_SKIN1;
  sr=BUTTON_SKIN1;
  sl=BUTTON_SKIN1;

  if(joy==GUI_JOY_UP) su=BUTTON_SKIN2;
  if(joy==GUI_JOY_LEFT) sl=BUTTON_SKIN2;
  if(joy==GUI_JOY_DOWN) sd=BUTTON_SKIN2; 
  if(joy==GUI_JOY_RIGHT) sr=BUTTON_SKIN2;

  // Button up
  koord.dest_xp=GUI_BTN_UP_X;
  koord.dest_yp=GUI_BTN_UP_Y;
  koord.w=BUTTON_WIDTH;
  koord.h=BUTTON_HEIGHT;
  koord.source_xp=Button_Skin[su].xp;
  koord.source_yp=Button_Skin[su].yp;
  UB_Graphic2D_DrawImageRectRotate(koord,3);

  // Button down
  koord.dest_xp=GUI_BTN_DOWN_X;
  koord.dest_yp=GUI_BTN_DOWN_Y;
  koord.w=BUTTON_WIDTH;
  koord.h=BUTTON_HEIGHT;
  koord.source_xp=Button_Skin[sd].xp;
  koord.source_yp=Button_Skin[sd].yp;
  UB_Graphic2D_DrawImageRectRotate(koord,2);

  // Button right
  koord.dest_xp=GUI_BTN_RIGHT_X;
  koord.dest_yp=GUI_BTN_RIGHT_Y;
  koord.w=BUTTON_WIDTH;
  koord.h=BUTTON_HEIGHT;
  koord.source_xp=Button_Skin[sr].xp;
  koord.source_yp=Button_Skin[sr].yp;
  UB_Graphic2D_DrawImageRectRotate(koord,0);

  // Button left
  koord.dest_xp=GUI_BTN_LEFT_X;
  koord.dest_yp=GUI_BTN_LEFT_Y;
  koord.w=BUTTON_WIDTH;
  koord.h=BUTTON_HEIGHT;
  koord.source_xp=Button_Skin[sl].xp;
  koord.source_yp=Button_Skin[sl].yp;
  UB_Graphic2D_DrawImageRectRotate(koord,1);
}


//--------------------------------------------------------------
// check 4 external buttons
//   if button aktiv = GND -> "BTN_RELEASED"
//   if button aktiv = VCC -> "BTN_PRESSED"
//--------------------------------------------------------------
uint32_t gui_check_button(void)
{
  uint32_t ret_wert=GUI_JOY_NONE;
  static uint32_t old_button=999;

  if(UB_Button_Read(BTN_UP)==BTN_RELEASED) {
    ret_wert=GUI_JOY_UP;
  }
  if(UB_Button_Read(BTN_RIGHT)==BTN_RELEASED) {
    ret_wert=GUI_JOY_RIGHT;
  }
  if(UB_Button_Read(BTN_DOWN)==BTN_RELEASED) {
    ret_wert=GUI_JOY_DOWN;
  }
  if(UB_Button_Read(BTN_LEFT)==BTN_RELEASED) {
    ret_wert=GUI_JOY_LEFT;
  }

  if(old_button!=ret_wert) {
    old_button=ret_wert;
    GUI.refresh_buttons=GUI_REFRESH_VALUE;
  }

  return(ret_wert);
}


//--------------------------------------------------------------
// debug per UART
//--------------------------------------------------------------
void gui_debug_uart(char *ptr)
{
  #if USE_GUI_UART_DEBUG==1
    UB_Uart_SendString(COM1,ptr,CRLF);  // for DEBUG
  #endif
}
