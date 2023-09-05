/* ========================================


Notes: Contains functins for UART, UI, and character parsing

Lab Project
 * ========================================
*/


#include "stdlib.h"
#include "stdio.h"
#include <strings.h>



        /*
        //for testing
        sprintf(print_string, "\n\r\n\rBig buffer: \n\r"); //convert to string
        uart_printf(print_string); //print to terminal
        
        for(int i = 0; i < 512; i++) { //for testing
            //UART 
            sprintf(print_string, "%d   ", buffer_big[i]); //convert to string
            uart_printf(print_string); //print to terminal
            //Cy_SysLib_Delay(500);
        }
        */
        
        /*
        while (Cy_SCB_UART_GetRxFifoStatus(UART_printf_HW) & CY_SCB_UART_RX_NOT_EMPTY){ //if RX FIFO NOT EMPTY
            char c = getchar();
            sprintf(print_string, "%d   ", c); //convert to string
            uart_printf(print_string); //print to terminal
        }
        */