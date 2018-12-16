#include "io.h"

/* Desc:disable all possible outputs and float EXP0
 * Pre: 
 * Post:all memories disabled data bus clear
 * Rtn: 
 */
void io_reset( USBtransfer *transfer ) 
{
	dictionary_call( transfer,	DICT_IO,	IO_RESET,		0,	0,	
					USB_IN,		NULL,	1);
}

/* Desc:initialize NES I/P and O/P
 * Pre: 
 * Post:memories disabled data bus clear
 * Rtn: 
 */
void nes_init( USBtransfer *transfer ) 
{
	dictionary_call( transfer,	DICT_IO,	NES_INIT,		0,	0,	
					USB_IN,		NULL,	1);
}


/* Desc:initialize SNES I/P and O/P
 * Pre: 
 * Post:memories disabled data bus clear
 * Rtn: 
 */
void snes_init( USBtransfer *transfer ) 
{
	dictionary_call( transfer,	DICT_IO,	SNES_INIT,		0,	0,	
					USB_IN,		NULL,	1);
}


/* Desc:test EXP0 pullup and determine now many avr cpu cycles it takes to go high
 * Pre: io_reset
 * Post:EXP0 left as pullup
 * Rtn: number of extra AVR cpu cycles it took to pullup EXP0
 *	SUCCESS = 0 which means pullup was immediate
 *	returns negative number on error
 */
int exp0_pullup_test( USBtransfer *transfer ) 
{
	uint8_t rv[RETURN_BUFF_SIZE];	
	int i = 0;

	//call EXP0 test command
	dictionary_call( transfer,	DICT_IO,	EXP0_PULLUP_TEST,	NILL,	NILL,	USB_IN,	
									rv,	RETURN_BUFF_SIZE);
	//return value first gives error/succes of opcode call
	//data first gives mask for EXP0 pin
	//then gives result of successive byte wide reads to be masked
	check( (rv[RV_ERR_IDX] == SUCCESS), "Device error calling EXP0 pullup test opcode" ) 

	if ( rv[RV_DATA0_IDX] & rv[RV_DATA0_IDX+1] ) {
		//mask out EXP0 bit, and EXP0 was set on first read after pullup
		return SUCCESS;
	} else if ( (rv[RV_DATA0_IDX] & rv[RV_DATA_MAX_IDX]) == 0 ) {
		//last read didn't have EXP0 high fail
		return GEN_FAIL;
	}else {
		//determine how many cycles it took for EXP0 to go high
		for( i=RV_DATA0_IDX+2; i<=RV_DATA_MAX_IDX; i++) {
			//debug("index %d", i);
			if ( rv[RV_DATA0_IDX] & rv[RV_DATA0_IDX+i] ) {
				return i-1;
			}
		}
	}

	//shouldn't be here..
	sentinel("EXP0 test failed to check results proplerly");

error:
	return -1;

}

