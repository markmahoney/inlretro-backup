#include "snes.h"

/* Desc:check if ROM visible at provided address
 * Pre: snes_init() been called to setup i/o
 * Post:Address left on bus memories disabled
 * Rtn: FALSE if memory not found
 */
int snes_mem_visible( USBtransfer *transfer, uint8_t bank, uint16_t addr ) 
{
	uint8_t rv[RV_DATA0_IDX+1];

	//place address on bus
	dictionary_call( transfer, DICT_PINPORT, 	ADDR24_SET,	addr,		bank,	
					USB_IN,		NULL,	1);
	//ensure data bus is pulled up 
	dictionary_call( transfer, DICT_PINPORT, 	DATA_HI,	0,		0,	
					USB_IN,		NULL,	1);
	//read data bus
	dictionary_call_debug( transfer, DICT_PINPORT, 	DATA_RD,	0,		0,	
					USB_IN,		rv,	RV_DATA0_IDX+1);
	if ( rv[RV_DATA0_IDX] != 0xFF ) {
		debug("Can't pull up data bus in attempt to detect SNES cart");
		return FALSE;
	}

	//enable rom control signals
	dictionary_call( transfer, DICT_PINPORT, 	SRST_HI,	0,		0,	
					USB_IN,		NULL,	1);
	dictionary_call( transfer, DICT_PINPORT, 	CSRD_LO,	0,		0,	
					USB_IN,		NULL,	1);
	dictionary_call( transfer, DICT_PINPORT, 	ROMSEL_LO,	0,		0,	
					USB_IN,		NULL,	1);
	//read data bus
	dictionary_call_debug( transfer, DICT_PINPORT, 	DATA_RD,	0,		0,	
					USB_IN,		rv,	RV_DATA0_IDX+1);
	//clear data bus
	dictionary_call( transfer, DICT_PINPORT, 	SRST_LO,	0,		0,	
					USB_IN,		NULL,	1);
	dictionary_call( transfer, DICT_PINPORT, 	CSRD_HI,	0,		0,	
					USB_IN,		NULL,	1);
	dictionary_call( transfer, DICT_PINPORT, 	ROMSEL_HI,	0,		0,	
					USB_IN,		NULL,	1);
	if ( rv[RV_DATA0_IDX] != 0xFF ) {
		debug("Found memory with SNES control signals");
		return ~FALSE;
	}
	
	return FALSE;
}

