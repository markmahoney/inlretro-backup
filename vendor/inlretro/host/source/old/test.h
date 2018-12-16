#ifndef _test_h
#define _test_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <libusb.h>
#include <time.h>

//include prior to other file includes
//that way DEBUG can be turned on/off for this file alone
//uncomment to DEBUG this file alone
#define DEBUG
//"make debug" to get DEBUG msgs on entire program
#include "dbg.h"

#include "usb_operations.h"
#include "shared_errors.h"
#include "shared_enums.h"
#include "shared_dictionaries.h"
#include "dictionary.h"
#include "buffer.h"
#include "cartridge.h"

//uncomment to DEBUG this file alone
#define DEBUG
//"make debug" to get DEBUG msgs on entire program
#include "dbg.h"

int test_function( cartridge *cart, USBtransfer *transfer );

#endif
