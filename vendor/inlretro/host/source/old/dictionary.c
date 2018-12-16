#include "dictionary.h"

static USBtransfer *usb_xfr = NULL;

void init_dictionary( USBtransfer *transfer ) {
	usb_xfr = transfer;
	return;
}

int lua_dictionary_call (lua_State *L) {
	int arg1 = luaL_checknumber(L, 1); /* get print argument */
	int arg2 = luaL_checknumber(L, 2); /* get dictionary argument */
	int arg3 = luaL_checknumber(L, 3); /* get opcode argument */
	int arg4 = luaL_checknumber(L, 4); /* get addr argument */
	int arg5 = luaL_checknumber(L, 5); /* get miscdata argument */
	int arg6 = luaL_checknumber(L, 6); /* get endpoint argument */
	//int arg7 = luaL_checknumber(L, 7); /* get buffer argument */
	int arg8 = luaL_checknumber(L, 8); /* get length argument */
	int rv; //return value


//	printf("arg1 %d, arg2 %d\n", arg1, arg2);
//	printf("arg3 %d, arg4 %d\n", arg3, arg4);

	check( usb_xfr != NULL, "dictionary usb transfer pointer not initialized.\n")

	//dictionary_call_print_option( FALSE, transfer, dictionary, opcode, addr, miscdata, endpoint, buffer, length);
	
	rv = dictionary_call_print_option( arg1, usb_xfr,	arg2,	arg3,	arg4,  arg5,   arg6, 	NULL,	arg8);

	lua_pushnumber(L, rv); /* push result */
	return 1; /* number of results */

error:
	printf("dictionary call went to error\n");
	lua_pushstring(L, "ERROR"); /* push result */
	return 1;

}

/* Make dictionary calls simpler
 * provide USBtransfer pointer with handle set to retro programmer
 * provide dictionary as defined in shared_dictionaries.h (request)
 * provide opcode from the dictionary (wValueLSB)
 * provide 16bit addr used for usb transfer index (optional, can also send 8bits applied to wIndexLSB)
 * provide miscdata optional to be used for wValueMSB
 * provide data buffer pointer and length
 *
 * makes call to usb_transfer after determining:
 * endpoint direction
 * data length
 *
 * debug print of call and return values
 */

//default call dictionary without print option
int dictionary_call( USBtransfer *transfer, uint8_t dictionary, uint8_t opcode, uint16_t addr, 
			uint8_t miscdata, uint8_t endpoint, uint8_t *buffer, uint16_t length)
{
	return dictionary_call_print_option( FALSE, transfer, dictionary, opcode, addr, miscdata, endpoint, buffer, length);
}

//debug call dictionary without print option
int dictionary_call_debug( USBtransfer *transfer, uint8_t dictionary, uint8_t opcode, uint16_t addr, 
			uint8_t miscdata, uint8_t endpoint, uint8_t *buffer, uint16_t length)
{
	return dictionary_call_print_option( ~FALSE, transfer, dictionary, opcode, addr, miscdata, 
						endpoint, buffer, length);
}

