#ifndef _dictionary_h
#define _dictionary_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <libusb.h>

//include prior to other file includes
//that way DEBUG can be turned on/off for this file alone
//uncomment to DEBUG this file alone
//#define DEBUG
//"make debug" to get DEBUG msgs on entire program
#include "dbg.h"

#include "usb_operations.h"
#include "shared_errors.h"
#include "shared_dictionaries.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"


void init_dictionary( USBtransfer *transfer );

int lua_dictionary_call (lua_State *L);

//default call dictionary without print option
int dictionary_call( USBtransfer *transfer, uint8_t dictionary, uint8_t opcode, uint16_t addr, 
			uint8_t miscdata, uint8_t endpoint, uint8_t *buffer, uint16_t length);

//debug call dictionary without print option
int dictionary_call_debug( USBtransfer *transfer, uint8_t dictionary, uint8_t opcode, uint16_t addr, 
			uint8_t miscdata, uint8_t endpoint, uint8_t *buffer, uint16_t length);

int dictionary_call_print_option( int print_debug, USBtransfer *transfer, uint8_t dictionary, 
			uint8_t opcode, uint16_t addr, uint8_t miscdata, uint8_t endpoint, 
			uint8_t *buffer, uint16_t length);

#endif
