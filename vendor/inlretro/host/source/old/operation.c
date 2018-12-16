#include "operation.h"

/* Desc:Set all oper_info elements based on cartridge
 * Pre: buff0 must be initialized
 * Post:oper_info elements loaded
 * Rtn: SUCCESS if no errors
 */
int load_oper_info_elements( USBtransfer *transfer, cartridge *cart ) 
{
	uint8_t rv[RETURN_BUFF_SIZE];
	uint8_t buff_num = 0;		//buffer used to load elements according to shared_dict_operation.h
	uint8_t oper_info[OPER_DATA_NUM_BYTE_ELEMENTS];
//	int i;
	
	//first make sure buff0 is big enough
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	NILL, 	buff_num,
								USB_IN,		rv,	RETURN_BUFF_SIZE);
	check( rv[BUFF_LASTIDX] >= OPER_DATA_NUM_BYTE_ELEMENTS, 
		"buff0 not large enough to load oper_info. Only %d available, need %d", 
		rv[BUFF_LASTIDX], OPER_DATA_NUM_BYTE_ELEMENTS );
	
	//fill array with oper_info elements then payload to buff0 
	//overall type of operation being performed
	oper_info[OPERATION] = PREPARING,
	//mask page_num lower byte to get directly addressable A15:A8 bits
	oper_info[ADDRH_DMASK] = MSK_32KB,
	//shift page_num to right this many bits to get cur bank value
	oper_info[PG2BANK_SHRIGHT] = PG2B_32KB,
	//most significant bit that must be valid for operation (ie A14 SST)
	oper_info[VALID_ADDR_MSB] = 14,
	//unlock sequence SST $5555 0xAA
	//unlock sequence #1 bank number for mapper reg
	oper_info[UNLOCK1_BANK]	= 0,
	//unlock sequence #1 A15:A8
	oper_info[UNLOCK1_AH]	= 0x55,
	//unlock sequence #1 A7:A0
	oper_info[UNLOCK1_AL]	= 0x55,
	//unlock sequence #1 D7:D0
	oper_info[UNLOCK1_DATA]	= 0xAA,
	////unlock sequence SST $2AAA 0x55
	//unlock sequence #1 bank number for mapper reg
	oper_info[UNLOCK2_BANK]	= 0,
	//unlock sequence #2 A15:A8
	oper_info[UNLOCK2_AH]	= 0x2A,
	//unlock sequence #2 A7:A0
	oper_info[UNLOCK2_AL]	= 0xAA,
	//unlock sequence #2 D7:D0
	oper_info[UNLOCK2_DATA]	= 0x55,
	//command SST byte write $5555 0xA0,  SST sector/chip erase $5555 0x80
	//flash command bank (ie bank to write byte write, sector erase cmd)
	oper_info[COMMAND_BANK]	= 0,
	//flash command A15:A8
	oper_info[COMMAND_AH]	= 0x55,
	//flash command A7:A0
	oper_info[COMMAND_AL]	= 0x55,
	///flash command D7:D0 command 1 data (ie SST sect erase 0x80)
	oper_info[COMMAND1_DATA]= 0xA0,
	//flash command D7:D0 command 2 data (ie SST sect erase 0x30)
	oper_info[COMMAND2_DATA]= 0,
	//actual byte operation (ie Byte address bank and addr)
	//current bank value for actual operation to be done (ie write byte)
	oper_info[OPER_BANK]	= 0,
	//operation A15:A8 (ie actual byte write address)
	oper_info[OPER_AH]	= 0,

	//load byte element data into buff0
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOADN,
			NILL, 		buff_num,	USB_OUT,	
			oper_info,	OPER_DATA_NUM_BYTE_ELEMENTS);
	//now that elements are in buff0 instruct them to be copied over
	dictionary_call_debug( transfer,	DICT_OPER,	COPY_BUFF0_TO_ELEMENTS,	
				NILL, 	NILL,	USB_IN,		NULL,	RV_ERR_IDX+1);
	
error:
	return ~SUCCESS;
}

/* Desc:Set all oper_info elements based on cartridge
 * Pre: buff0 must be initialized
 * Post:oper_info elements loaded
 * Rtn: SUCCESS if no errors
 */
