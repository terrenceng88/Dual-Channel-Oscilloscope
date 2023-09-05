/* ========================================



Lab Project
 * ========================================
*/


#include "project.h"
#include "GUI.h"
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include <strings.h>
#include "math.h"
#include "TinyScope.h"
#include "waves.h"

#define XSIZE 320 // Width of LCD screen in pixels
#define YSIZE 240 // Height of LCF screen in pixels
#define XDIVISIONS 10 // Number of x-axis divisions for grid
#define YDIVISIONS 8 // Number of y-axis divisions for grid
#define XMARGIN 5 // Margin around screen on x-axis
#define YMARGIN 4 // Margin around screen on y-axis
#define MAXPOINTS XSIZE-2*XMARGIN // Maximum number of points in wave for plotting

#define PI 3.14159265
#define BUFFER_SIZE 512

/* Arrays to hold channel waveform data for drawing */
GUI_HMEM ch1_spline;
GUI_HMEM ch2_spline;

void init_uart_printf(void);
void uart_printf(char *print_string);
bool full = false; //if one of the ping pong buffers are full or not

char print_string[16]; //UART string to print
uint16_t chan_1_buf_1[BUFFER_SIZE]; //first channel first ping pong buffer
uint16_t chan_1_buf_2[BUFFER_SIZE]; //first channel second ping pong buffer
uint16_t buffer_big_1[2 * BUFFER_SIZE]; //X scaled values from both ping pong buffers for channel 1
uint16_t Y_scaled_1[BUFFER_SIZE]; //scaled Y coordinates for channel 1  (Draw buffer 1)
uint16_t old_wave_1[BUFFER_SIZE]; //old draw buffer for channel 1
uint16_t triggered_1[2 * BUFFER_SIZE]; //trigger level draw buffer for channel 1

uint16_t chan_2_buf_1[BUFFER_SIZE]; //second channel first ping pong buffer
uint16_t chan_2_buf_2[BUFFER_SIZE]; //second channel second ping pong buffer
uint16_t buffer_big_2[2 * BUFFER_SIZE]; //X scaled values from both ping pong buffers for channel 2
uint16_t Y_scaled_2[BUFFER_SIZE]; //scaled Y coordinates for channel 2  (Draw buffer 2)
uint16_t old_wave_2[BUFFER_SIZE]; //old draw buffer for channel 2
uint16_t triggered_2[2 * BUFFER_SIZE]; //trigger level draw buffer for channel 2

int big_buf_index_1 = 0; //current index in big buffer channel 1
int big_buf_index_2 = 0; //current index in big buffer channel 2


int X_scale = 500; //default x scale 1000 (1 milisecond per division)
int Y_scale = 2000; //default y scale 1000 (1 Volt per division)


int frequency_1 = 0; //frequency for channel 1 waveform
int frequency_2 = 0; //frequency for channel 2 waveform

int trigger_level_1 = 1000; //trigger level for channel 1
int trigger_level_2 = 2000; //trigger level for channel 2

bool trigger_1 = false; //if channel 1 was triggered or not
bool trigger_2 = false; //if channel 2 was triggered or not

int pingpong_1 = 0; //0 if 1st descriptor full, 1 if 2nd descriptor full (for channel 1)
int pingpong_2 = 0; //0 if 1st descriptor full, 1 if 2nd descriptor full (for channel 2)
bool running = true; //true if the waveform is running, false otherwise (for start and stop) (default stop, false)


void DMA_1_ISR(void) { //channel 1
    //uart_printf("test");
    NVIC_ClearPendingIRQ(DMA_1_INT_cfg.intrSrc); 
    Cy_DMA_Channel_ClearInterrupt(DMA_1_HW, DMA_1_DW_CHANNEL);
    if(Cy_DMA_Channel_GetStatus(DMA_1_HW, DMA_1_DW_CHANNEL) == CY_DMA_INTR_CAUSE_COMPLETION) {
        full = true; //one of the ping pong buffers is full  
    }
    
    if(pingpong_1 == 0) { //if desriptor 1 full (not current descriptor)      
        for(int i = 0; i < BUFFER_SIZE; i++) {
            if(big_buf_index_1 >= 1024) {
                break;
            }
            if( i % X_scaling(X_scale) == 0) { //X scaling
                buffer_big_1[big_buf_index_1] = chan_1_buf_1[i];
                big_buf_index_1++;
            }
        } 
        pingpong_1 = 1;
    }
    else if(pingpong_1 == 1) { //if descriptor 2 full (not current descriptor)
        for(int i = 0; i < BUFFER_SIZE; i++) {
            if(big_buf_index_1 >= 1024) {
                break;
            }
            if( i % X_scaling(X_scale) == 0) { //X scaling
                buffer_big_1[big_buf_index_1] = chan_1_buf_2[i];
                big_buf_index_1++;
            }
        }
        pingpong_1 = 0;
    }
}


