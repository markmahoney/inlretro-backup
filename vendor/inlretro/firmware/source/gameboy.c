#include "gameboy.h"

//only need this file if connector is present on the device
#ifdef GB_CONN 

//=================================================================================================
//
//	GAMEBOY operations
//	This file includes all the gameboy functions possible to be called from the gameboy dictionary.
//
//	See description of the commands contained here in shared/shared_dictionaries.h
//
//=================================================================================================

/* Desc:Function takes an opcode which was transmitted via USB
 * 	then decodes it to call designated function.
 * 	shared_dict_gameboy.h is used in both host and fw to ensure opcodes/names align
 * Pre: Macros must be defined in firmware pinport.h
 * 	opcode must be defined in shared_dict_gameboy.h
 * Post:function call complete.
 * Rtn: SUCCESS if opcode found and completed, error if opcode not present or other problem.
 */
uint8_t gameboy_call( uint8_t opcode, uint8_t miscdata, uint16_t operand, uint8_t *rdata )
{

#define	RD_LEN	0
#define	RD0	1
#define	RD1	2

#define	BYTE_LEN 1
#define	HWORD_LEN 2
	
	switch (opcode) { 
//		//no return value:
		case GAMEBOY_WR:	
			gameboy_wr( operand, miscdata );
			break;

		//8bit return values:
		case GAMEBOY_RD:
			rdata[RD_LEN] = BYTE_LEN;
			rdata[RD0] = gameboy_rd( operand );
			break;
		default:
			 //macro doesn't exist
			 return ERR_UNKN_GAMEBOY_OPCODE;
	}
	
	return SUCCESS;

}

/* Desc:Gameboy CPU Read without being so slow
 * 	decode A15-14 from addrH to set SRAM /CS as expected
 * 	ignore clock pin toggling pretty sure it's unconnected on most carts
 * 	going by reference here: 
 * 	https://dhole.github.io/media/gameboy_stm32f4/cpu_manual_timing_small.png
 * Pre: gameboy_init() setup of io pins
 * Post:address left on bus
 * 	data bus left clear
 * Rtn:	Byte read from cartridge at addrHL
 */
uint8_t	gameboy_rd( uint16_t addr )
{
	uint8_t	read;	//return value

	//cycle would start with clock rise

	//set address bus
	ADDR_SET(addr);
	
	//enable /RD pin
	CSRD_LO();

	//set SRAM /CS
	//low for $A000-BFFF
	if( (addr >= 0xA000) && (addr < 0xC000) ) {	//addressing cart RAM space
		ROMSEL_LO();	//this is actually the SRAM /CS pin
	}

	//half cycle with clock fall
	//and /WR low for writes

	//couple more NOP's waiting for data
	//zero nop's returned previous databus value
	NOP();	//one nop got most of the bits right
	NOP();	//two nop got all the bits right
	NOP();	//add third nop for some extra
	NOP();	//one more can't hurt
	//might need to wait longer for some carts...

	//latch data
	DATA_RD(read);

	//return bus to default
	ROMSEL_HI();
	CSRD_HI();

	//next cycle clock rise
	
	return read;
}


/* Desc:Gameboy CPU Write
 * 	decode A15-14 from addrH to set SRAM /CS as expected
 * 	ignore clock pin toggling pretty sure it's unconnected on most carts
 * Pre: gameboy_init() setup of io pins
 * Post:data latched by anything listening on the bus
 * 	address left on bus
 * 	data left on bus, but pullup only
 * Rtn:	None
 */
void	gameboy_wr( uint16_t addr, uint8_t data )
{
	//cycle would start with clock rise

	//set address bus
	ADDR_SET(addr);
	
	//set SRAM /CS
	//low for $A000-BFFF
	if( (addr >= 0xA000) && (addr < 0xC000) ) {	//addressing cart RAM space
		ROMSEL_LO();	//this is actually the SRAM /CS pin
	}


	//put data on bus
	DATA_OP();
	DATA_SET(data);

	//half cycle with clock fall
	//and /WR low for writes
	CSWR_LO();

	//give some time
	NOP();
	NOP();
	NOP();

	//latch data to cart memory/mapper
	CSWR_HI();
	ROMSEL_HI();

	//Free data bus
	DATA_IP();
}

/* Desc:GAMEBOY 8bit CPU Page Read with optional USB polling
 * 	decode A15 from addrH to set SRAM /CE as expected
 *	if poll is true calls usbdrv.h usbPoll fuction
 *	this is needed to keep from timing out when double buffering usb data
 * Pre: gameboy_init() setup of io pins
 *	num_bytes can't exceed 256B page boundary
 * Post:address left on bus
 * 	data bus left clear
 *	data buffer filled starting at first to last
 * Rtn:	Index of last byte read
 */
uint8_t gameboy_page_rd_poll( uint8_t *data, uint8_t addrH, uint8_t first, uint8_t len, uint8_t poll )
{
	uint8_t i;

	//set address bus
	ADDRH(addrH);
	
	//enable /RD pin
	CSRD_LO();

	//set SRAM /CS
	//low for $A000-BFFF
	if( (addrH >= 0xA0) && (addrH < 0xC0) ) {	//addressing cart RAM space
		ROMSEL_LO();	//this is actually the SRAM /CS pin
	}

	//set lower address bits
	ADDRL(first);		//doing this prior to entry and right after latching

	//extra NOP was needed on stm6 as address hadn't settled in time for the very first read
	NOP();	
				//gives longest delay between address out and latching data
	for( i=0; i<=len; i++ ) {
		//testing shows that having this if statement doesn't affect overall dumping speed
		if ( poll ) {
			usbPoll();	//Call usbdrv.h usb polling while waiting for data
		} else {
			NOP();	//couple more NOP's waiting for data
			NOP();	//one prob good enough considering the if/else
		}

		//gameboy needed some extra NOPS
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();

		//latch data
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


#endif //GB_CONN
