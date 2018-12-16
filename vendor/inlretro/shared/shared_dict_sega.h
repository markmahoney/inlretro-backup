#ifndef _shared_dict_sega_h
#define _shared_dict_sega_h

//define dictionary's reference number in the shared_dictionaries.h file
//then include this dictionary file in shared_dictionaries.h
//The dictionary number is literally used as usb transfer request field
//the opcodes and operands in this dictionary are fed directly into usb setup packet's wValue wIndex fields


//=============================================================================================
//=============================================================================================
// SEGA (genesis/megadrive) DICTIONARY
//
// opcodes contained in this dictionary must be implemented in firmware/source/sega.c
//
//=============================================================================================
//=============================================================================================

//TODO THESE ARE JUST PLACE HOLDERS...
#define	SEGA_RD	0	//RL=3  return error code, data len = 1, 1 byte of data
#define	SEGA_WR	1

// GENESIS ADDR A17-23 along with #LO_MEM & #TIME
// TODO separate #LO_MEM & #TIME, they're currently fixed high
#define SET_BANK 2	


#endif
