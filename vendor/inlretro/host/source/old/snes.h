#ifndef _snes_h
#define _snes_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

//include prior to other file includes
//that way DEBUG can be turned on/off for this file alone
//uncomment to DEBUG this file alone
#define DEBUG
//"make debug" to get DEBUG msgs on entire program
#include "dbg.h"

#include "shared_errors.h"
#include "shared_dictionaries.h"
#include "dictionary.h"

#include "pindef.h"

int snes_mem_visible( USBtransfer *transfer, uint8_t bank, uint16_t addr );

#endif
