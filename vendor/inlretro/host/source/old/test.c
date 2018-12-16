#include "test.h"

int test_function( cartridge *cart, USBtransfer *transfer ) 
{
	debug("testing");
	detect_console( cart, transfer );
	dictionary_call( transfer,	DICT_IO,	IO_RESET,		0,   0,   USB_IN,
										NULL,			1);
	dictionary_call( transfer,	DICT_IO,	NES_INIT,		0,   0,   USB_IN,
										NULL,			1);
	debug("io reset and nes init'd");

	/*
	dictionary_call( transfer,	DICT_PINPORT,	PRGRW_RD,		0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	M2_LO,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ADDR16_SET,		0x00AA,	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	M2_HI,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ROMSEL_LO,		0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call_debug( transfer,DICT_PINPORT,	DATA_RD,		0,   	0,   USB_IN, 	NULL,	2);
	dictionary_call( transfer,	DICT_PINPORT,	M2_LO,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ROMSEL_HI,		0,   	0,   USB_IN, 	NULL,	1);

	dictionary_call( transfer,	DICT_PINPORT,	PRGRW_RD,		0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	M2_LO,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ADDR16_SET,		0x0055,	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	M2_HI,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ROMSEL_LO,		0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call_debug( transfer,DICT_PINPORT,	DATA_RD,		0,   	0,   USB_IN, 	NULL,	2);
	dictionary_call( transfer,	DICT_PINPORT,	M2_LO,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ROMSEL_HI,		0,   	0,   USB_IN, 	NULL,	1);
	*/

	//spansion/cypress A18-11 are don't care, that translates to A19-12 for byte mode I think
	//$AAA / AA 
	dictionary_call( transfer,	DICT_NES,	NES_CPU_WR,			0x8AAA,		0xAA,
										USB_IN,		NULL,	1);
	//$555 / 55
	dictionary_call( transfer,	DICT_NES,	NES_CPU_WR,			0x8555,		0x55,
										USB_IN,		NULL,	1);
	//$AAA / 90 manf ID
	dictionary_call( transfer,	DICT_NES,	NES_CPU_WR,			0x8AAA,		0x90,
										USB_IN,		NULL,	1);


	dictionary_call_debug( transfer,	DICT_NES,	EMULATE_NES_CPU_RD,			0x8000,		0,
										USB_IN,		NULL,	2);
	dictionary_call_debug( transfer,	DICT_NES,	EMULATE_NES_CPU_RD,			0x8002,		0,
										USB_IN,		NULL,	2);

	/*
	dictionary_call_debug( transfer,	DICT_NES,	NES_PPU_RD,			0x0000,		0,
										USB_IN,		NULL,	2);
	dictionary_call_debug( transfer,	DICT_NES,	NES_PPU_RD,			0x0055,		0,
										USB_IN,		NULL,	2);
	dictionary_call_debug( transfer,	DICT_NES,	NES_PPU_RD,			0x00AA,		0,
										USB_IN,		NULL,	2);
										*/
	//RESET write F0 anywhere
	dictionary_call( transfer,	DICT_NES,	NES_CPU_WR,			0x8000,		0xF0,
										USB_IN,		NULL,	1);

	dictionary_call_debug( transfer,	DICT_NES,	EMULATE_NES_CPU_RD,			0x8000,		0,
										USB_IN,		NULL,	2);
	dictionary_call_debug( transfer,	DICT_NES,	EMULATE_NES_CPU_RD,			0x8002,		0,
										USB_IN,		NULL,	2);
	//Read what we're looking to flash
	dictionary_call_debug( transfer,	DICT_NES,	EMULATE_NES_CPU_RD,			0x80AA,		0,
										USB_IN,		NULL,	2);
	dictionary_call_debug( transfer,	DICT_NES,	EMULATE_NES_CPU_RD,			0x8055,		0,
										USB_IN,		NULL,	2);
	dictionary_call_debug( transfer,	DICT_NES,	NES_CPU_RD,			0x80AA,		0,
										USB_IN,		NULL,	2);
	dictionary_call_debug( transfer,	DICT_NES,	NES_PPU_RD,			0x0055,		0,
										USB_IN,		NULL,	2);
	dictionary_call_debug( transfer,	DICT_NES,	NES_CPU_RD,			0x80AA,		0,
										USB_IN,		NULL,	2);
	dictionary_call_debug( transfer,	DICT_NES,	NES_PPU_RD,			0x00AA,		0,
										USB_IN,		NULL,	2);

/*
	//$AAA / AA 
	dictionary_call( transfer,	DICT_NES,	NES_CPU_WR,			0x8AAA,		0xAA,
										USB_IN,		NULL,	1);
	//$555 / 55
	dictionary_call( transfer,	DICT_NES,	NES_CPU_WR,			0x8555,		0x55,
										USB_IN,		NULL,	1);
	//$AAA / A0 program byte
	dictionary_call( transfer,	DICT_NES,	NES_CPU_WR,			0x8AAA,		0xA0,
										USB_IN,		NULL,	1);
	dictionary_call( transfer,	DICT_NES,	NES_CPU_WR,			0x8055,		0xAA,
										USB_IN,		NULL,	1);
*/

	/*
	dictionary_call( transfer,	DICT_PINPORT,	PRGRW_RD,		0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	M2_LO,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ADDR16_SET,		0x00AA,	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	M2_HI,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ROMSEL_LO,		0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call_debug( transfer,DICT_PINPORT,	DATA_RD,		0,   	0,   USB_IN, 	NULL,	2);
	dictionary_call( transfer,	DICT_PINPORT,	M2_LO,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ROMSEL_HI,		0,   	0,   USB_IN, 	NULL,	1);

	dictionary_call( transfer,	DICT_PINPORT,	PRGRW_RD,		0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	M2_LO,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ADDR16_SET,		0x0055,	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	M2_HI,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ROMSEL_LO,		0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call_debug( transfer,DICT_PINPORT,	DATA_RD,		0,   	0,   USB_IN, 	NULL,	2);
	dictionary_call( transfer,	DICT_PINPORT,	M2_LO,			0,   	0,   USB_IN, 	NULL,	1);
	dictionary_call( transfer,	DICT_PINPORT,	ROMSEL_HI,		0,   	0,   USB_IN, 	NULL,	1);
	*/
/*
	dictionary_call_debug( transfer,	DICT_IO,	IO_RESET,		0,   0,   USB_IN,
										NULL,			1);
	dictionary_call_debug( transfer,	DICT_IO,	NES_INIT,		0,   0,   USB_IN,
										NULL,			1);
	debug("\nreset");
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BUFFER_RESET,	0,   0,   USB_IN,
										NULL,			1);

	debug("\nallocate 0");							// id:base, numbanks
	dictionary_call_debug( transfer,	DICT_BUFFER,	ALLOCATE_BUFFER0,  0x0E00,   4,   USB_IN,
										NULL,			1);
	debug("\nallocate 1");							// id:base, numbanks
	dictionary_call_debug( transfer,	DICT_BUFFER,	ALLOCATE_BUFFER1,  0x8104,   4,   USB_IN,
										NULL,			1);
	debug("\nallocate 2");							// id:base, numbanks
	dictionary_call_debug( transfer,	DICT_BUFFER,	ALLOCATE_BUFFER2,  0x2208,   6,   USB_IN,
										NULL,			1);
	debug("\nallocate 3");							// id:base, numbanks
	dictionary_call_debug( transfer,	DICT_BUFFER,	ALLOCATE_BUFFER3,  0x330E,   2,   USB_IN,
										NULL,			1);
	debug("\nmapvar 2: 21,22");						
	dictionary_call_debug( transfer,	DICT_BUFFER,	SET_MAP_N_MAPVAR,  0x2122,   2,   USB_IN,
										NULL,			1);
	debug("\nmapvar 0: e1,1e");						
	dictionary_call_debug( transfer,	DICT_BUFFER,	SET_MAP_N_MAPVAR,  0xe11e,   0,   USB_IN,
										NULL,			1);
	debug("\nmapvar 1: 11,12");						
	dictionary_call_debug( transfer,	DICT_BUFFER,	SET_MAP_N_MAPVAR,  0x1112,   1,   USB_IN,
										NULL,			1);
	debug("\nmapvar 3: 31,33");						
	dictionary_call_debug( transfer,	DICT_BUFFER,	SET_MAP_N_MAPVAR,  0x3133,   3,   USB_IN,
										NULL,			1);
	

	debug("\npri elements");
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	0,   0,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	0,   1,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	0,   2,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	0,   3,   USB_IN,
										NULL,			8);
	debug("\nsec elements");
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,	0,   0,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,	0,   1,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,	0,   2,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,	0,   3,   USB_IN,
										NULL,			8);
	
///////////////////////////////
	debug("\nmem_part 0: ea,eb");						
	dictionary_call_debug( transfer,	DICT_BUFFER,	SET_MEM_N_PART,  0xeaeb,   0,   USB_IN,
										NULL,			1);
	debug("\nmem_part 1: 1a,1b");						
	dictionary_call_debug( transfer,	DICT_BUFFER,	SET_MEM_N_PART,  0x1a1b,   1,   USB_IN,
										NULL,			1);
	debug("\nmem_part 2: 2a,2b");						
	dictionary_call_debug( transfer,	DICT_BUFFER,	SET_MEM_N_PART,  0x2a2b,   2,   USB_IN,
										NULL,			1);
	debug("\nmem_part 3: 3a,3b");						
	dictionary_call_debug( transfer,	DICT_BUFFER,	SET_MEM_N_PART,  0x3a3b,   3,   USB_IN,
										NULL,			1);
///////////////////////////////

	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	0,   0,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	0,   1,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	0,   2,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	0,   3,   USB_IN,
										NULL,			8);
	debug("\nsec elements");
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,	0,   0,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,	0,   1,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,	0,   2,   USB_IN,
										NULL,			8);
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,	0,   3,   USB_IN,
										NULL,			8);
										*/
	/*debug("\nraw bank status");
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	0,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	1,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	2,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	3,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	4,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	5,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	6,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	7,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	8,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	9,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	10,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	11,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	12,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	13,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	14,   0,   USB_IN,
										NULL,			2);
	dictionary_call_debug( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,	15,   0,   USB_IN,
										NULL,			2);
*/
	//debug("uninit");
	//get_buff_elements( transfer, 0 );
	//get_buff_elements( transfer, 1 );

/*
	check(! reset_buffers( transfer ), "Unable to reset device buffers");
	//need to allocate some buffers for dumping
	//2x 128Byte buffers
	check(! allocate_buffers( transfer, 2, 128 ), "Unable to allocate buffers");

	debug("reset and allocate");
	get_buff_elements( transfer, 0 );
	get_buff_elements( transfer, 1 );

	check(! set_mem_n_part( transfer, 0, 0x12, 0x34 ), "Unable to set mem_type and part");
	debug("set buff0 mem_n_part");
	get_buff_elements( transfer, 0 );
	get_buff_elements( transfer, 1 );
	check(! set_mem_n_part( transfer, 1, 0x56, 0x78 ), "Unable to set mem_type and part");
	debug("set buff1 mem_n_part");
	get_buff_elements( transfer, 0 );
	get_buff_elements( transfer, 1 );

	check(! set_map_n_mapvar( transfer, 0, 0x89, 0xAB ), "Unable to set mapper and map_var");
	debug("set buff0 map_n_mapvar");
	get_buff_elements( transfer, 0 );
	get_buff_elements( transfer, 1 );
	check(! set_map_n_mapvar( transfer, 1, 0xCD, 0XEF ), "Unable to set mapper and map_var");
	debug("set buff1 map_n_mapvar");

	get_buff_operation( transfer );
	get_buff_elements( transfer, 0 );
	get_buff_elements( transfer, 1 );
*/
/*

	dictionary_call( transfer,	DICT_IO,	IO_RESET,			0,		0,		USB_IN,		NULL,		1);
	dictionary_call( transfer,	DICT_IO,	NES_INIT,			0,		0,		USB_IN,		NULL,		1);
	dictionary_call( transfer,	DICT_IO,	EXP0_PULLUP_TEST,		0,		0,		USB_IN,		NULL,		8);
					//dict	opcode	 		   addr/index  		miscdata	endpoint	*buffer 	length
	debug("reset butters");
	dictionary_call( transfer,	DICT_BUFFER,	RAW_BUFFER_RESET,		0,		0,		USB_IN,		NULL,		1);
	dictionary_call( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,		0,		0,		USB_IN,		NULL,		2);
	debug("get pri");						//id:basebank  num32B banks
	dictionary_call( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,		0,		0,		USB_IN,		NULL,		8);
	dictionary_call( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,		0,		1,		USB_IN,		NULL,		8);
//	debug("get nonexistent 7");						//id:basebank  num32B banks
//	dictionary_call( transfer,	BUFFER,	GET_PRI_ELEMENTS,		0,		7,		USB_IN,		NULL,		8);
//	dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER7,		0x7008,		8,		USB_IN,		NULL,		1);
//	debug("get nonexistent 8");						//id:basebank  num32B banks
//	dictionary_call( transfer,	BUFFER,	GET_PRI_ELEMENTS,		0,		8,		USB_IN,		NULL,		8);
	debug("allocate buff0 256B");						//id:basebank  num32B banks
	dictionary_call( transfer,	DICT_BUFFER,	ALLOCATE_BUFFER0,		0x1000,		8,		USB_IN,		NULL,		1);
	debug("allocate buff2 0B");						//id:basebank  num32B banks
	dictionary_call( transfer,	DICT_BUFFER,	ALLOCATE_BUFFER1,		0x2008,		0,		USB_IN,		NULL,		1);
	debug("allocate buff1 256B");						//id:basebank  num32B banks
	dictionary_call( transfer,	DICT_BUFFER,	ALLOCATE_BUFFER1,		0x2008,		8,		USB_IN,		NULL,		1);
	debug("status");						//id:basebank  num32B banks
	dictionary_call( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,		0,		0,		USB_IN,		NULL,		2);
	dictionary_call( transfer,	DICT_BUFFER,	RAW_BANK_STATUS,		8,		0,		USB_IN,		NULL,		2);
	debug("get pri");						//id:basebank  num32B banks
	dictionary_call( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,		0,		0,		USB_IN,		NULL,		8);
	dictionary_call( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,		0,		1,		USB_IN,		NULL,		8);
	debug("get sec");						//id:basebank  num32B banks
	dictionary_call( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,		0,		0,		USB_IN,		NULL,		8);
	dictionary_call( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,		0,		1,		USB_IN,		NULL,		8);

	uint8_t load_in[256];
	uint8_t load_out[256];
	int i = 0;

	load_in[0] = 0xEE;
	//print load
	printf("load_in data:");
	for (i=0; i<256; i++) {
		printf(" %x",load_in[i]);
	}
	printf("\n");

	debug("read payload0 uninitialized");
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_IN,		load_in,		254);

	//print load
	printf("load_in data:");
	for (i=0; i<254; i++) {
		printf(" %x",load_in[i]);
	}
	printf("\n");

	//fill load with 0-127
	for (i=0; i<254; i++) {
		load_out[i] = i;
	}

	//print contents before sending
	printf("load_out with data:");
	for (i=0; i<256; i++) {
		printf(" %x",load_out[i]);
	}
	printf("\n");

	debug("send payload0");
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_OUT,	load_out,		254);

	debug("read payload0");
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_IN,		load_in,		254);

	//print load
	printf("load_in data:");
	for (i=0; i<254; i++) {
		printf(" %x",load_in[i]);
	}
	printf("\n");

	dictionary_call( transfer,	DICT_USB,	0,			0,		0,		USB_IN,		NULL,		3);

	debug("send payload0");
	dictionary_call( transfer,	DICT_BUFFER,	 BUFF_OUT_PAYLOAD_2B_INSP,		0xa5c3,		0,		USB_OUT,	&load_out[2],		254);

	debug("read payload0");
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_IN,		load_in,		254);

	printf("load_in data:");
	for (i=0; i<254; i++) {
		printf(" %x",load_in[i]);
	}
	printf("\n");
	//clock_t tstart, tstop;
	//printf("load_in data:");
	//for (i=0; i<254; i++) {
	//	printf(" %x",load_in[i]);
	//}
	//printf("\n");
	//tstart = clock();
	//for ( i = (1024 * 2); i>0; i--) {
	//dictionary_call( transfer,	BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_OUT,		load_out,		254);
	////for ( i = (1033 * 2); i>0; i--) {
	////dictionary_call( transfer,	BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_IN,		load_in,		254);
	////for ( i = (1024 * 4); i>0; i--) {
	////dictionary_call( transfer,	BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_IN,		load_in,		128);
	//}
	//tstop = clock();
	//float timediff = ( (float)(tstop-tstart) / CLOCKS_PER_SEC);
	//printf("total time: %fsec, speed: %fKBps", timediff, (512/timediff));
	
	//256byte transfers currently clocking in around 21KBps
	
					//dict	opcode	 		   addr/index  		miscdata	endpoint	*buffer 	length
	debug("set func");						
	dictionary_call( transfer,	DICT_BUFFER,	SET_FUNCTION,			DUMPING,	0,		USB_IN,		NULL,		1);
	debug("get sec");				
	dictionary_call( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,		0,		0,		USB_IN,		NULL,		8);
	dictionary_call( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,		0,		1,		USB_IN,		NULL,		8);

	debug("read payload0");
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_IN,		load_in,		254);

	dictionary_call( transfer,	DICT_BUFFER,	SET_FUNCTION,			0,	0,		USB_IN,		NULL,		1);
	printf("load_in data:");
	for (i=0; i<254; i++) {
		printf(" %x",load_in[i]);
	}
	printf("\n");

	printf("load_out with data:");
	for (i=0; i<256; i++) {
		printf(" %x",load_out[i]);
	}
	printf("\n");

	debug("send payload0");
	dictionary_call( transfer,	DICT_BUFFER,	 BUFF_OUT_PAYLOAD_2B_INSP,		0xa5c3,		0,		USB_OUT,	&load_out[2],		254);


	debug("set func");						
	dictionary_call( transfer,	DICT_BUFFER,	SET_FUNCTION,			FLASHING,	0,		USB_IN,		NULL,		1);
	debug("get sec");				
	dictionary_call( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,		0,		0,		USB_IN,		NULL,		8);
	dictionary_call( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,		0,		1,		USB_IN,		NULL,		8);

	debug("read payload0");
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_IN,		load_in,		254);

	printf("load_in data:");
	for (i=0; i<254; i++) {
		printf(" %x",load_in[i]);
	}
	printf("\n");
	debug("set func");						
	dictionary_call( transfer,	DICT_BUFFER,	SET_FUNCTION,			DUMPING,	0,		USB_IN,		NULL,		1);
	debug("get sec");				
	dictionary_call( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,		0,		0,		USB_IN,		NULL,		8);
	dictionary_call( transfer,	DICT_BUFFER,	GET_SEC_ELEMENTS,		0,		1,		USB_IN,		NULL,		8);

	debug("read payload0");
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOAD0,			0,		0,		USB_IN,		load_in,		254);

	printf("load_in data:");
	for (i=0; i<254; i++) {
		printf(" %x",load_in[i]);
	}
	printf("\n");

	dictionary_call( transfer,	DICT_IO,	IO_RESET,			0,		0,		USB_IN,		NULL,		1);
*/
//	dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER2,		0x3508,		4);
//	dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER3,		0x4A0C,		4);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		0,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		1,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		2,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		3,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		4,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		5,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		6,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		7,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		8,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		9,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		10,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		11,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		12,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		13,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		14,		0);
//	dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		15,		0);

					//dict	opcode	 		   addr     		data
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		0,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		1,		0);
	//debug("reset");
	//dictionary_call( transfer,	BUFFER,	RAW_BUFFER_RESET,		0,		0);
	//debug("read status");
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		0,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		1,		0);
	//debug("allocate 0");
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER0,		0x1A00,		1);
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER1,		0x2A01,		1);
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER2,		0x3A02,		1);
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER3,		0x4A03,		1);
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER4,		0x5A04,		1);
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER5,		0x6A05,		1);
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER6,		0x7A06,		1);
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER7,		0x8A07,		1);
	//debug("read status");
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		0,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		1,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		2,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		3,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		4,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		5,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		6,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		7,		0);

	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER7,		0x8A07,		1);
	//debug("reset");
	//dictionary_call( transfer,	BUFFER,	RAW_BUFFER_RESET,		0,		0);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		7,		0);
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER7,		0x8A07,		1);
	//dictionary_call( transfer,	BUFFER,	RAW_BANK_STATUS,		7,		0);
	//dictionary_call( transfer,	BUFFER,	ALLOCATE_BUFFER7,		0x5A05,		1);

	
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

//program byte
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0xAA);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x2AAA,		0x55);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x5555,		0xA0);
//	dictionary_call( transfer,	NES,	DISCRETE_EXP0_PRGROM_WR,	0x8000,		0x00);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);
//	dictionary_call( transfer,	NES,	NES_CPU_RD,			0x8000,		0);

	dictionary_call( transfer,	DICT_IO,	IO_RESET,		0,   0,   USB_IN,
										NULL,			1);

	return 0;

//error:
//	return -1;

}
