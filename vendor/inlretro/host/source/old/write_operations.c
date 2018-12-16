#include "write_operations.h"

#define SIZE_NES_HEADER 16
#define SIZE_PRG_BANK 16384
#define SIZE_CHR_BANK 8192

int write_file( libusb_device_handle *usbhandle, char *filename, char *ines_mapper, char *board_config )
{
	int rv = 0;
	int index = 0;
	FILE *fileptr = NULL;
//warn	uint8_t data[128];

	//first open file
	fileptr = fopen( filename, "rb");
	//returns file ptr on success, NULL on fail
	check( fileptr, "Unable to open file: %s in read binary mode", filename); 	

	//then determine file type
	uint8_t header[SIZE_NES_HEADER];
	//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
	rv = fread( header, sizeof(header[0]), (sizeof(header)/sizeof(header[0])), fileptr);
	check( rv = sizeof(header), "Unable to read NES header");

	for ( index = 0; index < SIZE_NES_HEADER; index++ ) {
		debug("header byte #%d = %x", index, header[index]);
	}

	//0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
	//4: Size of PRG ROM in 16 KB units
	//5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
	//6: Flags 6
	//7: Flags 7
	//8: Size of PRG RAM in 8 KB units (Value 0 infers 8 KB for compatibility; see PRG RAM circuit)
	//9: Flags 9
	//10: Flags 10 (unofficial)
	//11-15: Zero filled
//warn	uint8_t num_prg_banks = header[4];
//warn	uint8_t num_chr_banks = header[5];
//warn
//warn	int prg_size = num_prg_banks * SIZE_PRG_BANK;
//warn	int chr_size = num_chr_banks * SIZE_CHR_BANK;
	
	//next check board inserted
	
	//determine if file is compatible with board
	
	//load script file
	
	//load retro-prog commands
	
	//check if erased, erase as needed, and verify
	
	//write proper data from file to board proper location on board
	
	//close file
	//int fclose(FILE *fp);
	check( fclose(fileptr) == SUCCESS, "Unable to close file");
	fileptr = NULL;
	
	return 0;

error:

	if (fileptr) {
		fclose(fileptr);
	}

	return -1;

}
