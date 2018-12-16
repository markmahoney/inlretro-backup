#ifndef _operation_h
#define _operation_h

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
#include "memory.h"
#include "cartridge.h"

#include "pindef.h"

int load_oper_info_elements( USBtransfer *transfer, cartridge *cart );
int load_oper_info_elements_chr( USBtransfer *transfer, cartridge *cart );
int get_oper_info_elements( USBtransfer *transfer );
int set_operation( USBtransfer *transfer, int operation );
int get_operation( USBtransfer *transfer );



#endif
