#include "file.h"

void init_rom_elements( rom_image *rom )
{
	rom->console = UNKNOWN;
	rom->mapper = UNKNOWN;
	rom->submap = UNKNOWN;
	rom->mapvariant = UNKNOWN;
	rom->prg_size = UNKNOWN;
	rom->chr_size = UNKNOWN;
	rom->ram_size = UNKNOWN;
	rom->battery = UNKNOWN;
	rom->mirroring = UNKNOWN;
	rom->fileptr = NULL;

}

//Need to pass in pointer to a filepointer to properly pass by reference
//the OS/stdio creates a FILE struct and returns the address of that struct
//so when this function opens a file it's setting the value of a pointer
//for the calling function.  To set a pointer we must have a pointer to that pointer..
//int open_file( FILE **fptr, char *filename ) 
int open_rom( rom_image *rom, char *filename ) 
{
	//first open file
	//*fptr = fopen( filename, "rb");
	rom->fileptr = fopen( filename, "rb");
	//returns file ptr on success, NULL on fail
	//check( *fptr, "Unable to open file: %s in read binary mode", filename); 	
	check( rom->fileptr, "Unable to open file: %s in read binary mode", filename); 	

	return SUCCESS;
error:
	return -1;
}

int close_rom( rom_image *rom ) 
{
	debug("flushing");
	fflush(rom->fileptr);
	debug("closing");
	return fclose( rom->fileptr );
}

int detect_file( rom_image *rom )
{
	int rv = 0;
	uint8_t header[SIZE_NES_HEADER];

	//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
	rv = fread( header, sizeof(header[0]), (sizeof(header)/sizeof(header[0])), rom->fileptr);
	check( rv == sizeof(header), "Unable to read NES header");

	//0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
	if ( (header[0]=='N') && (header[1]=='E') && (header[2]=='S') && (header[3]==0x1A) ) {
		debug("detected ines header");
		rom->console = NES_CART;
	} else {
		debug("only ines files currently accepted as input");
		goto error;
	}

	//4: Size of PRG ROM in 16 KB units
	rom->prg_size = header[4] * SIZE_PRG_BANK;
	debug("PRG ROM size= %d", rom->prg_size);

	//5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
	rom->chr_size = header[5] * SIZE_CHR_BANK;
	debug("CHR ROM size= %d", rom->chr_size);

	//6: Flags 6
	//   76543210
	//   ||||||||
	//   ||||+||+- 0xx0: vertical arrangement/horizontal mirroring (CIRAM A10 = PPU A11)
	//   |||| ||   0xx1: horizontal arrangement/vertical mirroring (CIRAM A10 = PPU A10)
	//   |||| ||   1xxx: four-screen VRAM
	//   |||| |+-- 1: Cartridge contains battery-backed PRG RAM ($6000-7FFF) or other persistent memory
	//   |||| +--- 1: 512-byte trainer at $7000-$71FF (stored before PRG data)
	//   ++++----- Lower nybble of mapper number
	rom->mapper = (header[6]>>4);
	rom->mapper |= (header[7] & 0xF0);
	debug("mapper #%d", rom->mapper);
	rom->mirroring = header[6] & 0x09;	//0b0000 1001
	debug("mirroring:%x", rom->mirroring);

	//7: Flags 7
	//   76543210
	//   ||||||||
	//   |||||||+- VS Unisystem
	//   ||||||+-- PlayChoice-10 (8KB of Hint Screen data stored after CHR data)
	//   ||||++--- If equal to 2, flags 8-15 are in NES 2.0 format
	//   ++++----- Upper nybble of mapper number

	//8: Size of PRG RAM in 8 KB units (Value 0 infers 8 KB for compatibility; see PRG RAM circuit)
	//9: Flags 9
	//10: Flags 10 (unofficial)
	//11-15: Zero filled
	
	return SUCCESS;
error:
	return -1;
}


int create_file( rom_image *rom, char *filename )
{
	//TODO check if file already exists, if so prompt if user would like to overwrite

	rom->fileptr = fopen( filename, "wb+");
	check( rom->fileptr, "Unable to create file: %s in read/write binary mode", filename); 	

	return SUCCESS;
error:
	return -1;
}


/* Desc:Append data to rom file
 * Pre: rom file created and opened
 * Post:data of length appended to file
 *	file still open
 * Rtn: SUCCESS if no errors
 */
int append_to_file( rom_image *rom, uint8_t *data, int length )
{
	int rv = 0;
	//size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
	rv = fwrite( data, sizeof(data[0]), length, rom->fileptr );

	check( (rv == length), "Error appending to file, %dB out written when trying to write %d", rv, length);

	return SUCCESS;
error:
	return -1;
}

/* Desc:Read data from file
 * Pre: file opened and current position set to desired location
 * Post:data filled with length amount of data from file
 *	file still open
 * Rtn: SUCCESS if no errors
 */
int read_from_file( rom_image *rom, uint8_t *data, int length )
{
	int rv = 0;
	//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
	rv = fread( data, sizeof(data[0]), length, rom->fileptr );

	check( (rv == length), "Error reading from file, %dB read when trying to read %d", rv, length);

	return SUCCESS;
error:
	return -1;
}
