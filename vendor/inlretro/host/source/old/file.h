#ifndef _file_h
#define _file_h

//uncomment to DEBUG this file alone
#define DEBUG
//"make debug" to get DEBUG msgs on entire program
#include "dbg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

//TODO put defintions in separate project wide .h file
#include "cartridge.h"
#include "shared_enums.h"

#define SIZE_NES_HEADER 16
#define SIZE_PRG_BANK 16384
#define SIZE_CHR_BANK 8192

//cartridge object/struct
typedef struct rom_image{
	int	console;	//console the cart plays in
	int	mapper;		//mapper number of the board
	int	submap;
	int	mapvariant;	
	int	prg_size;
	int	chr_size;
	int	ram_size;
	int	battery;
	int	mirroring;
	FILE	*fileptr;
} rom_image;

void init_rom_elements(rom_image *rom);
int open_rom( rom_image *rom, char *filename );
int detect_file( rom_image *rom );
int create_file( rom_image *rom, char *filename );
int append_to_file( rom_image *rom, uint8_t *data, int length );
int read_from_file( rom_image *rom, uint8_t *data, int length );
int close_rom( rom_image *rom );

#endif
