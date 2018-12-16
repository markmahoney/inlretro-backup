#include "nes.h"

/* Desc:check if PPU /A13 -> CIRAM /CE jumper present
 *	Does NOT check if PPU A13 is inverted and then drives CIRAM /CE
 * Pre: nes_init() been called to setup i/o
 * Post:PPU /A13 left high (disabled), all other ADDRH signals low
 * Rtn: FALSE if jumper is not set
 */
int jumper_ciramce_ppuA13n( USBtransfer *transfer ) 
{
	uint8_t rv[RV_DATA0_IDX+1];

	//check that we can clear CIRAM /CE with PPU /A13
	dictionary_call( transfer, DICT_PINPORT, 	ADDRH_SET,	0,		0,	
					USB_IN,		NULL,	1);
	//read CIRAM /CE pin
	dictionary_call( transfer, DICT_PINPORT, 	CICE_RD,	0,		0,	
					USB_IN,		rv,	RV_DATA0_IDX+1);

	//CIRAM /CE's port PIN register contents are now in rv[RV_DATA_IDX]
	//need to mask out the CIRAM /CE pin
	if ( rv[RV_DATA0_IDX] & CICE_MSK ) {
		//CIRAM /CE pin was always high regardless of PPU /A13
		debug("CIRAM /CE high when /A13 low ");
		return FALSE;
	}

	//set PPU /A13 high
	dictionary_call( transfer, DICT_PINPORT, 	ADDRH_SET,	PPU_A13N_MSK,	0,	
					USB_IN,		NULL,	1);
	//read CIRAM /CE pin
	dictionary_call( transfer, DICT_PINPORT, 	CICE_RD,	0,		0,	
					USB_IN,		rv,	RV_DATA0_IDX+1);

	//CIRAM /CE's port PIN register contents are now in rv[RV_DATA_IDX]
	//need to mask out the CIRAM /CE pin
	if ( (rv[RV_DATA0_IDX] & CICE_MSK) == 0 ) {
		//CICE jumper not present
		debug("CIRAM /CE low when /A13 high ");
		return FALSE;
	}

	//CICE low jumper appears to be present
	debug("CIRAM /CE <- PPU /A13 jumper present");
	return ~FALSE;

}

/* Desc:check if PPU A13 is inverted then drives CIRAM /CE 
 *	Some mappers may do this including INLXO-ROM boards
 *	Does NOT check if PPU /A13 is drives CIRAM /CE
 * Pre: nes_init() been called to setup i/o
 * Post:PPU A13 left disabled (hi)
 * Rtn: FALSE if inverted PPU A13 doesn't drive CIRAM /CE
 */
int ciramce_inv_ppuA13( USBtransfer *transfer ) 
{
	uint8_t rv[RV_DATA0_IDX+1];

	//set PPU /A13 low
	dictionary_call( transfer, DICT_PINPORT, 	ADDRH_SET,	0,		0,	
					USB_IN,		NULL,	1);
	//read CIRAM /CE pin
	dictionary_call( transfer, DICT_PINPORT, 	CICE_RD,	0,		0,	
					USB_IN,		rv,	RV_DATA0_IDX+1);

	// CIRAM /CE should be high if inverted A13 is what drives it
	if ( (rv[RV_DATA0_IDX] & CICE_MSK) == 0 ) {
		//CICE jumper not present
		debug("CIRAM /CE low when /A13 low ");
		return FALSE;
	}

	//check that we can clear CIRAM /CE with PPU /A13 high
	dictionary_call( transfer, DICT_PINPORT, 	ADDRH_SET,	PPU_A13_MSK,	0,	
					USB_IN,		NULL,	1);
	//read CIRAM /CE pin
	dictionary_call( transfer, DICT_PINPORT, 	CICE_RD,	0,		0,	
					USB_IN,		rv,	RV_DATA0_IDX+1);

	// CIRAM /CE should be low if inverted A13 is what drives it
	if ( rv[RV_DATA0_IDX] & CICE_MSK ) {
		//CIRAM /CE pin was always high regardless of PPU /A13
		debug("CIRAM /CE high when /A13 high ");
		return FALSE;
	}


	//CICE low jumper appears to be present
	debug("CIRAM /CE <- inverse PPU /A13");
	return ~FALSE;

}


