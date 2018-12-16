#ifndef _cartridge_h
#define _cartridge_h

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

#include "io.h"
#include "nes.h"
#include "snes.h"
#include "memory.h"

//cartridge object/struct
typedef struct cartridge{
	int	console;	//console the cart plays in
	int	mapper;		//mapper number of the board
	int	submap;
	int	mapvariant;	
	int	manf;
	int	product;
	int	mirroring;
	int	sound;
	memory	*pri_rom;	//main executable rom (PRG-ROM for NES)
	memory	*sec_rom;	//secondary rom if used (CHR-ROM for NES)
	memory	*save_mem;	//save data memory
	memory	*aux_mem;	//additional memory
	memory	*logic_mem;	//programmable logic
} cartridge;


int init_cart_elements( cartridge *cart );
int detect_console( cartridge *cart, USBtransfer *transfer );
int detect_mirroring( cartridge *cart, USBtransfer *transfer );
int detect_map_mem( cartridge *cart, USBtransfer *transfer, int oper );
int detect_mirroring( cartridge *cart, USBtransfer *transfer );


#endif
