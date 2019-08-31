#include "flash.h"

//TODO replace all use of this function with write_page_verify below
uint8_t	write_page( uint8_t addrH, buffer *buff, write_funcptr wr_func )
{
	uint16_t cur = buff->cur_byte;
	uint8_t	n = buff->cur_byte;
//	uint8_t read;

	while ( cur <= buff->last_idx ) {

		wr_func( ((addrH<<8)| n), buff->data[n] );
		//write function returns when it's complete or errors out
	
    		n++;
    		cur++;
	}
	buff->cur_byte = n;

	//TODO error check/report
	return SUCCESS;
} 

uint8_t	write_page_verify( uint8_t addrH, buffer *buff, write_rv_funcptr wr_func )
{
	uint16_t cur = buff->cur_byte;
	uint8_t	n = buff->cur_byte;
	uint8_t read;

	while ( cur <= buff->last_idx ) {

		//only works for NROM right now..
		read = wr_func( ((addrH<<8)| n), buff->data[n] );
		//write function returns read back data post flash attempt
	
		//TODO create flag/static variable to determine what behavior to have
		//if write fails
		if ( read != buff->data[n] ) {
			LED_OP();
			LED_HI();
		} 
		else { //next byte
			LED_IP_PU();	
			//LED_LO();
			//if there's a WDT the device should reset if get stuck here
    			n++;
			cur++;
		}
		//put this increment only in pass case if want to retry
		//n++;
    		//cur++;
	}
	buff->cur_byte = n;

	//TODO error check/report
	return SUCCESS;
} 


//only used by cninja currently..
uint8_t	write_page_cninja( uint8_t bank, uint8_t addrH, uint16_t unlock1, uint16_t unlock2, buffer *buff, write_funcptr wr_func, read_funcptr rd_func )
{
	uint16_t cur = buff->cur_byte;
	uint8_t	n = buff->cur_byte;
	uint8_t read;
	while ( cur <= buff->last_idx ) {
		//write unlock sequence
		wr_func( unlock1, 0xAA );
		wr_func( unlock2, 0x55 );
		wr_func( unlock1, 0xA0 );
		wr_func( ((addrH<<8)| n), buff->data[n] );
		do {
			usbPoll();
			read = rd_func((addrH<<8)|n);
		} while( read != rd_func((addrH<<8)|n) );
	}
	buff->cur_byte = n;
	return SUCCESS;
} 

//only used by MM2 currently
uint8_t	write_page_mm2( uint8_t bank, uint8_t addrH, uint16_t unlock1, uint16_t unlock2, buffer *buff, write_funcptr wr_func, read_funcptr rd_func )
{
	uint16_t cur = buff->cur_byte;
	uint8_t	n = buff->cur_byte;
	uint8_t read;
	while ( cur <= buff->last_idx ) {
		nes_cpu_wr( (0xFD69), 0x00 );
		wr_func( unlock1, 0xAA );
		wr_func( unlock2, 0x55 );
		wr_func( unlock1, 0xA0 );
		nes_cpu_wr( (0xFD69+bank), bank );
		wr_func( ((addrH<<8)| n), buff->data[n] );
		do {
			usbPoll();
			read = rd_func((addrH<<8)|n);
		} while( read != rd_func((addrH<<8)|n) );
		if (read == buff->data[n]) {
	    		n++;
	    		cur++;
			LED_IP_PU();	
			LED_LO();
		} else {
			LED_OP();
			LED_HI();
		}
	}
	buff->cur_byte = n;
	return SUCCESS;
} 


