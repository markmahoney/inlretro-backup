#ifndef _io_h
#define _io_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <libusb.h>

//include prior to other file includes
//that way DEBUG can be turned on/off for this file alone
//uncomment to DEBUG this file alone
#define DEBUG
//"make debug" to get DEBUG msgs on entire program
#include "dbg.h"

#include "shared_errors.h"
#include "shared_dictionaries.h"
#include "dictionary.h"

void io_reset( USBtransfer *transfer );
void nes_init( USBtransfer *transfer );
void snes_init( USBtransfer *transfer );
int exp0_pullup_test( USBtransfer *transfer );

#endif