void DMA_2_ISR(void) { //channel 2
    //uart_printf("test");
    NVIC_ClearPendingIRQ(DMA_2_INT_cfg.intrSrc); 
    Cy_DMA_Channel_ClearInterrupt(DMA_2_HW, DMA_2_DW_CHANNEL);
    if(Cy_DMA_Channel_GetStatus(DMA_2_HW, DMA_2_DW_CHANNEL) == CY_DMA_INTR_CAUSE_COMPLETION) {
        full = true; //one of the ping pong buffers is full  
    }
    
    if(pingpong_2 == 0) { //if desriptor 1 full (not current descriptor)      
        for(int i = 0; i < BUFFER_SIZE; i++) {
            if(big_buf_index_2 >= 1024) {
                        break;
                    }
                if( i % X_scaling(X_scale) == 0) { //X scaling
                    buffer_big_2[big_buf_index_2] = chan_2_buf_1[i];
                    big_buf_index_2++;
                }
            } 
        pingpong_2 = 1;
    }
    else if(pingpong_2 == 1) { //if descriptor 2 full (not current descriptor)
        for(int i = 0; i < BUFFER_SIZE; i++) {
            if(big_buf_index_2 >= 1024) {
                        break;
                    }
                if( i % X_scaling(X_scale) == 0) { //X scaling
                    buffer_big_2[big_buf_index_2] = chan_2_buf_2[i];
                    big_buf_index_2++;
                }
            }
        pingpong_2 = 0;
    }
}