uint8_t	write_page_a53( uint8_t bank, uint8_t addrH, buffer *buff, write_funcptr wr_func, read_funcptr rd_func )
{
	uint16_t cur = buff->cur_byte;
	uint8_t	n = buff->cur_byte;
	uint8_t read;
	//enter unlock bypass mode
	wr_func( 0x8AAA, 0xAA );
	wr_func( 0x8555, 0x55 );
	wr_func( 0x8AAA, 0x20 );
	while ( cur <= buff->last_idx ) {
		//TODO FIX THIS!  It shouldn't be needed!
		//but for some reason the mapper is loosing it's setting for $5000 register to 
		//permit flash writes.  Many writes go through, but at somepoint it gets lost..
		//maybe the best fix it to require address to be equal to $5555 to write to flash enable register..
		//but for now, this rewrite hack solves the issue.
		nes_cpu_wr(0x5000, 0x54); //chr reg select act like CNROM & enable flash writes
		//AVR didn't need this patch so maybe is a speed issue
		//stmadapter didn't have problems either..
		//added time delay before m2 rising edge and it didn't change anything for stm6
	//	curaddresswrite( 0xA0 );	//gained ~3KBps (59.13KBps) inl6 with v3.0 proto
		wr_func( ((addrH<<8)| n), 0xA0  );
		wr_func( ((addrH<<8)| n), buff->data[n] );
		do {
			usbPoll();
			read = rd_func((addrH<<8)|n);
		} while( read != rd_func((addrH<<8)|n) );
		//retry if write failed
		//this helped but still seeing similar fails to dumps
		if (read == buff->data[n]) {
      		n++;
        		cur++;
			LED_IP_PU();	
			LED_LO();
		} else {
			//kaz6 final needs a retry, but proto doesn't...
			nes_cpu_wr(0x5000, 0x81); //outer reg select mode
			nes_cpu_wr(0x8000, bank);	  //outer bank
			nes_cpu_wr(0x5000, 0x54); //chr reg select act like CNROM & enable flash writes
			LED_OP();
			LED_HI();
		}
	}
	buff->cur_byte = n;
	//exit unlock bypass mode
	wr_func( 0x8000, 0x90 );
	wr_func( 0x8000, 0x00 );
	//reset the flash chip, supposed to exit too
	wr_func( 0x8000, 0xF0 );
	return SUCCESS;
} 

uint8_t	write_page_tssop( uint8_t bank, uint8_t addrH, buffer *buff, write_funcptr wr_func, read_funcptr rd_func )
{
	uint16_t cur = buff->cur_byte;
	uint8_t	n = buff->cur_byte;
	uint8_t read;
	//enter unlock bypass mode
	wr_func( 0x8AAA, 0xAA );
	wr_func( 0x8555, 0x55 );
	wr_func( 0x8AAA, 0x20 );
	while ( cur <= buff->last_idx ) {
	//	curaddresswrite( 0xA0 );	//gained ~3KBps (59.13KBps) inl6 with v3.0 proto
		wr_func( ((addrH<<8)| n), 0xA0  );
		wr_func( ((addrH<<8)| n), buff->data[n] );
		do {
			usbPoll();
			read = rd_func((addrH<<8)|n);
		} while( read != rd_func((addrH<<8)|n) );
		//retry if write failed
		//this helped but still seeing similar fails to dumps
		if (read == buff->data[n]) {
      		n++;
        		cur++;
			LED_IP_PU();	
			LED_LO();
		} else {
			//kaz6 final needs a retry, but proto doesn't...
		//	nes_cpu_wr(0x5000, 0x81); //outer reg select mode
		//	nes_cpu_wr(0x8000, bank);	  //outer bank
		//	nes_cpu_wr(0x5000, 0x54); //chr reg select act like CNROM & enable flash writes
			LED_OP();
			LED_HI();
		}
	}
	buff->cur_byte = n;
	//exit unlock bypass mode
	wr_func( 0x8000, 0x90 );
	wr_func( 0x8000, 0x00 );
	//reset the flash chip, supposed to exit too
	wr_func( 0x8000, 0xF0 );
	return SUCCESS;
} 


