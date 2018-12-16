#ifndef _flash_h
#define _flash_h

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
#include "cartridge.h"
#include "file.h"
#include "buffer.h"
#include "operation.h"

int flash_cart( USBtransfer* transfer, rom_image *rom, cartridge *cart );


#endif
