#ifndef _memory_h
#define _memory_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

//include prior to other file includes
//that way DEBUG can be turned on/off for this file alone
//uncomment to DEBUG this file alone
#define DEBUG
//"make debug" to get DEBUG msgs on entire program
#include "dbg.h"

#include "usb_operations.h"
#include "shared_errors.h"
#include "shared_dictionaries.h"
#include "dictionary.h"
#include "shared_enums.h"


//KByte for easy math
#define KByte	1024

//memory object/struct
typedef struct memory{
	int	manf;
	int	part;
	int	volatility;	//sram no batt vs batt, mask rom, erasability, etc
	int	size;		//size of the max addressable memory grounding addr pins lowers this value
	int	bank_size;	//size of banks/pages of memory created by mapper banking
	int	wr_dict;	//dictionary used to write to rom
	int	wr_opcode;	//opcode used to write to rom
	int	width;		//width of data bus as configured
	int	protocol;	//parallel, SPI, I2C, JTAG, custom etc.
	int	sector_size;	//minimum eraseable size in bytes
} memory;

void init_memory_elements( memory *mem );


#endif