uint8_t	write_page_dualport( uint8_t bank, uint8_t addrH, buffer *buff, write_funcptr wr_func, read_funcptr rd_func )
{
	uint16_t cur = buff->cur_byte;
	uint8_t	n = buff->cur_byte;
	uint8_t read;
	//enter unlock bypass mode
	wr_func( 0x0AAA, 0xAA );
	wr_func( 0x0555, 0x55 );
	wr_func( 0x0AAA, 0x20 );
	while ( cur <= buff->last_idx ) {
		wr_func( ((addrH<<8)| n), 0xA0  );
		wr_func( ((addrH<<8)| n), buff->data[n] );
		do {
			usbPoll();
			read = rd_func((addrH<<8)|n);
		} while( read != rd_func((addrH<<8)|n) );
		//TODO verify byte is value that was trying to be flashed
		//move on to next byte
		//n++;
		//cur++;
		if (read == buff->data[n]) {
			n++;
			cur++;
			LED_IP_PU();	
			LED_LO();
		} else {
			LED_OP();
			LED_HI();
		}
	}
	buff->cur_byte = n;
	//exit unlock bypass mode
	wr_func( 0x0000, 0x90 );
	wr_func( 0x0000, 0x00 );
	//reset the flash chip, supposed to exit too
	wr_func( 0x0000, 0xF0 );
	return SUCCESS;
} 

//#define PRGM_MODE() swim_wotf(SWIM_HS, 0x500F, 0x40)
//#define PLAY_MODE() swim_wotf(SWIM_HS, 0x500F, 0x00)
//#define PRGM_MODE() EXP0_LO()
//#define PLAY_MODE() EXP0_HI()
#define PRGM_MODE() NOP()
#define PLAY_MODE() NOP()

uint8_t	write_page_snes( uint8_t bank, uint8_t addrH, buffer *buff, write_snes_funcptr wr_func, read_snes_funcptr rd_func )
{
	uint16_t cur = buff->cur_byte;
	uint8_t	n = buff->cur_byte;
	uint8_t read;
#ifdef AVR_CORE
		wdt_reset();	
#endif
	//set to program mode for first entry
	//EXP0_LO();
	//swim_wotf(SWIM_HS, 0x500F, 0x40)
	PRGM_MODE();

//; TODO I don't think all these NOPs are actually needed, but they work and don't seem to significantly affect program time on stm32
	NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();
	//enter unlock bypass mode
	wr_func( 0x8AAA, 0xAA, 0 );
	wr_func( 0x8555, 0x55, 0 );
	wr_func( 0x8AAA, 0x20, 0 );
	while ( cur <= buff->last_idx ) {
		//write unlock sequence
		//unlocked	wr_func( 0x0AAA, 0xAA );
		//unlocked	wr_func( 0x0555, 0x55 );
		//wr_func( 0x0000, 0xA0 );
		snes_wr_cur_addr( 0xA0, 0 );	//gained ~3KBps (59.13KBps) inl6 with v3.0 proto
		wr_func( ((addrH<<8)| n), buff->data[n], 0 );
		//wr_func( ((addrH<<8)| n), cur_data );  //didn't actually speed up
		//Targetting 2MByte 16mbit flash which doesn't have buffered writes
		//currently have average flash speed of 21.05KBps going to start removing some of these NOPs
		//and optimizing flash routine to get time down.
		//exit program mode
	//	EXP0_HI();
		PLAY_MODE();
	NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();
		//pre-fetch next byte of data
		//cur_data = buff->data[n+1];
#ifdef AVR_CORE
		wdt_reset();	
#endif
		//wait for byte to flash
	//	do {
	//		usbPoll();
	//		read = rd_func((addrH<<8)|n);
	//
	//	//} while( read != rd_func((addrH<<8)|n) );
	//	} while( read != buff->data[n] );
		//this can cause things to hang on failed programs..
		//need a smarter flash polling algo, kind of a pain because we don't have
		//a good way to toggle /OE or /CE quickly on v3 SNES boards
		usbPoll();
		read = rd_func((addrH<<8)|n, 0);
		//prepare for upcoming write cycle, or allow for a polling read
		//EXP0_LO();
		PRGM_MODE();
	NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();
		//First check if already outputting final data
		if (read != buff->data[n] )  {
			//if not, lets see if toggle is occuring
			//EXP0_HI();
			PLAY_MODE();
	NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();
			while( read != rd_func((addrH<<8)|n, 0) ){
				//EXP0_LO();
				PRGM_MODE();
				NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();
	NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();
				//EXP0_HI();
				PLAY_MODE();
	NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();
				read = rd_func((addrH<<8)|n, 0);
			}
			//prepare for upcoming write cycle
			//EXP0_LO();
			PRGM_MODE();
	NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP();
		}
//		//IDK why, but AVR will exit early sometimes
//		//without this second check, ~20 errors per 32KByte on SNES v3.0
//		//All error bytes are 0xFF instead of true data
//		//may need a smarter flash polling routine..
//		//Tried to add extra delay to read algo, and didn't change anything
//		//Also have decent trust in read routine as it's comparable to page read
//		//which works flawlessly for dumps.  So think it has to do with flashing specifically...
//		//Hmm maybe the avr is missing a read..  flash /CE, /OE, and /WE never toggle
//		//so why would flash polling output different data between polls..?
//		//Ahh this is the issue, adding the code below only adds delay which gives flash
//		//enough time to complete write.

		//retry if write failed
		//this helped but still seeing similar fails to dumps
		n++;
		cur++;
//		if (read == buff->data[n]) {
//			//n++;
//			//cur++;
//			LED_IP_PU();	
//			LED_LO();
//		} else {
//			LED_OP();
//			LED_HI();
//		}

	}

	buff->cur_byte = n;

	//exit unlock bypass mode
	wr_func( 0x8000, 0x90, 0 );
	wr_func( 0x8000, 0x00, 0 );
	//reset the flash chip, supposed to exit too
	wr_func( 0x8000, 0xF0, 0 );

	//exit program mode
	//EXP0_HI();
	PLAY_MODE();
	return SUCCESS;
} 

