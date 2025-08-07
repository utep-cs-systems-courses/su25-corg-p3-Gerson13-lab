#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 
#define LED BIT6		/* note that bit zero req'd for display */


#define BUTTON1 BIT0
#define BUTTON2 BIT1
#define BUTTON3 BIT2
#define BUTTON4 BIT3
#define BUTTONS (BUTTON1 | BUTTON2 | BUTTON3 | BUTTON4)
//STATES
typedef enum{MAN, STREET, HOUSE, UTEP} State;
volatile State current_state = UTEP;


void button_init(){

  P2REN |= BUTTONS;
  P2OUT |= BUTTONS;
  P2DIR &= ~BUTTONS;
  P2IE |= BUTTONS;
  P2IES |= BUTTONS;
  P2IFG &= ~BUTTONS;
  P2IES |= BUTTONS;
}


void leg_1(){
  int row_mid = 110;
  int col_mid = screenWidth / 2;

  int n = 0;

  while(n !=50){
    drawPixel(col_mid, row_mid, COLOR_BLACK);
    row_mid ++;
    col_mid --;
    n++;
  }
}
 
void leg_2(){
  int row_mid = 110;
  int col_mid = screenWidth / 2;

  int n = 0;

  while(n !=50){
    drawPixel(col_mid, row_mid, COLOR_BLACK);
    row_mid ++;
    col_mid ++;
    n++;
  }
}


void torso(){
  int row_mid = 110;
  int col_mid = screenWidth / 2;

  int n = 0;

  while(n !=65){
    drawPixel(col_mid, row_mid, COLOR_BLACK);
    row_mid --;
    n++;
  }
}
void arms(){
  int row = screenHeight / 2;
  int col = 30;
  int n = 0;

  while(n != 70){
    drawPixel(col, row, COLOR_BLACK);
    col++;
    n++;
  }
}

void head_1(int col, int row){ //does left side of head
  int n = 0;
  while(n!=40){
    drawPixel(col,row, COLOR_BLACK);
    row++;
    n++;
  }
}
void head_2(int col, int row){ //does right side of head
 int end_col = screenWidth - col;
 int n=0;
  while(col < end_col){
    drawPixel(col,row, COLOR_BLACK);
    col++;
    n++;
  }
}
void stickman(){
  clearScreen(COLOR_YELLOW);
  leg_1();
  leg_2();
  torso();
  arms();
  head_1(40,5);
  head_1(screenWidth-40,5);
  head_2(40,5);
  head_2(40,45);
}
void dashes(int row){ //control the dashes on the street drawing
  int dash_length = 30;
  int col = screenWidth / 2; //to get in the middle of screen
  int n=0;
  while(n!=30){
    drawPixel(col, row, COLOR_YELLOW);
    row++;
    n++;
  }
}

void street(){ //draws street w dashes
  clearScreen(COLOR_DARK_GREEN);
  int row=0;
  int col=40;
  int end_col=screenWidth - 40;
  int n=0;

  while(row <= screenHeight){
    while(col != end_col){
      drawPixel(col, row, COLOR_BLACK);
      col++;
    }
    row++;
    col=40;
  }
  dashes(0);
  dashes(50);
  dashes(100);
  dashes(150);
}
void left_roof(){ //roof for house
  clearScreen(COLOR_RED);

  int mid_col = screenWidth / 2;
  int row = 0;

  while(mid_col >= 0){
      drawPixel(mid_col, row, COLOR_BLACK);
      mid_col --;
      row++;
    }
}

void right_roof(){ //roof for house

  int mid_col = screenWidth / 2;
  int row = 0;

  while(mid_col<= screenWidth){
      drawPixel(mid_col, row, COLOR_BLACK);
      mid_col ++;
      row++;
    }
}
void wall(int row, int col){ //house walls
  while(row <= screenHeight){
    drawPixel(col, row, COLOR_BLACK);
    row++;
  }

}
void door(){
  wall(screenHeight-50, (screenWidth /2) -15);
  wall(screenHeight-50, (screenWidth /2) +15);

  int row = screenHeight -50;
  int start_col = (screenWidth /2)-15;
  int end_col = (screenWidth /2)+15;

  while(start_col <= end_col){
    drawPixel(start_col, row, COLOR_BLACK);
    start_col++;
  }
 

}
void window_top(int row, int col){ //part of window
  int col_start = col;
  int n=0;
  while(n<=15){
    drawPixel(col_start, row, COLOR_BLACK);
    col_start ++;
    n++;
  }
}

void window_side(int col){ //part of window
  int col_start = col;
  int row = 70;
  int n=0;
  while(n<=15){
    drawPixel(col_start, row, COLOR_BLACK);
    row ++;
    n++;
  }
}


void house(){
  left_roof();
  right_roof();
  wall(50,15);
  wall(50, screenWidth-15);
  door();
  window_top(70,(screenWidth/2)-30);
  window_top(70,(screenWidth/2)+15);
  window_top(85,(screenWidth/2)-30);
  window_top(85,(screenWidth/2)+15);
 
  window_side((screenWidth/2)-30);
  window_side((screenWidth/2)-15);
  window_side((screenWidth/2)+30);
  window_side((screenWidth/2)+15);  
}

void color_screen(int row, int max_row, unsigned short color){
  int col = 0;

  while(row != max_row){
    while(col <= screenWidth){
      drawPixel(col,row, color);
      col++;
    }
    row++;
    col = 0;
  }
}

void utep_colors(){ //makes screen utep colors
  color_screen(0, screenHeight/2, COLOR_ORANGE_RED);
  color_screen(screenHeight/2, screenHeight, COLOR_NAVY);
}

void main()
{
  WDTCTL = WDTPW | WDTHOLD;
  P1DIR |= LED;		
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  button_init();
  or_sr(0x18);
}
 void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & BUTTON1){
    current_state = MAN;
    clearScreen(COLOR_RED);
    stickman();
  }else if(P2IFG & BUTTON2){
    current_state = STREET;
    clearScreen(COLOR_RED);
    street();
  }else if (P2IFG & BUTTON3){
    current_state = HOUSE;
    house();
  }else if(P2IFG & BUTTON4){
    current_state = UTEP;
    utep_colors();
  }
  P2IFG &= ~BUTTONS;
  P2IES |= BUTTONS;
 }
