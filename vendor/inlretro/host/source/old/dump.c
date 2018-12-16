#include "dump.h"

/* Desc:
 * Pre: 
 * Post:
 * Rtn: 
 */
//main collected as much data about cart as possible without reading roms
//now it's time to start running CRC's to try and finalize mapper/config
//Once final mapper is known store header data in rom file and start dumping!
int dump_cart( USBtransfer* transfer, rom_image *rom, cartridge *cart )
{
	int num_buffers = 2;
	int buff_size = 128;	
	int buff0 = 0;
	int buff1 = 1;
	int i;
	int cur_buff_status = 0;
	uint8_t data[buff_size];


	debug("dumping cart");

	//TODO provide user arg to force all these checks passed
	//first check if any provided args differ from what was detected
	check( (cart->console != UNKNOWN), "cartridge not detected, must provide console if autodetection is off");

	if ( rom->console != UNKNOWN ) {
		check( rom->console == cart->console, 
			"request system dump doesn't match detected cartridge");
	}
	if ( (cart->mapper != UNKNOWN) && (rom->mapper != UNKNOWN) ) {
		check( rom->mapper == cart->mapper,	
			"request mapper dump doesn't match detected mapper");
	}

	//start with reset and init
	io_reset( transfer );
	nes_init( transfer );
	//Run some CRC's to determine size of memories

	//setup buffers and manager
	//reset buffers first
	check(! reset_buffers( transfer ), "Unable to reset device buffers");
	//need to allocate some buffers for dumping
	//2x 128Byte buffers
	check(! allocate_buffers( transfer, num_buffers, buff_size ), "Unable to allocate buffers");

	//set buffer elements as needed
	//set reload which gets added to page_num after each buffer read
	//set reload to 256 = 1 when translated to page_num (done in allocate buffers funct)
	//set page_num to non-zero if offset arg sent
	//set mem_type and part_num to designate how to get/write data
	check(! set_mem_n_part( transfer, buff0, PRGROM, MASKROM ), "Unable to set mem_type and part");
	check(! set_mem_n_part( transfer, buff1, PRGROM, MASKROM ), "Unable to set mem_type and part");
	//set multiple and add_mult only when flashing
	//set mapper, map_var, and function to designate read/write algo

	//just dump visible NROM memory to start
	check(! set_map_n_mapvar( transfer, buff0, NROM, NILL ), "Unable to set mapper and map_var");
	check(! set_map_n_mapvar( transfer, buff1, NROM, NILL ), "Unable to set mapper and map_var");

	//tell buffers what function to use for dumping
	//TODO when start implementing other mappers
	dictionary_call_debug( transfer, DICT_NES, 	NES_CPU_RD,			0x8000,		NILL,	
								USB_IN,		NULL,	RV_DATA0_IDX+1);

	//debugging print out buffer elements
	//get_operation( transfer );
	//get_buff_elements( transfer, buff0 );
	//get_buff_elements( transfer, buff1 );

	debug("\n\nsetting operation STARTDUMP");
	//inform buffer manager to start dumping operation now that buffers are initialized
	check(! set_operation( transfer, STARTDUMP ), "Unable to set buffer operation");

//	get_operation( transfer );
//	get_buff_elements( transfer, buff0 );
//	get_buff_elements( transfer, buff1 );
	//manager updates buffer status' so they'll start dumping
	//once they're full manager prepares them to be read back on USB payloads
	//once the next payload request happens manager knows last buffer can start dumping again
	//buffer updates it's elements and goes off to dump next page

//	debug("first payload");
//	check(! payload_in( transfer, data, buff_size ), "Error with payload IN");
//	check(! append_to_file( rom, data, buff_size ), "Error with file append");
//
//	debug("first payload done");
//	get_operation( transfer );
//	get_buff_elements( transfer, buff0 );
//	get_buff_elements( transfer, buff1 );
//
//	debug("second payload");
//	check(! payload_in( transfer, data, buff_size ), "Error with payload IN");
//	check(! append_to_file( rom, data, buff_size ), "Error with file append");
//
//	get_operation( transfer );
//	get_buff_elements( transfer, buff0 );
//	get_buff_elements( transfer, buff1 );
	clock_t tstart, tstop;
	tstart = clock();

	//now just need to call series of payload IN transfers to retrieve data
	//for( i=0; i<(512*KByte/buff_size); i++) {
	for( i=0; i<(32*KByte/buff_size); i++) {
		//ensure cur_buff is DUMPED prior to requsting data
		check(! get_cur_buff_status( transfer, &cur_buff_status ), "Error retrieving cur_buff->status");
		while (cur_buff_status != DUMPED ) {
			//debug("cur_buff->status: %x ", cur_buff_status);
			check(! get_cur_buff_status( transfer, &cur_buff_status ), "Error retrieving cur_buff->status");
		}
	//for( i=0; i<(8*KByte/buff_size); i++) {
		//payload transfer in and append to file
	//	if ( i % 256 == 0 ) debug("payload in #%d", i);
		check(! payload_in( transfer, data, buff_size ), "Error with payload IN");
		if (i==0) printf("first byte: %x\n", data[0]);
		check(! append_to_file( rom, data, buff_size ), "Error with file append");
	}
	debug("payload done");

	tstop = clock();
	float timediff = ( (float)(tstop-tstart) / CLOCKS_PER_SEC);
	printf("total time: %fsec, speed: %fKBps", timediff, (512/timediff));
	//TODO flush file from time to time..?


	//tell buffer manager when to stop
	// or not..?  just reset buffers and start next memory or quit
	//reset buffers and setup to dump CHR-ROM

	check(! reset_buffers( transfer ), "Unable to reset device buffers");
	check(! allocate_buffers( transfer, num_buffers, buff_size ), "Unable to allocate buffers");
	check(! set_mem_n_part( transfer, buff0, CHRROM, MASKROM ), "Unable to set mem_type and part");
	check(! set_mem_n_part( transfer, buff1, CHRROM, MASKROM ), "Unable to set mem_type and part");
	check(! set_map_n_mapvar( transfer, buff0, NROM, NILL ), "Unable to set mapper and map_var");
	check(! set_map_n_mapvar( transfer, buff1, NROM, NILL ), "Unable to set mapper and map_var");

	debug("\n\nsetting operation STARTDUMP");
	check(! set_operation( transfer, STARTDUMP ), "Unable to set buffer operation");

	for( i=0; i<(8*KByte/buff_size); i++) {
		//ensure cur_buff is DUMPED prior to requsting data
		check(! get_cur_buff_status( transfer, &cur_buff_status ), "Error retrieving cur_buff->status");
		while (cur_buff_status != DUMPED ) {
			//debug("cur_buff->status: %x ", cur_buff_status);
			check(! get_cur_buff_status( transfer, &cur_buff_status ), "Error retrieving cur_buff->status");
		}
		//payload transfer in and append to file
		if ( i % 256 == 0 ) debug("payload in #%d", i);
		check(! payload_in( transfer, data, buff_size ), "Error with payload IN");
		if (i==0) printf("first byte: %x\n", data[0]);
		check(! append_to_file( rom, data, buff_size ), "Error with file append");
	}
	debug("payload done");

	//close file in main

	//reset io at end
	io_reset( transfer );

	return SUCCESS;
error:
	return ~SUCCESS;
}
