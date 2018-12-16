#include "cartridge.h"

//init all cart elements to UNKNOWN
//allocate memory for memory elements
int init_cart_elements( cartridge *cart ) 
{
	cart->console =	UNKNOWN;
	cart->mapper = UNKNOWN;
	cart->submap = UNKNOWN;
	cart->mapvariant = UNKNOWN;
	cart->manf = UNKNOWN;
	cart->product = UNKNOWN;
	cart->mirroring = UNKNOWN;
	cart->sound = UNKNOWN;
	
	cart->pri_rom = malloc( sizeof(memory));	
	check_mem(cart->pri_rom);
	init_memory_elements(cart->pri_rom);

	cart->sec_rom = malloc( sizeof(memory));	
	check_mem(cart->sec_rom);
	init_memory_elements(cart->sec_rom);

	cart->save_mem = malloc( sizeof(memory));	
	check_mem(cart->save_mem);
	init_memory_elements(cart->save_mem);

	cart->aux_mem = malloc( sizeof(memory));	
	check_mem(cart->aux_mem);
	init_memory_elements(cart->aux_mem);

	cart->logic_mem = malloc( sizeof(memory));	
	check_mem(cart->logic_mem);
	init_memory_elements(cart->logic_mem);

	return SUCCESS;
error:
	return ~SUCCESS;
}

int detect_console( cartridge *cart, USBtransfer *transfer ) 
{
	printf("attempting to detect cartridge...\n");
	//always start with resetting i/o
	io_reset( transfer );

	//TODO check if can detect a cart inserted backwards before continuing

	//check if NES/Famicom cart 
	nes_init( transfer );

	//if PPU /A13 is tied to CIRAM /CE we know it's NES/Famicom
	if ( jumper_ciramce_ppuA13n( transfer ) ) {
		//NES with 2 screen mirroring
		debug("CIRAM /CE is jumpered to PPU /A13");
		cart->console = NES_CART;
	} else if ( ciramce_inv_ppuA13( transfer ) ) {
		//some boards including INLXO-ROM boards drive CIRAM /CE with inverse of PPU A13
		debug("CIRAM /CE is inverse of PPU A13");
		cart->console = NES_CART;
	} else {
		//TODO check if CIRAM on cartridge or NT CHR-ROM
	}

	//if NES/FC determine which if possible
	//also possible that this could catch failed detections above which is current case with VRC6
	//Famicom has audio in and audio out pins connected to each other
	//For this to pass with a NES cart EXP6 would have to be jumpered to EXP0 for some strange reason
	//might fail if expansion audio mixing circuitry foils the check above
	//but worst case we detected NES when famicom which isn't big deal..
	if ( famicom_sound( transfer ) ) {
		debug("Famicom audio jumper found");
		cart->console = FC_CART;
	}

	//if couldn't detect NES/FC check for SNES cartridge
	//want to keep from outputting on EXP bus if NES cart was found
	if ( cart->console == UNKNOWN ) {
		//only way I can think of is if memory is enabled by certain addresses and control signals
		snes_init( transfer );
		if ( snes_mem_visible( transfer, 0x00, 0xFFFC )) {
			//CHECK for memory visible near NES reset vector
			debug("SNES memories detected");
			cart->console = SNES_CART;
		}
		//now it's possible that rom is there, but data is 0xFF so above test would fail
		//one option would be to drive bus low for short period and see if bus can be
		//driven low.  This could damage pin drivers though, best to create command in 
		//firmware to perform this to limit to one CPU cycle instead of USB xfr times

		//Prob best to check if able to read flash ID's if reset vector data is 0xFF
		//Since reset vector being 0xFF prob means it's blank flash cart..

		//playable SNES carts should have data somewhere in reset vector...
	}

	//always end with resetting i/o
	io_reset( transfer );

	switch (cart->console) {
		case NES_CART: printf("NES cartridge detected!\n");	
			break;
		case FC_CART: printf("Famicom cartridge detected!\n");	
			break;
		case SNES_CART: printf("SNES cartridge detected!\n");	
			break;
		case BKWD_CART: log_err("CARTRIDGE INSERTED BACKWARDS!!!\n");	
			//TODO detection not yet implemented need to look over connector pinouts
			break;
		case UNKNOWN: printf("Unable to detect cartridge...\n");
			//TODO error out properly
			break;
		default:
			sentinel("cartridge console element got set to something unsupported.");
	}

	return SUCCESS;

error:
	//always end with resetting i/o
	io_reset( transfer );
	return -1;

}

	//Can detect INL discrete, XO, and possily others with pullup EXP0 test
	//These carts have pullups on EXP0 so rising edge is faster
	//Also detecting that CIRAM /CE is driven by inverted PPU A13 is good way to 
	//detect INLX0-ROM boards.

	//SNES /RESET pin should control whether memory is enabled on original boards
	//INL SNES boards memory mapping is controlled by /RESET pin
	//roms are still visible when /RESET low, but SRAM isn't


/* Desc:Run through supported mapper mirroring modes to help detect mapper.
 * Pre: 
 * Post:cart mirroring set to found mirroring
 * Rtn: SUCCESS if nothing bad happened, neg if error with kazzo etc
 */
