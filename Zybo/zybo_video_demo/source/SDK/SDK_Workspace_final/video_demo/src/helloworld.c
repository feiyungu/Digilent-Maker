/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include <xil_io.h>
#include "linear_filter_header.h"
#include "color_space_header.h"
#include <xparameters.h>

//void print(char *str);

#define SW_BASEADDR XPAR_SWS_4BITS_BASEADDR
#define BTN_BASEADDR XPAR_BTNS_4BITS_BASEADDR
#define COLORSPACE_BASEADDR XPAR_AXI_COLOR_SPACE_0_BASEADDR
#define LINEAR_BASEADDR XPAR_AXI_LINEARFILTER_0_BASEADDR

/*
 * 1280x720 parameters
 */
#define hsync 40   //0x00000028	//40
#define hbpr 260  //0x00000104	//260
#define hfpr 1540 //0x00000604	//1540
#define htr 1649  //0x00000671	//1649
#define vsync 5   //0x00000005	//5
#define vbpr 25   //0x00000019	//25
#define vfpr 745  //0x000002E9	//745
#define vtr 749   //0x000002ED	//749

int main()
{
    u32 btnState = 0;
    u32 swState = 0;
    u32 linearState = 0;
    u32 colorState = 0;
    u32 lLinearFilterBaseAddress = LINEAR_BASEADDR;

	init_platform();



   // xil_printf("Hello World\n\r");

	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterMat11, 0x00000000); // <1,1>
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterMat12, 0x00000000); // <1,2>
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterMat13, 0x00000000); // <1,3>
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterMat21, 0x00000000); // <2,1>
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterMat22, 0x00000001); // <2,2>
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterMat23, 0x00000000); // <2,3>
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterMat31, 0x00000000); // <3,1>
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterMat32, 0x00000000); // <3,2>
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterMat33, 0x00000000); // <3,3>
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterDIV, 0x00000001); // set divider: 1
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterHSR, hsync); // hsync
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterHBPR, hbpr); // hbpr
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterHFPR, hfpr); // hfpr
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterHTR, htr); // htr
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterVSR, vsync); // vsync
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterVBPR, vbpr); // vbpr
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterVFPR, vfpr); // vfpr
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterVTR, vtr); // vtr
	Xil_Out32(lLinearFilterBaseAddress + blLinearFilterCR, 0x00000001); // enable linear filter

    ColorSpaceInit(COLORSPACE_BASEADDR, 0);
    //LinearFilterInit(LINEAR_BASEADDR, 0);

    //xil_printf("Entering While Loop\n\r");

    while (1)
    {
    	btnState = Xil_In32(BTN_BASEADDR);
    	swState = Xil_In32(SW_BASEADDR) & 0b0111;


    	if ((btnState & 0b0001) && linearState != LinearFilter_Off)
    	{
    		linearState = LinearFilter_Off;
    		LinearFilterInit(LINEAR_BASEADDR, linearState);
    	    //xil_printf("Linear Filter state changed!\n\r");
    	}
    	else if ((btnState & 0b0010) && linearState != LinearFilter_Bilinear)
    	{
    		linearState = LinearFilter_Bilinear;
    		LinearFilterInit(LINEAR_BASEADDR, linearState);
    	    //xil_printf("Linear Filter state changed!\n\r");
    	}
    	else if ((btnState & 0b0100) && linearState != LinearFilter_Sobel)
    	{
    		linearState = LinearFilter_Sobel;
    		LinearFilterInit(LINEAR_BASEADDR, linearState);
    	    //xil_printf("Linear Filter state changed!\n\r");
    	}
    	else if ((btnState & 0b1000) && linearState != LinearFilter_Enhanced)
    	{
    		linearState = LinearFilter_Enhanced;
    		LinearFilterInit(LINEAR_BASEADDR, linearState);
    	    //xil_printf("Linear Filter state changed!\n\r");
    	}

    	if (swState != colorState)
    	{
    		colorState = swState;
    		ColorSpaceInit(COLORSPACE_BASEADDR, colorState);
    	    //xil_printf("Color Space state changed!\n\r");
    	}
//    	if ((btnState & 0b100) && swState != ctrlState)
//		{
//			ctrlState = swState;
//			temp = Xil_In32(LINEAR_BASEADDR);
//			if (ctrlState == 0b0001)
//				temp = temp | 0b1;
//			else if (ctrlState == 0)
//				temp = temp & ~0b1;
//			Xil_Out32(LINEAR_BASEADDR, temp);
//
//			xil_printf("ctrl state changed!\n\r");
//
//			ctrlState = swState;
//			temp = Xil_In32(COLORSPACE_BASEADDR);
//			if (ctrlState == 0b0001)
//				temp = temp | 0x80000000;
//			else if (ctrlState == 0)
//				temp = temp & ~0x80000000;
//			Xil_Out32(COLORSPACE_BASEADDR, temp);
//
//			xil_printf("ctrl state changed!\n\r");
//		}
    }
    return 0;
}