int dictionary_call_print_option( int print_debug, USBtransfer *transfer, uint8_t dictionary, uint8_t opcode,
			 uint16_t addr, uint8_t miscdata, uint8_t endpoint, uint8_t *buffer, uint16_t length)
{
	transfer->request   = dictionary;	
	transfer->wValueMSB = miscdata;
	transfer->wValueLSB = opcode;
	transfer->wIndexMSB = (addr>>8);
	transfer->wIndexLSB = addr;

	//default IN for now reading back error codes from short commands
	transfer->endpoint = endpoint;
	transfer->wLength = length;
	//default length of zero
//	transfer->wLength = 0;

	//return data buffer big enough for one data packet
	uint8_t rbuf[8];

	if ( buffer == NULL ) {
		rbuf[0] = 0xee;
		rbuf[1] = 0xee;
		rbuf[2] = 0xee;
		rbuf[3] = 0xee;
		rbuf[4] = 0xee;
		rbuf[5] = 0xee;
		rbuf[6] = 0xee;
		rbuf[7] = 0xee;
		transfer->data = rbuf;
	} else {
		transfer->data = buffer;
	}

	
	if (print_debug) {
		printf("\ndictionary call dict:%d opcode:%dd/%xh addr:%x data:%x\n", 
			dictionary, opcode, opcode, addr, miscdata);
	}
	switch (dictionary) {
		case DICT_PINPORT: debug("dict: PINPORT");
			//transfer->wLength = 1;
			switch (opcode) {
				case PP_OPCODE_ONLY_MIN ... PP_OPCODE_ONLY_MAX:
					debug("PP_OPCODE_ONLY");
					break;
				case PP_OPCODE_8BOP_MIN ... PP_OPCODE_8BOP_MAX:
					debug("PP_OPCODE_8BOP");
					break;
				case PP_OPCODE_16BOP_MIN ... PP_OPCODE_16BOP_MAX:
					debug("PP_OPCODE_16BOP");
					break;
				case PP_OPCODE_24BOP_MIN ... PP_OPCODE_24BOP_MAX:
					debug("PP_OPCODE_24BOP");
					break;
				case PP_OPCODE_8BRV_MIN ... PP_OPCODE_8BRV_MAX:
					debug("PP_OPCODE_8BRV");
//					transfer->wLength = 2;
					break;
				default:	//pinport opcode min/max definition error 
					sentinel("bad PP opcode min/max err:%d",ERR_BAD_PP_OP_MINMAX);
			}
			break; //end of PINPORT

		case DICT_IO: debug("dict: IO");
//			transfer->wLength = 1;
			switch (opcode) {
				case IO_OPCODE_ONLY_MIN ... IO_OPCODE_ONLY_MAX:
					debug("IO_OPCODE_ONLY");
					break;
				case IO_OPCODE_RTN_MIN ... IO_OPCODE_RTN_MAX:
					debug("IO_OPCODE_RTN");
//					transfer->wLength = 8;
					break;
				default:	//io opcode min/max definition error 
					sentinel("bad IO opcode min/max err:%d",ERR_BAD_IO_OP_MINMAX);
			}
			break; //end of IO

		case DICT_NES: debug("dict: NES");
//			transfer->wLength = 1;
			switch (opcode) {
				case NES_OPCODE_24BOP_MIN ... NES_OPCODE_24BOP_MAX:
					debug("NES_OPCODE_24BOP");
					break;
				case NES_OPCODE_16BOP_8BRV_MIN ... NES_OPCODE_16BOP_8BRV_MAX:
					debug("NES_OPCODE_16BOP_8BRV");
//					transfer->wLength = 2;
					break;
				default:	//nes opcode min/max definition error 
					sentinel("bad NES opcode min/max err:%d",ERR_BAD_NES_OP_MINMAX);
			}
			break; //end of NES

		case DICT_SNES: debug("dict: SNES");
//			transfer->wLength = 1;
			switch (opcode) {
				case SNES_OPCODE_24BOP_MIN ... SNES_OPCODE_24BOP_MAX:
					debug("SNES_OPCODE_24BOP");
					break;
				case SNES_OPCODE_24BOP_8BRV_MIN ... SNES_OPCODE_24BOP_8BRV_MAX:
					debug("SNES_OPCODE_24BOP_8BRV");
//					transfer->wLength = 2;
					break;
				default:	//snes opcode min/max definition error 
					sentinel("bad SNES opcode min/max err:%d",ERR_BAD_SNES_OP_MINMAX);
			}
			break; //end of SNES

		case DICT_BUFFER: debug("dict: BUFFER");
//			transfer->wLength = length;
			if (buffer != NULL) {
				transfer->data = (unsigned char *)buffer;
			}
			break; //end of BUFF

		case DICT_USB: debug("dict: USB");
//			transfer->wLength = length;
			break;

		case DICT_OPER: debug("dict: OPER");
//			transfer->wLength = length;
			if (buffer != NULL) {
				transfer->data = (unsigned char *)buffer;
			}
			break; //end of BUFF

		default:
			//request (aka dictionary) is unknown
			sentinel("unknown DICT err:%d",ERR_UNKN_DICTIONARY);
	}


	int xfr_cnt;

	xfr_cnt = usb_transfer( transfer );

	if (print_debug) {
		//print transfer details if small xfr
		if (xfr_cnt <= 8) {
		printf("	xf: %d   er: %d rv:",xfr_cnt, transfer->data[0]);
		int i ;
		for (i=1; i<xfr_cnt; i++){
			printf(" %x,", transfer->data[i]);
		}
		printf("\n"); 
		} else {
		//just print xfr cnt
		printf("						xf: %d\n",xfr_cnt); 

		}
	}

	if (xfr_cnt <= 8) {
		check(transfer->data[0] == SUCCESS, "retro programmer had error: %d, dict:%d, opcode:%d/%x, addr:%x, data:%x",transfer->data[0], dictionary, opcode, opcode, addr, miscdata)
	}

	return SUCCESS;

error:
	printf("dictionary call went to error\n");
	return -1;

}