int detect_mirroring( cartridge *cart, USBtransfer *transfer ) 
{
	//always start with resetting i/o
	io_reset( transfer );

	if ( (cart->console == NES_CART) || (cart->console == FC_CART) ) {
		nes_init(transfer);
		//TODO call mmc3 detection function

		//TODO call mmc1 detection function

		//fme7 and many other ASIC mappers

		//none of ASIC mappers passed, assume fixed/discrete style mirroring
		cart->mirroring = ciram_A10_mirroring( transfer );
		switch (cart->mirroring) {
			case MIR_1SCNA:	debug("detected mirroring: 1scnA");	break;
			case MIR_1SCNB:	debug("detected mirroring: 1scnB");	break;
			case MIR_VERT:	debug("detected mirroring: Vert");	break;
			case MIR_HORIZ:	debug("detected mirroring: Horiz");	break;
			default:	debug("detected mirroring: %x", cart->mirroring);
		}
	}
	
	//always end with reset
	io_reset( transfer );

	return SUCCESS;

//error:
	//always end with resetting i/o
	//io_reset( transfer );
//	return -1;
}


//detecting mapper and memories ends up being one big operation
int detect_map_mem( cartridge *cart, USBtransfer *transfer, int oper ) 
{
	//debug("detecting mapping");
	//always start with resetting i/o
	io_reset( transfer );


	//Most ASIC mappers can be determined by their mirroring alone

	//Discrete mappers are tricky as memory operations are needed
	//If flashing can attempt to determine by reading flash manf/part
	//If dumping have to rely on rom page checksums and register writes
//================
// NES & Famicom
//================
	switch(cart->console) {

		case FC_CART:
		case NES_CART:
			nes_init(transfer);
			//debug("NES cart mapping");
			//gather other helpful info

			//result of chr-ram test
			if ( ppu_ram_sense( transfer, 0x0000 ) == SUCCESS ) {
				debug("CHR-RAM detected @ PPU $0000");
				cart->sec_rom->manf = SRAM;
				cart->sec_rom->part = SRAM;
			} 
			
			//perform WRAM test without corrupting results
			//TODO store result in save_mem


//mapper select switch<<<<-------------------------------------------------------------
switch (cart->mirroring) {
	case MIR_MMC1:
		break;
	case MIR_MMC3:
		break;
	case MIR_1SCNA:
	case MIR_1SCNB:
		break;
	case MIR_VERT:
	case MIR_HORIZ:
		//check for CHR-ROM flash
		if ( cart->sec_rom->part != SRAM ) {
			if ( read_flashID_chrrom_8K( transfer, cart->sec_rom ) == SUCCESS ) {
				//8KB bank with no banking operations
				debug("8K CHR-ROM flash detected");
				cart->sec_rom->size = 8 * KByte;
			}
		}
		//exp0 pullup test passes on many INL boards
		if ( exp0_pullup_test(transfer) == SUCCESS) {
			debug("EXP0 pullup cart mapping");
			//if passed exp0 test try 16/32KB bank flash check

			//if 16KB banks writing 0xFF to mapper reg should set A14 bit
			//That will cause flash detection to fail.
			//TODO handle bus conflicts...?
			dictionary_call( transfer, DICT_NES, 	NES_CPU_WR,	0x8000,	0xFF,	
								USB_IN,		NULL,	1);
			//if ID check passes, the should be 32KB PRG-ROM banking
			if ( read_flashID_prgrom_exp0( transfer, cart->pri_rom ) == SUCCESS ) {
				//32KB bank with EXP0->WE PRG-ROM sensed
				debug("32KB banking NES EXP0 enabled flash");
				cart->pri_rom->bank_size = 32 * KByte;
			} else { 
			//set mapper reg to 0 if present which sets A14 low when needed if 16KB banks
				dictionary_call( transfer, DICT_NES, 	NES_CPU_WR,	0x8000,	0x00,	
									USB_IN,		NULL,	1);
				if ( read_flashID_prgrom_exp0( transfer, cart->pri_rom ) == SUCCESS ){
					//16KB bank with EXP0->WE PRG-ROM sensed
					debug("16KB banking NES EXP0 enabled flash");
					cart->pri_rom->bank_size = 16 * KByte;
					cart->mapper = UxROM;
				}
			}
			//TODO determine how many banks are present
			//best to do this by writing last bank, then see if
			//blank banks can be found
		}
		//check for mapper 30 controlled PRG-ROM writes
		if ( read_flashID_prgrom_map30( transfer, cart->pri_rom ) == SUCCESS ){
			debug("16KB mapper30 flash writes enabled");
			cart->pri_rom->bank_size = 16 * KByte;
			cart->mapper = UNROM512;
		}
		//TODO check for mapper 31 EZ-NSF
		debug("PRG-ROM manfID: %x, prodID: %x", cart->pri_rom->manf, cart->pri_rom->part);
		break;
	default:
		sentinel("Problem with map mem detect based on mirroring switch statement.");
}
//mapper select switch<<<<-------------------------------------------------------------
			break;
//================
// SNES
//================
		case SNES_CART:
			snes_init(transfer);
			break;

		default:
			sentinel("This console not supported by detect_map_mem function.");
	}

	//always end with resetting i/o
	io_reset( transfer );

	return SUCCESS;

error:
	//always end with resetting i/o
	io_reset( transfer );
	return -1;
}

