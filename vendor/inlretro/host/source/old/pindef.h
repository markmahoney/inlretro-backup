#ifndef _pindef_h
#define _pindef_h

//--------------------------------------------------
//	PIN and PORT Definitions
//	gives easier/more abstract access
//	to I/O reading on host
//	the dictionaries only provide access to 
//	read a byte wide port for speed and simplicity
//	of code on firmware.  The host must decode
//	these bytes to get desired pin data
//	these are copied from firmware pinport.h
//	then adjusted to work in host setting
//--------------------------------------------------

//To utilize these macros first make a dictionary call and provide the <PIN>_RD opcode
//This is effectively just using the CTL_RD/AUX_RD opcode but easier bc you don't have 
//to remember what pin is on which port.
//Store the result of that dictionary call in a byte
//This is currently the second byte returned as the first one is the error code
//Then mask the return data byte with <PIN>_MSK
//If result is 0 pin is low, if equal to <PIN>_MSK (not zero) pin is high


//first make it so we don't have to remember what port the io resides on
//these don't work if the ddr wasn't set to i/p before hand
//these are simply macros to call the dictionary entry in shared_dict_pinport.h
//these become new opcodes which the host can use to call the underlying PIN READ
//they will return the byte wide PIN register which then must be masked to get
//the specific io desired
#define	M2_RD		CTL_RD
#define	ROMSEL_RD	CTL_RD
//#define	PRGRW_RD	CTL_RD	broke due to redefine PRGRW_RD sets pin low
#define	FREE_RD		CTL_RD
#define	CSRD_RD		CTL_RD
#define	CSWR_RD		CTL_RD
#define	CICE_RD		CTL_RD
#define	AHL_RD		CTL_RD	

#define	EXP0_RD		AUX_RD
#define	FC_APU_RD	AUX_RD
#define	TDO_RD		AUX_RD
#define	SRST_RD		AUX_RD
#define	LED_RD		AUX_RD
#define	EXP9_RD		AUX_RD
#define	IRQ_RD		AUX_RD
#define	CIA10_RD	AUX_RD
#define	BL_RD		AUX_RD
#define	AXLOE_RD	AUX_RD



//Now we need a mask for the io's place in the return byte
//This was copied from firmware's pinport.h
//then _MSK prefix to pin name to make the fact this is a mask obvious
//in place of PC#/PD# which only makes sense to avr-gcc place the hex mask equivalent
//============================
//CTL PORTC
//============================
#define M2_MSK		0x01	//PC0	//NES, FC, & SNES (SYSCLK)
#define ROMSEL_MSK	0x02	//PC1	//(aka PRG/CE) NES, FC, & SNES 
#define PRGRW_MSK	0x04	//PC2	//PRG R/W on NES & FC 

//#ifdef PURPLE_KAZZO
#define p_AXL_MSK	0x08	//PC3	//EXP FF CLK on purple boards
//#else
#define FREE_MSK	0x08	//PC3	//Free pin on all other boards
//#endif

#define CSRD_MSK	0x10	//PC4	//NES & FC CHR /RD, SNES /RD
#define CSWR_MSK	0x20	//PC5	//NES & FC CHR /WR, SNES /WR
#define CICE_MSK 	0x40	//PC6	//NES & FC CIRAM /CE, most carts are 2screen tying this to CHR /A13 making this an I/P

//#ifdef GREEN_KAZZO
#define g_AXHL_MSK	0x80	//PC7	//Both ADDR_MID & EXP/ADDRHI FF CLK on green prototype
//#else
#define AHL_MSK		0x80	//PC7	//ADDR MID FF CLK per orig kazzo design
//#endif



//============================
//AUX PORTD
//============================
#define EXP0_MSK	0x01	//PD0	//NES EXP0 controls a number of varying flash cart features...
#define FC_APU_MSK	0x01	//PD0	//FC Audio in cart from 2A03 APU
#define TDO_MSK		0x01	//PD0	//CPLD JTAG on INL-ROM NES/FC boards released after ~Oct2016
#define SRST_MSK	0x01	//PD0	//SNES /RESET pin used for CPLD prgm/play mode and SRAM CE

#define LED_MSK		0x02	//PD1	//LED on INL retro prog-dumper
#define EXP9_MSK	0x02	//PD1	//NES dual purposed pin

#define USBP_MSK	0x04	//PD2	//USB D+ don't touch this pin!
#define IRQ_MSK		0x08	//PD3	//Connected to NES, FC, & SNES
#define USBM_MSK	0x10	//PD4	//USB D- don't touch this pin!
#define CIA10_MSK	0x20	//PD5	//NES & FC CIRAM A10 (aka VRAM A10)
#define BL_MSK		0x40	//PD6	//Bootloader switch BL->GND, RUN->float

//#ifdef PURPLE_KAZZO
#define pg_XOE_MSK	0x80	//PD7	//EXP/ADDRHI FF /OE pin on purple and green boards
//#endif
//#ifdef GREEN_KAZZO
#define pg_XOE_MSK	0x80	//PD7	//EXP/ADDRHI FF /OE pin on purple and green boards
//#endif
//#ifndef pg_XOE	//FINAL_DESIGN
#define AXLOE_MSK	0x80	//PD7	//EXP/ADDRHI FF CLK & /OE pin on final board versions
//#endif



//The following macros help locate control signals behind flipflops
//To set/clear these signals call the dictionary flipflop's opcode with this value
//note all 8 pins get set at once must and/or in values to a host variable
//if trying to maintain value of other signals

//pin masks for where signal resides on ADDRH flipflop
#define PPU_A13N_MSK	0x80
#define PPU_A13_MSK	0x20

//EXP FF connects D7:0 to EXP8:1 so everything is shifted one bit
//0b8765 4321
#define EXP8_MSK	0x80
#define EXP7_MSK	0x40

#define FC_RF_MSK	0x20	
#define EXP6_MSK	0x20

#define EXP5_MSK	0x10
#define EXP4_MSK	0x08
#define EXP3_MSK	0x04
#define EXP2_MSK	0x02
#define EXP1_MSK	0x01


#endif