/* Desc:check for famicom audio in->out jumper
 *	This drives EXP6 (RF out) -> EXP0 (APU in) which is backwards..
 *	not much can do about that for current kazzo designs
 *	There are probably caps/resistors for synth carts anyway
 *	but to be safe only apply short pulses.
 *	While we typically don't want to apply 5v to EXP port on NES carts,
 *	this only does so for EXP6 which is safe on current designs.
 *	All other EXP1-8 pins are only driven low.
 * Pre: nes_init() been called to setup i/o
 *	which makes EXP0 floating i/p
 * Post:EXP FF left disabled and EXP0 floating
 *	AXLOE pin returned to input with pullup
 * Rtn: FALSE if jumper/connection is not present
 * Test:Works on non-expansion sound carts obviously
 *	Works on VRC6 and VRC7
 *	Others untested
 */
int famicom_sound( USBtransfer *transfer ) 
{
	uint8_t rv[RV_DATA0_IDX+1];

	//EXP0 should be floating input
	//AXLOE pin needs to be set as output and
	//EXP FF needs enabled before we can clock it, 
	//but don't leave it enabled before exiting function

	//set AXLOE to output
	dictionary_call( transfer, DICT_PINPORT, 	AXLOE_OP,	0,		0,	
					USB_IN,		NULL,	1);
	//enable EXP FF
	dictionary_call( transfer, DICT_PINPORT, 	EXPFF_OP,	0,		0,	
					USB_IN,		NULL,	1);
	//Latch low first
	dictionary_call( transfer, DICT_PINPORT, 	ADDRX_SET,	0,		0,	
					USB_IN,		NULL,	1);
	//read EXP0 Famicom APU audio pin
	dictionary_call( transfer, DICT_PINPORT, 	FC_APU_RD,	0,		0,	
					USB_IN,		rv,	RV_DATA0_IDX+1);

	//need to mask out the pin
	if ( rv[RV_DATA0_IDX] & FC_APU_MSK ) {
		debug("RF audio out (EXP6) didn't drive APU audio in (EXP0) low");
		//disable EXP FF
		dictionary_call( transfer, DICT_PINPORT, 	EXPFF_FLT,	0,		0,	
						USB_IN,		NULL,	1);
		//retun AXLOE to input
		dictionary_call( transfer, DICT_PINPORT, 	AXLOE_IP,	0,		0,	
					USB_IN,		NULL,	1);
		return FALSE;
	}

	//Latch pin high 
	dictionary_call( transfer, DICT_PINPORT, 	ADDRX_SET,	FC_RF_MSK,	0,	
					USB_IN,		NULL,	1);
	//read EXP0 Famicom APU audio pin
	dictionary_call( transfer, DICT_PINPORT, 	FC_APU_RD,	0,		0,	
					USB_IN,		rv,	RV_DATA0_IDX+1);
	//disable EXP FF
	dictionary_call( transfer, DICT_PINPORT, 	EXPFF_FLT,	0,		0,	
					USB_IN,		NULL,	1);
	//retun AXLOE to input
	dictionary_call( transfer, DICT_PINPORT, 	AXLOE_IP,	0,		0,	
					USB_IN,		NULL,	1);

	//mask pin from byte
	if ( (rv[RV_DATA0_IDX] & FC_APU_MSK) == 0 ) {
		debug("RF audio out (EXP6) didn't drive APU audio in (EXP0) high");
		return FALSE;
	}

	//CICE low jumper appears to be present
	debug("RF audio out (EXP6) is connected to APU audio in (EXP0)");
	return ~FALSE;

}

