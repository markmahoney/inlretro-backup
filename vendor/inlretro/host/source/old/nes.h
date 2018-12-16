#ifndef _nes_h
#define _nes_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

//include prior to other file includes
//that way DEBUG can be turned on/off for this file alone
//uncomment to DEBUG this file alone
//#define DEBUG
//"make debug" to get DEBUG msgs on entire program
#include "dbg.h"

#include "shared_errors.h"
#include "shared_dictionaries.h"
#include "dictionary.h"
#include "memory.h"
//	 "cartridge.h" is above the scope of this module don't include

#include "pindef.h"

int jumper_ciramce_ppuA13n( USBtransfer *transfer );
int ciramce_inv_ppuA13( USBtransfer *transfer );
int famicom_sound( USBtransfer *transfer );
int read_flashID_prgrom_exp0( USBtransfer *transfer, memory *flash );
int read_flashID_prgrom_map30( USBtransfer *transfer, memory *flash );
int read_flashID_chrrom_8K( USBtransfer *transfer, memory *flash );
int ppu_ram_sense( USBtransfer *transfer, uint16_t addr );
int ciram_A10_mirroring( USBtransfer *transfer );

#endif