/*******************************************************************************
* Function Name: int main(void)
*******************************************************************************/
int main(void)
{
     __enable_irq(); /* Enable global interrupts. */
    
    //initialize DMA  and descriptors
    cy_stc_dma_channel_config_t channelConfig1; //Channel 1
    channelConfig1.descriptor = &DMA_1_Descriptor_1; //DMA 1
    channelConfig1.preemptable = DMA_1_PREEMPTABLE;
    channelConfig1.priority = DMA_1_PRIORITY;
    channelConfig1.enable = false;
    channelConfig1.bufferable = DMA_1_BUFFERABLE;
    
    cy_stc_dma_channel_config_t channelConfig2; //Channel 1
    channelConfig2.descriptor = &DMA_2_Descriptor_1; //DMA 1
    channelConfig2.preemptable = DMA_2_PREEMPTABLE;
    channelConfig2.priority = DMA_2_PRIORITY;
    channelConfig2.enable = false;
    channelConfig2.bufferable = DMA_2_BUFFERABLE;
    
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_1, &DMA_1_Descriptor_1_config); //channel 1 descriptor 1
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_1, (uint32_t *) &(SAR->CHAN_RESULT[0])); 
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_1, chan_1_buf_1); 
    
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_2, &DMA_1_Descriptor_2_config); //channel 1 descriptor 2
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_2, (uint32_t *) &(SAR->CHAN_RESULT[0])); 
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_2, chan_1_buf_2); 
    
    Cy_DMA_Descriptor_Init(&DMA_2_Descriptor_1, &DMA_2_Descriptor_1_config); //channel 2 descriptor 1
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_2_Descriptor_1, (uint32_t *) &(SAR->CHAN_RESULT[3])); 
    Cy_DMA_Descriptor_SetDstAddress(&DMA_2_Descriptor_1, chan_2_buf_1); 
    
    Cy_DMA_Descriptor_Init(&DMA_2_Descriptor_2, &DMA_2_Descriptor_2_config); //channel 2 descriptor 2
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_2_Descriptor_2, (uint32_t *) &(SAR->CHAN_RESULT[3])); 
    Cy_DMA_Descriptor_SetDstAddress(&DMA_2_Descriptor_2, chan_2_buf_2); 
    
    
     /* Initialize channel */
    Cy_DMA_Channel_Init(DMA_1_HW, DMA_1_DW_CHANNEL, &channelConfig1); //change channel to a channel config structure (in yuja video)
    Cy_DMA_Channel_Init(DMA_2_HW, DMA_2_DW_CHANNEL, &channelConfig2);
    /* Enable DMA */
    Cy_DMA_Enable(DMA_1_HW);
    Cy_DMA_Enable(DMA_2_HW);
    /* Enable channel */
    Cy_DMA_Channel_Enable(DMA_1_HW, DMA_1_DW_CHANNEL);
    Cy_DMA_Channel_Enable(DMA_2_HW, DMA_2_DW_CHANNEL);
    
    /* Initialize and enable interrupt from DMA*/
    Cy_SysInt_Init(&DMA_1_INT_cfg, DMA_1_ISR);
    NVIC_EnableIRQ(DMA_1_INT_cfg.intrSrc);
    Cy_DMA_Channel_SetInterruptMask(DMA_1_HW, DMA_1_DW_CHANNEL, CY_DMA_INTR_MASK);
    
    Cy_SysInt_Init(&DMA_2_INT_cfg, DMA_2_ISR);
    NVIC_EnableIRQ(DMA_2_INT_cfg.intrSrc);
    Cy_DMA_Channel_SetInterruptMask(DMA_2_HW, DMA_2_DW_CHANNEL, CY_DMA_INTR_MASK);
    
    //Start the ADC and PWM
    PWM_1_Start(); //initialize PWM
    ADC_1_Start();
    ADC_1_StartConvert();
    //UART_printf_Start(); 
    
    
    GUI_Init(); // Initialize EmWin Graphics    
    
    /* Display the startup screen for 5 seconds */
    ShowStartupScreen();   
    Cy_SysLib_Delay(1000);
    
    /* Display background */
    drawBackground(XSIZE, YSIZE, XMARGIN, YMARGIN);
    /* Display grid*/
    drawGrid(XSIZE, YSIZE, XDIVISIONS, YDIVISIONS, XMARGIN, YMARGIN);
    /* Display scale settings */
    printScaleSettings(1000, 500, 200, 15);
    /* Display frequency */
    printFrequency(1, 1, 10, 15); //(freq_1, freq_2, xpos, ypos)
    
    for(;;)
    {  
        /* Display background */
        //drawBackground(XSIZE, YSIZE, XMARGIN, YMARGIN);
        /* Display grid*/
        drawGrid(XSIZE, YSIZE, XDIVISIONS, YDIVISIONS, XMARGIN, YMARGIN);
        /* Display scale settings */
        printScaleSettings(X_scale, Y_scale, 200, 15); //(xscale, yscale, xpos, ypos)
        /* Display frequency */
        printFrequency(frequency_1, frequency_2, 10, 15); //(freq_1, freq_2, xpos, ypos)
        
        GUI_SetPenSize(2);
        
        
        //get and store result from ADC (potentiometer 1)
        //uint32_t chan = 1UL; //channel (from pdl documentations)
        uint16_t compare_1 = ADC_1_GetResult16(1UL); //the number in front of UL is the ADC channel (potentiometer 1 in this case)
        compare_1 = PWM_conversion(compare_1); //convert input to PWM range
        PWM_1_SetCompare0(compare_1);
        

        //Draw Waveform 1
        if(big_buf_index_1 >= 1024) { //if X scaled buffer half full
            if(!trigger_1) { //if start state, draw
                Y_scaling(buffer_big_1, Y_scaled_1, Y_scale); //Y scale the X scaled coordinates
                frequency_1 = calc_freq(Y_scaled_1, X_scale); //calculate frequency
            } else { //if sotp state, draw
                Y_scaling(buffer_big_1, Y_scaled_1, Y_scale); //Y scale the X scaled coordinates
                trig_buf_fill(Y_scaled_1, triggered_1, trigger_level_1, Y_scale); //fill triggered buffer
                frequency_1 = calc_freq(Y_scaled_1, X_scale); //calculate frequency
            }
            for(int i = 0; i < 320; i++) { //Draw waveform 1
                if(!trigger_1) { //running state
                    draw_wave_line(i, Y_scaled_1, old_wave_1, compare_1, GUI_RED); //draw wave segment
                } else { //stopped state with trigger
                    draw_wave_line(i, triggered_1, old_wave_1, compare_1, GUI_RED); //draw wave segment
                }
            }
            big_buf_index_1 = 0; //reset X scaled buffer index
        }
        
        
        //get and store result from ADC (potentiometer 1)
        uint16_t compare_2 = ADC_1_GetResult16(2UL); //the number in front of UL is the ADC channel (potentiometer 2 in this case)
        compare_2 = PWM_conversion(compare_2); //convert input to PWM range
        PWM_1_SetCompare0(compare_2);
        
        
        //Draw Waveform 2
        if(big_buf_index_2 >= 1024) { //if X scaled buffer half full
            if(!trigger_2) { //if start state, draw
                Y_scaling(buffer_big_2, Y_scaled_2, Y_scale); //Y scale the X scaled coordinates
                frequency_2 = calc_freq(Y_scaled_2, X_scale); //calculate frequency
            }  else { //if sotp state, draw
                Y_scaling(buffer_big_2, Y_scaled_2, Y_scale); //Y scale the X scaled coordinates
                trig_buf_fill(Y_scaled_2, triggered_2, trigger_level_2, Y_scale); //fill triggered buffer
                frequency_2 = calc_freq(Y_scaled_2, X_scale); //calculate frequency
            }
            for(int i = 0; i < 320; i++) { //Draw waveform 1
                if(!trigger_2) { //running state
                    draw_wave_line(i, Y_scaled_2, old_wave_2, compare_2, GUI_GREEN); //draw wave segment
                } else { //stopped state with trigger
                    draw_wave_line(i, triggered_2, old_wave_2, compare_2, GUI_GREEN); //draw wave segment
                }
            }
            big_buf_index_2 = 0; //reset X scaled buffer index
        }
        
        
        //START & STOP & triggers
        //running = false; //simulate trigger start & stop
        //trigger_1 = true; //trigger channel 1
        //trigger_2 = true; //trigger channel 2
    }
}

/* [] END OF FILE */