/* Desc:Flash buffer contents on to cartridge memory
 * Pre: buffer elements must be updated to designate how/where to flash
 * 	buffer's cur_byte must be cleared or set to where to start flashing
 *	mapper registers must be initialized
 * Post:buffer page flashed/programmed to memory.
 * Rtn: SUCCESS or ERROR# depending on if there were errors.
 */
uint8_t flash_buff( buffer *buff ) {

	uint8_t addrH = buff->page_num;	//A15:8  while accessing page
	uint8_t	bank;

	switch ( buff->mem_type ) {
		#ifdef NES_CONN
		case PRGROM:		//$8000

			//Latest method used here!
			//leave the host responsible for init & banking
			//we just need to call a page write algo and give it mmc3_prgrom_flash_wr function
			//think of this only as an 8KB ROM
			//ie MMC3 flash writes are always $8000-9FFF, but the host arranges this
			
			if (buff->mapper == NROM) {
				//used by other 32KB PRG bank discrete mappers like BNROM, CNROM, & color dreams
				write_page_verify( (0x80+addrH), buff, nrom_prgrom_flash_wr);
			}
			if (buff->mapper == MMC1) {
				write_page( (0x80+addrH), buff, mmc1_prgrom_flash_wr);
			}
			if (buff->mapper == UxROM) {
				write_page( (0x80+addrH), buff, unrom_prgrom_flash_wr);
			}
			if (buff->mapper == MMC3) {
				write_page_verify( (0x80+addrH), buff, mmc3_prgrom_flash_wr);
			}
			if (buff->mapper == MMC3S) {
				write_page_verify( (0x80+addrH), buff, mmc3s_prgrom_flash_wr);
			}
			//SOP-44
			/*
			if (buff->mapper == MMC4) {
				write_page( (0x80+addrH), buff, mmc4_prgrom_sop_flash_wr);
			}
			*/
			//TODO use mapper variant to differentiate between the two
			//PLCC-32
			if (buff->mapper == MMC4) {
				write_page_verify( (0x80+addrH), buff, mmc4_prgrom_flash_wr);
			}
			if (buff->mapper == MM2) {
				//addrH &= 0b1011 1111 A14 must always be low
				addrH &= 0x3F;
				addrH |= 0x80;	//A15 doesn't apply to exp0 write, but needed for read back
				//write bank value
				//page_num shift by 6 bits A14 >> A8(0)
				bank = buff->page_num >> 6;
				//bank gets written inside flash algo
				write_page_mm2( bank, addrH, 0x5555, 0x2AAA, buff, disc_push_exp0_prgrom_wr, nes_cpu_rd );
			}
			if (buff->mapper == MAP30) {
				write_page_verify( (0x80+addrH), buff, map30_prgrom_flash_wr);
			}
			if (buff->mapper == CNINJA) {
				//addrH &= 0b1001 1111 A14-13 must always be low
				addrH &= 0x1F;
				addrH |= 0x80;
				//write bank value
				//page_num shift by 5 bits A13 >> A8(0)
				bank = buff->page_num >> 5;
				nes_cpu_wr( (0x6000), 0xA5 );	//select desired bank
				nes_cpu_wr( (0xFFFF), bank );	//select desired bank
				write_page_cninja( 0, addrH, 0xD555, 0xAAAA, buff, nes_cpu_wr, nes_cpu_rd );
			}
			if (buff->mapper == A53) {

				//enter unlock bypass mode
				nes_m2_high_wr( 0x8AAA, 0xAA );
				nes_m2_high_wr( 0x8555, 0x55 );
				nes_m2_high_wr( 0x8AAA, 0x20 );

				write_page_verify( (0x80+addrH), buff, a53_tssop_prgrom_flash_wr);

				//exit unlock bypass mode
				nes_m2_high_wr( 0x8000, 0x90 );
				nes_m2_high_wr( 0x8000, 0x00 );
				//reset the flash chip, supposed to exit too
				nes_m2_high_wr( 0x8000, 0xF0 );

				//////////////
				// OLD WAY, still used by PLCC flash
				// need to uncomment and reflash firmware to get it to work
				// Long term solution is to have a mapper variant for each
				// or PRG-ROM part number type variable..?
				//////////////
				/*
				
				//write bank value to bank table
				//page_num shift by 7 bits A15 >> A8(0)
				bank = (buff->page_num)>>7;
				//Setup as CNROM, then scroll through outer banks.
				//cpu_wr(0x5000, 0x80); //reg select mode
				//   xxSSPPMM   SS-size: 0-32KB, PP-prg mode: 0,1 32KB, MM-mirror
				//cpu_wr(0x8000, 0b00000000);   //reg value 256KB inner, 32KB banks
				nes_cpu_wr(0x5000, 0x81); //outer reg select mode
				nes_cpu_wr(0x8000, bank);	  //outer bank
				nes_cpu_wr(0x5000, 0x54); //chr reg select act like CNROM & enable flash writes

				//need to use standard CPU writes
				//write_page_old( 0, (0x80 | addrH), buff, nes_cpu_wr, nes_cpu_rd );
				//break;
				//WORKS PLCC Action53:
				//had problems later not all bytes getting programmed..
				write_page_old( bank, (0x80 | addrH), 0xD555, 0xAAAA, buff, nes_cpu_wr, nes_cpu_rd );
				//TSSOP-28 action53:
				//write_page_a53( bank, (0x80 | addrH), buff, nes_m2_high_wr, nes_cpu_rd );
				//write_page_verify( (0x80+addrH), buff, mmc3_prgrom_flash_wr);
				*/
			}
			if (buff->mapper == EZNSF) {
				/*
				//addrH &= 0b1000 1111 A14-12 must always be low
				addrH &= 0x8F;
				//write bank value to bank table
				//page_num shift by 4 bits A12 >> A8(0)
				bank = (buff->page_num)>>4;
				nes_cpu_wr(0x5000, bank);	  //bank @ $8000-8FFF

				write_page_tssop( bank, (0x80 | addrH), buff, nes_cpu_wr, nes_cpu_rd );
				*/
				//enter unlock bypass mode
				nes_m2_high_wr( 0x9AAA, 0xAA );
				nes_m2_high_wr( 0x9555, 0x55 );
				nes_m2_high_wr( 0x9AAA, 0x20 );

				write_page_verify( (0x90+addrH), buff, tssop_prgrom_flash_wr);

				//exit unlock bypass mode
				nes_m2_high_wr( 0x9000, 0x90 );
				nes_m2_high_wr( 0x9000, 0x00 );
				//reset the flash chip, supposed to exit too
				nes_m2_high_wr( 0x9000, 0xF0 );
			}
			if (buff->mapper == GTROM) {
				write_page_verify( (0x80+addrH), buff, gtrom_prgrom_flash_wr);
			}
			break;

		case CHRROM:		//$0000
			if (buff->mapper == NROM) {
				write_page( addrH, buff, nrom_chrrom_flash_wr);
			}
			if (buff->mapper == MMC1) {
				write_page( addrH, buff, mmc1_chrrom_flash_wr);
			}
			if (buff->mapper == CNROM) {
				write_page( addrH, buff, cnrom_chrrom_flash_wr);
			}
			if (buff->mapper == MMC3) {
				write_page( addrH, buff, mmc3_chrrom_flash_wr);
			}
			if (buff->mapper == MMC4) {
				write_page( addrH, buff, mmc4_chrrom_flash_wr);
			}
			if (buff->mapper == CDREAM) {
				write_page( addrH, buff, cdream_chrrom_flash_wr);
			}
			if (buff->mapper == DPROM) {
				//select bank
				//8KB banks $0000-1FFF
				//page_num shift by 5 bits A13 >> A8(0)
				bank = (buff->page_num)>>5;
				//write bank to register
				nes_ppu_wr(0x3FFF, bank);
				addrH &= 0x1F;	//only A12-8 are directly addressable
				write_page_dualport( 0, addrH, buff, nes_dualport_wr, nes_dualport_rd );
			}
			break;

		case PRGRAM:
			write_page( addrH+0x60, buff, nes_cpu_wr);
			break;
		#endif

		#ifdef SNES_CONN
		case SNESROM:
			if (buff->mapper == LOROM_5VOLT) {
				//LOROM banks start at $XX:8000
				write_page( addrH+0x80, buff, snes_5v_flash_wr);
			}
			if (buff->mapper == HIROM_5VOLT) {
				//HIROM banks start at $XX:0000
				write_page( addrH, buff, snes_5v_flash_wr);
			}
			if (buff->mapper == LOROM_3VOLT) {
				//LOROM banks start at $XX:8000
				write_page( addrH+0x80, buff, snes_3v_flash_wr);
			}
			if (buff->mapper == HIROM_3VOLT) {
				//HIROM banks start at $XX:0000
				write_page( addrH, buff, snes_3v_flash_wr);
			}

			if (buff->mapper == LOROM) {
				addrH |= 0x80;	//$8000 LOROM space
				//need to split page_num
				//A14-8 page_num[7-0]
				//A15 high (LOROM)
				//A23-16 page_num[14-8]
				bank = (buff->page_num)>>7;
				//clear any reset state
				//EXP0_HI();
			HADDR_SET( bank );
			write_page_snes( 0, addrH, buff, snes_wr, snes_rd );
			}
			if (buff->mapper == HIROM) {
				//need to split page_num
				//A15-8 page_num[7-0]
				//A21-16 page_num[13-8]
				//A22 high (HIROM)
				//A23 ~page_num[14] (bank CO starts first half, bank 40 starts second)
				bank = ((((buff->page_num)>>8) | 0x40) & 0x7F);
			HADDR_SET( bank );
			write_page_snes( 0, addrH, buff, snes_wr, snes_rd );
			}
		case SNESRAM:
//warn			addrX = ((buff->page_num)>>8);
			break;
		#endif

		default:
			return ERR_BUFF_UNSUP_MEM_TYPE;
	}


	return SUCCESS;
}

