#include "erase.h"

int erase_nes( USBtransfer *transfer ) 
{
	uint8_t rv[8];
	//int i = 0;

	
	debug("erasing_nrom");

	io_reset(transfer);

	nes_init(transfer);
	
	debug("erasing PRG-ROM");
	dictionary_call_debug( transfer,   DICT_NES,   DISCRETE_EXP0_PRGROM_WR,   	0x5555,   0xAA,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   DISCRETE_EXP0_PRGROM_WR,   	0x2AAA,   0x55,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   DISCRETE_EXP0_PRGROM_WR,   	0x5555,   0x80,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   DISCRETE_EXP0_PRGROM_WR,   	0x5555,   0xAA,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   DISCRETE_EXP0_PRGROM_WR,   	0x2AAA,   0x55,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   DISCRETE_EXP0_PRGROM_WR,   	0x5555,   0x10,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   NES_CPU_RD,	0x8000,   0,	USB_IN, NULL, 2); 
	dictionary_call_debug( transfer,   DICT_NES,   NES_CPU_RD,	0x8000,   0,	USB_IN, NULL, 2); 

	do {
		dictionary_call( transfer,   DICT_NES,   NES_CPU_RD,	0x8000,   0,	USB_IN, rv, 2); 
		printf("%x, ",rv[1]);
	}
	while ( rv[1] != 0xFF);
	printf("\n done erasing prg.\n");

	debug("erasing CHR-ROM");
	dictionary_call_debug( transfer,   DICT_NES,   NES_PPU_WR,   	0x1555,   0xAA,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   NES_PPU_WR,   	0x0AAA,   0x55,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   NES_PPU_WR,   	0x1555,   0x80,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   NES_PPU_WR,   	0x1555,   0xAA,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   NES_PPU_WR,   	0x0AAA,   0x55,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   NES_PPU_WR,   	0x1555,   0x10,	USB_IN, NULL, 1);
	dictionary_call_debug( transfer,   DICT_NES,   NES_PPU_RD,	0x0000,   0,	USB_IN, NULL, 2); 
	dictionary_call_debug( transfer,   DICT_NES,   NES_PPU_RD,	0x0000,   0,	USB_IN, NULL, 2); 

	do {
		dictionary_call( transfer,   DICT_NES,   NES_PPU_RD,	0x0000,   0,	USB_IN, rv, 2); 
		printf("%x, ",rv[1]);
	}
	while ( rv[1] != 0xFF);
	printf("\n done erasing chr.\n");

	//dictionary_call( transfer,	IO,	IO_RESET,			0,		0);
	//dictionary_call( transfer,	IO,	NES_INIT,			0,		0);
	//dictionary_call( transfer,	IO,	EXP0_PULLUP_TEST,		0,		0);
	//dictionary_call( transfer,	PINPORT,	AUX_RD,		0,		0);
	//dictionary_call( transfer,	PINPORT,	AUX_RD,		0,		0);
	//dictionary_call( transfer,	PINPORT,	AUX_RD,		0,		0);
	//dictionary_call( transfer,	PINPORT,	AUX_RD,		0,		0);
	//dictionary_call( transfer,	PINPORT,	AUX_RD,		0,		0);
	//dictionary_call( transfer,	PINPORT,	AUX_RD,		0,		0);
	//dictionary_call( transfer,	PINPORT,	AUX_RD,		0,		0);

////software mode
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0xAA);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x2AAA,		0x55);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0x90);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8001,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8001,		0);
////exit software
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x8000,		0xF0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8001,		0);

//erase
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0xAA);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x2AAA,		0x55);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0x80);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0xAA);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x2AAA,		0x55);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0x10);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0); 
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0); 
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0); 
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0); 
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0); 
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);


	return 0;
//error:
//	return -1;
}
