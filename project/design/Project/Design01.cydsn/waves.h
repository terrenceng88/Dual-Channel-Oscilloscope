/* ========================================


Notes: Contains functins for dealing with waves

Lab Project
 * ========================================
*/


#include "stdlib.h"
#include "stdio.h"
#include "math.h"


/*******************************************************************************
Perform Y scaling and fill Y scaled buffer
unscaled = unscaled buffer
scaled = scaled buffer
Sy = y scaling
*******************************************************************************/
void Y_scaling(uint16_t unscaled[], uint16_t scaled[], int Sy);


/*******************************************************************************
Perform X scaling and returnvalue to skip indexes in buffer by
unscaled = unscaled buffer
scaled = scaled buffer
Sx = x scaling
*******************************************************************************/
int X_scaling(int Sx);


/*******************************************************************************
Calculate and return offset from potentiometer
raw = raw ADC value
converted = raw ADC value converted to PWM range
*******************************************************************************/
int PWM_conversion(uint16_t raw);


/*******************************************************************************
Calculate and return RAW ADCa value to start waveform given trigger level
trigger level = User inputed trigger level
*******************************************************************************/
int Trigger_Conversion(uint16_t trigger_level); 


/*******************************************************************************
Calculates and returns Frequency 
draw_buf = draw buffer
X_scale = X scale value
*******************************************************************************/
int calc_freq(uint16_t draw_buf[], int X_scale); 


/*******************************************************************************
Fills a buffer with coordinates for stopped waveform at a trigger level
untriggered[] = Y scaled draw buffer
triggered[] = new draw buffer with coordinates to fit triggered level
level = trigger level
Sy = Y scaling
*******************************************************************************/
void trig_buf_fill(uint16_t untriggered[], uint16_t triggered[], int level, int Sy);


/*******************************************************************************
Draws wave and erases old one
x1 = start x coordinate
x2 = end x coordinate
curr[] = draw buffer
old[] = old buffer
pot = potentiometer offset
color = color of the line
*******************************************************************************/
void draw_wave_line(int x, uint16_t curr[], uint16_t old[], int pot, int color);