int load_oper_info_elements_chr( USBtransfer *transfer, cartridge *cart ) 
{
	uint8_t rv[RETURN_BUFF_SIZE];
	uint8_t buff_num = 0;		//buffer used to load elements according to shared_dict_operation.h
	uint8_t oper_info[OPER_DATA_NUM_BYTE_ELEMENTS];
//	int i;
	
	//first make sure buff0 is big enough
	dictionary_call_debug( transfer,	DICT_BUFFER,	GET_PRI_ELEMENTS,	NILL, 	buff_num,
								USB_IN,		rv,	RETURN_BUFF_SIZE);
	check( rv[BUFF_LASTIDX] >= OPER_DATA_NUM_BYTE_ELEMENTS, 
		"buff0 not large enough to load oper_info. Only %d available, need %d", 
		rv[BUFF_LASTIDX], OPER_DATA_NUM_BYTE_ELEMENTS );
	
	//fill array with oper_info elements then payload to buff0 
	//overall type of operation being performed
	oper_info[OPERATION] = PREPARING,
	//mask page_num lower byte to get directly addressable A15:A8 bits
	oper_info[ADDRH_DMASK] = MSK_8KB,
	//shift page_num to right this many bits to get cur bank value
	oper_info[PG2BANK_SHRIGHT] = PG2B_8KB,
	//most significant bit that must be valid for operation (ie A14 SST)
	oper_info[VALID_ADDR_MSB] = 12,
	//unlock sequence SST $5555 0xAA
	//unlock sequence #1 bank number for mapper reg
	oper_info[UNLOCK1_BANK]	= 0,
	//unlock sequence #1 A15:A8
	oper_info[UNLOCK1_AH]	= 0x15,
	//unlock sequence #1 A7:A0
	oper_info[UNLOCK1_AL]	= 0x55,
	//unlock sequence #1 D7:D0
	oper_info[UNLOCK1_DATA]	= 0xAA,
	////unlock sequence SST $2AAA 0x55
	//unlock sequence #1 bank number for mapper reg
	oper_info[UNLOCK2_BANK]	= 0,
	//unlock sequence #2 A15:A8
	oper_info[UNLOCK2_AH]	= 0x0A,
	//unlock sequence #2 A7:A0
	oper_info[UNLOCK2_AL]	= 0xAA,
	//unlock sequence #2 D7:D0
	oper_info[UNLOCK2_DATA]	= 0x55,
	//command SST byte write $5555 0xA0,  SST sector/chip erase $5555 0x80
	//flash command bank (ie bank to write byte write, sector erase cmd)
	oper_info[COMMAND_BANK]	= 0,
	//flash command A15:A8
	oper_info[COMMAND_AH]	= 0x15,
	//flash command A7:A0
	oper_info[COMMAND_AL]	= 0x55,
	///flash command D7:D0 command 1 data (ie SST sect erase 0x80)
	oper_info[COMMAND1_DATA]= 0xA0,
	//flash command D7:D0 command 2 data (ie SST sect erase 0x30)
	oper_info[COMMAND2_DATA]= 0,
	//actual byte operation (ie Byte address bank and addr)
	//current bank value for actual operation to be done (ie write byte)
	oper_info[OPER_BANK]	= 0,
	//operation A15:A8 (ie actual byte write address)
	oper_info[OPER_AH]	= 0,

	//load byte element data into buff0
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOADN,
			NILL, 		buff_num,	USB_OUT,	
			oper_info,	OPER_DATA_NUM_BYTE_ELEMENTS);
	//now that elements are in buff0 instruct them to be copied over
	dictionary_call_debug( transfer,	DICT_OPER,	COPY_BUFF0_TO_ELEMENTS,	
				NILL, 	NILL,	USB_IN,		NULL,	RV_ERR_IDX+1);
	
error:
	return ~SUCCESS;
}

/* Desc:Get all oper_info elements
 * Pre: buff0 must be initialized
 * Post:
 * Rtn: SUCCESS if no errors
 */
int get_oper_info_elements( USBtransfer *transfer ) 
{
	uint8_t oper_info[OPER_DATA_NUM_BYTE_ELEMENTS];
	uint8_t buff_num = 0;		//buffer used to load elements according to shared_dict_operation.h
	int i;

	//now that buff0 is filled with junk copy elements over to buff0
	dictionary_call( transfer,	DICT_OPER,	COPY_ELEMENTS_TO_BUFF0,	
				NILL, 	NILL,	USB_IN,		NULL,	RV_ERR_IDX+1);
	//now read back buff0
	dictionary_call( transfer,	DICT_BUFFER,	BUFF_PAYLOADN,
			NILL, 		buff_num,	USB_IN,	
			oper_info,	OPER_DATA_NUM_BYTE_ELEMENTS);
	printf("oper_info:\n");
	for( i=0; i<OPER_DATA_NUM_BYTE_ELEMENTS; i++) {
		printf("%d: %x \t", i, oper_info[i]) ;
	}
	printf("\n");

	return SUCCESS;
}


/* Desc:Set operation 
 * Pre: buffers are allocated and oper_info elements set ready to start operation
 * Post:operation starts on device
 * Rtn: SUCCESS if no errors
 */
int set_operation( USBtransfer *transfer, int operation ) 
{
	return dictionary_call( transfer,	DICT_OPER,	SET_OPERATION,	operation, 	
					NILL,	USB_IN,	NULL,	1);
}


/* Desc:Get opertation
 * Pre: 
 * Post:
 * Rtn: SUCCESS if no errors
 */
int get_operation( USBtransfer *transfer ) 
{
	printf("operation:");
	dictionary_call_debug( transfer,	DICT_OPER,	GET_OPERATION,	NILL, 	NILL,
								USB_IN,		NULL,	RV_DATA0_IDX+1);
	return SUCCESS;
}
