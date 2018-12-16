#include "sega.h"

//only need this file if connector is present on the device
#ifdef SEGA_CONN 

//=================================================================================================
//
//	SEGA operations
//	This file includes all the sega functions possible to be called from the sega dictionary.
//
//	See description of the commands contained here in shared/shared_dictionaries.h
//
//=================================================================================================

/* Desc:Function takes an opcode which was transmitted via USB
 * 	then decodes it to call designated function.
 * 	shared_dict_sega.h is used in both host and fw to ensure opcodes/names align
 * Pre: Macros must be defined in firmware pinport.h
 * 	opcode must be defined in shared_dict_sega.h
 * Post:function call complete.
 * Rtn: SUCCESS if opcode found and completed, error if opcode not present or other problem.
 */
uint8_t sega_call( uint8_t opcode, uint8_t miscdata, uint16_t operand, uint8_t *rdata )
{

#define	RD_LEN	0
#define	RD0	1
#define	RD1	2

#define	BYTE_LEN 1
#define	HWORD_LEN 2

	uint16_t temp;
	
	switch (opcode) { 
//		//no return value:
		case SEGA_WR:	
			sega_wr( operand, miscdata );
			break;

		case SET_BANK:	
			temp = ADDR_CUR; 	//this will get stomped
#define LOMEM_TIME_MASK 0x84
			//A17-18, 20-23
			FFADDR_SET( operand | LOMEM_TIME_MASK );	//TODO decode #TIME & LO_MEM
			ADDR_SET(temp);		//restore A1-16
#define SEGA_A19_MASK 0x04
			//A19
			if ( operand & SEGA_A19_MASK ) {
				IRQ_HI();
			} else {
				IRQ_LO();
			}
			break;

		//8bit return values:
		case SEGA_RD:
			rdata[RD_LEN] = BYTE_LEN;
			rdata[RD0] = sega_rd( operand );
			break;
		default:
			 //macro doesn't exist
			 return ERR_UNKN_SEGA_OPCODE;
	}
	
	return SUCCESS;

}

uint8_t	sega_rd( uint16_t addr )
{
	return 0xAA;
}


void	sega_wr( uint16_t addr, uint8_t data )
{
	return;
}


/* Desc:SNES ROM Page Read with optional USB polling
 * 	/ROMSEL based on romsel arg, EXP0/RESET unaffected
 *	if poll is true calls usbdrv.h usbPoll fuction
 *	this is needed to keep from timing out when double buffering usb data
 * Pre: snes_init() setup of io pins
 *	num_bytes can't exceed 256B page boundary
 * Post:address left on bus
 * 	data bus left clear
 *	data buffer filled starting at first to last
 * Rtn:	Index of last byte read
 */
uint8_t genesis_page_rd( uint8_t *data, uint16_t addrH, uint8_t first, uint8_t len )
{
	uint8_t i;

	uint16_t address = first>>1; //shift because there is no A0

	//address = ((addrH<<8) | first)>>1;	//shift because there is no A0
	address = (addrH<<7) | address;	//shift because there is no A0

	//set address
	//ADDRH(addrH);
	ADDRH(address>>8);
	
	//set #C_CE
	ROMSEL_LO();

	//set #C_OE
	CSRD_LO();

	first = address;

	//set lower address bits
	ADDRL(first);		//doing this prior to entry and right after latching
				//gives longest delay between address out and latching data
	for( i=0; i<=len; i++ ) {

		//gameboy needed some extra NOPS
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		
		//latch data high byte
		data[i] = HDATA_VAL;

		i++;

		//latch data low byte
		DATA_RD(data[i]);

		//set lower address bits
		//ADDRL(++first);	THIS broke things, on stm adapter because macro expands it twice!
		first++;
		ADDRL(first);
	}

	//return bus to default
	CSRD_HI();
	ROMSEL_HI();
	
	//return index of last byte read
	return i;
}


#endif //SEGA_CONN