/* Desc:PRG-ROM flash manf/prod ID sense test
 *	Using EXP0 /WE writes 
 *	Only senses SST flash ID's
 *	Assumes that isn't getting tricked by having manf/prodID at $8000/8001
 *	could add check and increment read address to ensure doesn't get tricked..
 * Pre: nes_init() been called to setup i/o
 *	exp0 pullup test must pass
 *	if ROM A14 is mapper controlled it must be low when CPU A14 is low
 *	controlling A14 outside of this function acts as a means of bank size detection
 * Post:memory manf/prod ID set to read values if passed
 *	memory wr_dict and wr_opcode set if successful
 *	Software mode exited if entered successfully
 * Rtn: SUCCESS if flash sensed, GEN_FAIL if not, neg if error 
 */
int read_flashID_prgrom_exp0( USBtransfer *transfer, memory *flash ) {

	uint8_t rv[RV_DATA0_IDX];

	//enter software mode
	//ROMSEL controls PRG-ROM /OE which needs to be low for flash writes
	//So unlock commands need to be addressed below $8000
	//DISCRETE_EXP0_PRGROM_WR doesn't toggle /ROMSEL by definition though, so A15 is unused
	//	    15 14 13 12
	// 0x5 = 0b  0  1  0  1	-> $5555
	// 0x2 = 0b  0  0  1  0	-> $2AAA
	dictionary_call( transfer, DICT_NES, 	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0xAA,	
									USB_IN,		NULL,	1);
	dictionary_call( transfer, DICT_NES, 	DISCRETE_EXP0_PRGROM_WR,	0x2AAA,		0x55,	
									USB_IN,		NULL,	1);
	dictionary_call( transfer, DICT_NES, 	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0x90,	
									USB_IN,		NULL,	1);
	//read manf ID
	dictionary_call( transfer, DICT_NES, 	NES_CPU_RD,			0x8000,		NILL,	
								USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("manf id: %x", rv[RV_DATA0_IDX]);
	if ( rv[RV_DATA0_IDX] != SST_MANF_ID ) {
		return GEN_FAIL;
		//no need for software exit since failed to enter
	}

	//read prod ID
	dictionary_call( transfer, DICT_NES, 	NES_CPU_RD,			0x8001,		NILL,	
								USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("prod id: %x", rv[RV_DATA0_IDX]);
	if ( (rv[RV_DATA0_IDX] == SST_PROD_128)
	||   (rv[RV_DATA0_IDX] == SST_PROD_256)
	||   (rv[RV_DATA0_IDX] == SST_PROD_512) ) {
		//found expected manf and prod ID
		flash->manf = SST_MANF_ID;
		flash->part = rv[RV_DATA0_IDX];
		flash->wr_dict = DICT_NES;
		flash->wr_opcode = DISCRETE_EXP0_PRGROM_WR;
	}

	//exit software
	dictionary_call( transfer, DICT_NES, 	DISCRETE_EXP0_PRGROM_WR,	0x8000,		0xF0,	
									USB_IN,		NULL,	1);

	//verify exited
	//dictionary_call( transfer, DICT_NES, 	NES_CPU_RD,			0x8000,		NILL,	
	//							USB_IN,		rv,	RV_DATA0_IDX+1);
	//debug("prod id: %x", rv[RV_DATA0_IDX]);

	return SUCCESS;
}


/* Desc:PRG-ROM flash manf/prod ID sense test
 *	Using mapper 30 defined PRG-ROM flash writes
 *	Only senses SST flash ID's
 *	Assumes that isn't getting tricked by having manf/prodID at $8000/8001
 *	could add check and increment read address to ensure doesn't get tricked..
 * Pre: nes_init() been called to setup i/o
 * Post:memory manf/prod ID set to read values if passed
 *	memory wr_dict and wr_opcode set if successful
 *	Software mode exited if entered successfully
 * Rtn: SUCCESS if flash sensed, GEN_FAIL if not, neg if error 
 */
int read_flashID_prgrom_map30( USBtransfer *transfer, memory *flash ) {

	uint8_t rv[RV_DATA0_IDX];

	//enter software mode
	//$8000-BFFF writes to flash
	//$C000-FFFF writes to mapper
	//	    15 14 13 12
	// 0x5 = 0b  0  1  0  1	-> $9555
	// 0x2 = 0b  0  0  1  0	-> $2AAA
	//set A14 in mapper reg for $5555 command
	dictionary_call( transfer, DICT_NES, 	NES_CPU_WR,	0xC000,		0x01,	
									USB_IN,		NULL,	1);
	//write $5555 0xAA
	dictionary_call( transfer, DICT_NES, 	NES_CPU_WR,	0x9555,		0xAA,	
									USB_IN,		NULL,	1);
	//clear A14 in mapper reg for $2AAA command
	dictionary_call( transfer, DICT_NES, 	NES_CPU_WR,	0xC000,		0x00,	
									USB_IN,		NULL,	1);
	//write $2AAA 0x55
	dictionary_call( transfer, DICT_NES, 	NES_CPU_WR,	0xAAAA,		0x55,	
									USB_IN,		NULL,	1);
	//set A14 in mapper reg for $5555 command
	dictionary_call( transfer, DICT_NES, 	NES_CPU_WR,	0xC000,		0x01,	
									USB_IN,		NULL,	1);
	//write $5555 0x90 for software mode
	dictionary_call( transfer, DICT_NES, 	NES_CPU_WR,	0x9555,		0x90,	
									USB_IN,		NULL,	1);

	//read manf ID
	dictionary_call( transfer, DICT_NES, 	NES_CPU_RD,			0x8000,		NILL,	
								USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("manf id: %x", rv[RV_DATA0_IDX]);
	if ( rv[RV_DATA0_IDX] != SST_MANF_ID ) {
		return GEN_FAIL;
		//no need for software exit since failed to enter
	}

	//read prod ID
	dictionary_call( transfer, DICT_NES, 	NES_CPU_RD,			0x8001,		NILL,	
								USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("prod id: %x", rv[RV_DATA0_IDX]);
	if ( (rv[RV_DATA0_IDX] == SST_PROD_128)
	||   (rv[RV_DATA0_IDX] == SST_PROD_256)
	||   (rv[RV_DATA0_IDX] == SST_PROD_512) ) {
		//found expected manf and prod ID
		flash->manf = SST_MANF_ID;
		flash->part = rv[RV_DATA0_IDX];
		flash->wr_dict = DICT_NES;
		flash->wr_opcode = NES_CPU_WR;
	}

	//exit software
	dictionary_call( transfer, DICT_NES, 	NES_CPU_WR,	0x8000,		0xF0,	
									USB_IN,		NULL,	1);

	//verify exited
	dictionary_call( transfer, DICT_NES, 	NES_CPU_RD,			0x8000,		NILL,	
								USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("prod id: %x", rv[RV_DATA0_IDX]);

	return SUCCESS;
}


/* Desc:CHR-ROM flash manf/prod ID sense test
 *	Only senses SST flash ID's
 *	Does not make CHR bank writes so A14-A13 must be made valid outside of this funciton
 *	An NROM board does this by tieing A14:13 to A12:11
 *	Other mappers will pass this function if PT0 has A14:13=01, PT1 has A14:13=10
 *	Assumes that isn't getting tricked by having manf/prodID at $0000/0001
 *	could add check and increment read address to ensure doesn't get tricked..
 * Pre: nes_init() been called to setup i/o
 * Post:memory manf/prod ID set to read values if passed
 *	memory wr_dict and wr_opcode set if successful
 *	Software mode exited if entered successfully
 * Rtn: SUCCESS if flash sensed, GEN_FAIL if not, neg if error 
 */
int read_flashID_chrrom_8K( USBtransfer *transfer, memory *flash ) {

	uint8_t rv[RV_DATA0_IDX];

	//enter software mode
	//NROM has A13 tied to A11, and A14 tied to A12.
	//So only A0-12 needs to be valid
	//A13 needs to be low to address CHR-ROM
	//	    15 14 13 12
	// 0x5 = 0b  0  1  0  1	-> $1555
	// 0x2 = 0b  0  0  1  0	-> $0AAA
	dictionary_call( transfer, DICT_NES, 	NES_PPU_WR,	0x1555,		0xAA,	
									USB_IN,		NULL,	1);
	dictionary_call( transfer, DICT_NES, 	NES_PPU_WR,	0x0AAA,		0x55,	
									USB_IN,		NULL,	1);
	dictionary_call( transfer, DICT_NES, 	NES_PPU_WR,	0x1555,		0x90,	
									USB_IN,		NULL,	1);
	//read manf ID
	dictionary_call( transfer, DICT_NES, 	NES_PPU_RD,	0x0000,		NILL,	
							USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("manf id: %x", rv[RV_DATA0_IDX]);
	if ( rv[RV_DATA0_IDX] != SST_MANF_ID ) {
		return GEN_FAIL;
		//no need for software exit since failed to enter
	}

	//read prod ID
	dictionary_call( transfer, DICT_NES, 	NES_PPU_RD,	0x0001,		NILL,	
							USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("prod id: %x", rv[RV_DATA0_IDX]);
	if ( (rv[RV_DATA0_IDX] == SST_PROD_128)
	||   (rv[RV_DATA0_IDX] == SST_PROD_256)
	||   (rv[RV_DATA0_IDX] == SST_PROD_512) ) {
		//found expected manf and prod ID
		flash->manf = SST_MANF_ID;
		flash->part = rv[RV_DATA0_IDX];
		flash->wr_dict = DICT_NES;
		flash->wr_opcode = NES_PPU_WR;
	}

	//exit software
	dictionary_call( transfer, DICT_NES, 	NES_PPU_WR,	0x0000,		0xF0,	
									USB_IN,		NULL,	1);

	//verify exited
	//dictionary_call( transfer, DICT_NES, 	NES_PPU_RD,	0x0000,		NILL,	
	//							USB_IN,		rv,	RV_DATA0_IDX+1);
	//debug("prod id: %x", rv[RV_DATA0_IDX]);

	return SUCCESS;
}

/* Desc:Simple CHR-RAM sense test
 *	A more thourough test should be implemented in firmware
 *	This one simply tests one address in PPU address space
 * Pre: nes_init() been called to setup i/o
 * Post:
 * Rtn: SUCCESS if ram sensed, GEN_FAIL if not, neg if error 
 */
int ppu_ram_sense( USBtransfer *transfer, uint16_t addr ) {

	uint8_t rv[RV_DATA0_IDX];

	//write 0xAA to addr 
	dictionary_call( transfer, DICT_NES, 	NES_PPU_WR,	addr,		0xAA,	
									USB_IN,		NULL,	1);
	//try to read it back
	dictionary_call( transfer, DICT_NES, 	NES_PPU_RD,	addr,		NILL,	
							USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("reading back 0xAA: %x", rv[RV_DATA0_IDX]);
	if ( rv[RV_DATA0_IDX] != 0xAA ) {
		return GEN_FAIL;
	} 

	//write 0x55 to addr 
	dictionary_call( transfer, DICT_NES, 	NES_PPU_WR,	addr,		0x55,	
									USB_IN,		NULL,	1);
	//try to read it back
	dictionary_call( transfer, DICT_NES, 	NES_PPU_RD,	addr,		NILL,	
							USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("reading back 0x55: %x", rv[RV_DATA0_IDX]);
	if ( rv[RV_DATA0_IDX] != 0x55 ) {
		return GEN_FAIL;
	}

	return SUCCESS;
}


/* Desc:Just calls CIRAM_A10_MIRROR opcode and returns result.
 *	result will be return value of opcode
 * Pre: nes_init() been called to setup i/o
 * Post:address bus left assigned
 * Rtn: VERT/HORIZ/1SCNA/1SCNB
 */
int ciram_A10_mirroring( USBtransfer *transfer )
{
	uint8_t rv[RV_DATA0_IDX];

	dictionary_call( transfer, DICT_NES, 	CIRAM_A10_MIRROR,	NILL,		NILL,	
							USB_IN,		rv,	RV_DATA0_IDX+1);
	debug("mirroring detected: %x", rv[RV_DATA0_IDX]);
	return rv[RV_DATA0_IDX];
}
