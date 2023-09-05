/* ========================================
Terrence Ng
CSE 121/L
Section: B

Notes: Contains functins for dealing with waves

Lab Project
 * ========================================
*/


#include "project.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "waves.h"
#include "TinyScope.h"
#include "GUI.h"


/*******************************************************************************
Perform Y scaling and fill Y scaled buffer
unscaled = unscaled buffer
scaled = scaled buffer
Sy = y scaling
*******************************************************************************/

void Y_scaling(uint16_t unscaled[], uint16_t scaled[], int Sy) //Y has 8 divisions
{
    for(int i = 0; i < 512; i++) {
        int Y = unscaled[i]; //ADC Result
        scaled[i] =  (30 * 3300 * Y) / (2048 * Sy) ; //Scaled Y = [(240/8) / (Sy)] * 3300 * (ADC_Result/4096)
    }
}


/*******************************************************************************
Perform X scaling and returnvalue to skip indexes in buffer by
unscaled = unscaled buffer
scaled = scaled buffer
Sx = x scaling
*******************************************************************************/
int X_scaling(int Sx) //X has 10 divisions
{
    int n = 1;
    int Px = 32; //Px == 320/10 = 32 pixels per division
    float pixels_per_ms = (float)Px/ (float)Sx; //pixels per milisecond
    float Ps = pixels_per_ms * 4; //pixels per sample point'
    if(Ps < 1) {
        n = (float) 1/ (float)Ps; //plot every n th sample at x = 0, 1, 2, ...
    } else if (Ps > 1) {
        n = Ps;
    }else {
        n = 1;
    }
    return n;
}


/*******************************************************************************
Calculate and return offset from potentiometer
raw = raw ADC value
converted = raw ADC value converted to PWM range
*******************************************************************************/
int PWM_conversion(uint16_t raw) {
    int compare = raw;
    compare = compare & 0x7ff; //(0x7ff = 2047) = ADC range
    compare = (compare * 240)/2047; //convert to PWM range
    return compare;
}


/*******************************************************************************
Calculate and return RAW ADC value to start waveform given trigger level
trigger level = User inputed trigger level
*******************************************************************************/
int Trigger_Conversion(uint16_t trigger_level) {
    int offset = (240 * (3300 - trigger_level)) / 3300; //accounts for wave being upside down since  (0,0) is in top left corner
    return offset;
}


/*******************************************************************************
Calculates and returns Frequency 
draw_buf = draw buffer
X_scale = X scale value
*******************************************************************************/
int calc_freq(uint16_t draw_buf[], int X_scale) {
    bool greater = true; //if a > b
    int a = draw_buf[0]; //starting point of threshold
    int b = draw_buf[3];//ending point of threshold
    int start = 0; //starting point of period
    int end = 10000; //end point of period
    int period = 0; 
    
    if(a <= b) { //checks slope for 2 points
        greater = false;
    }
    
    for(int i = 10; i < 512 - 4; i++) {
        if(greater) { //positive slope threshold
            if(b < draw_buf[i] && draw_buf[i] < a) { //if inside threshold
                if(draw_buf[i] > draw_buf[i + 4]) { //if positive slope
                    end = i;
                }
            }
        } else { //negative slope threshold
            if(b >= draw_buf[i] && draw_buf[i] >= a) { //if inside threshold
                if(draw_buf[i] > draw_buf[i + 4]) { //if negative slope
                    end = i;
                }
            }
        }
    }
    period = (end - start) * X_scaling(X_scale);
    return 1000000/period; //return frequency (1000000 = 250000 * 4)
}



/*******************************************************************************
Fills a buffer with coordinates for stopped waveform at a trigger level
untriggered[] = Y scaled draw buffer
triggered[] = new draw buffer with coordinates to fit triggered level
level = trigger level
Sy = Y scaling
*******************************************************************************/
void trig_buf_fill(uint16_t untriggered[], uint16_t triggered[], int level, int Sy) {
    int start = 0; //index to start iterating through to get triggered buffer
    int adjusted = (level * 4096) / 3300; //convert to Raw ADC value
    int ylev = (30 * 3300 * adjusted) / (2048 * Sy); //Y scale
    for(int i = 0; i < 512; i++) {
        if( untriggered[i] == ylev - 5 ) { //if the Y coordinate is at trigger level (5 is just to make sure trigger level is reached beacuse of noise)
            start = i; //get index
        }
    }
    for(int i = 0; i < 320; i++) { //fill buffer with triggered coordinates
        if(start >= 512) { //just here for now to make sure it doesn't go out of bounds until I can fiugure out something better
            start = 360; //seems to draw most even waveform   
            //break;
        }
        triggered[i] = untriggered[start];
        start++;
    }
}


/*******************************************************************************
Draws wave and erases old one
x = start x coordinate
curr[] = current draw buffer
old[] = old buffer
pot = potentiometer offset
color = color of the line
*******************************************************************************/
void draw_wave_line(int x, uint16_t curr[], uint16_t old[], int pot, int color) {
    GUI_SetColor(GUI_DARKCYAN); //erases old waveform
    GUI_DrawLine(x, old[x], x + 1, old[x + 1]); //(x1, y1, x2, y2)
    GUI_SetColor(color); //draws new waveform
    GUI_DrawLine(x, curr[x] + pot, x + 1, curr[x + 1] + pot); //(x1, y1, x2, y2)
    
    old[x] = curr[x] + pot; //gets old waveform coordinates
}



