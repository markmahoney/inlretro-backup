#ifndef _nes_h
#define _nes_h

#include "pinport.h"
#include "buffer.h"	//TODO remove this junk when get rid of FALSE
#include "shared_dictionaries.h"
#include "shared_errors.h"
#include "stuff.h"

uint8_t nes_call( uint8_t opcode, uint8_t miscdata, uint16_t operand, uint8_t *rdata );

void	discrete_exp0_prgrom_wr( uint16_t addr, uint8_t data );
void	disc_push_exp0_prgrom_wr( uint16_t addr, uint8_t data );
//void	discrete_exp0_mapper_wr( uint16_t addr, uint8_t data );
uint8_t	emulate_nes_cpu_rd( uint16_t addr );
uint8_t	nes_cpu_rd( uint16_t addr );
void	nes_cpu_wr( uint16_t addr, uint8_t data );
void	nes_m2_low_wr( uint16_t addr, uint8_t data );
uint8_t	nes_ppu_rd( uint16_t addr );
void	nes_ppu_wr( uint16_t addr, uint8_t data );
uint8_t	nes_dualport_rd( uint16_t addr );
void	nes_dualport_wr( uint16_t addr, uint8_t data );
//uint8_t	ciram_a10_mirroring( void );
uint8_t nes_cpu_page_rd_poll( uint8_t *data, uint8_t addrH, uint8_t first, uint8_t last, uint8_t poll );
uint8_t nes_ppu_page_rd_poll( uint8_t *data, uint8_t addrH, uint8_t first, uint8_t last, uint8_t poll );
uint8_t nes_dualport_page_rd_poll( uint8_t *data, uint8_t addrH, uint8_t first, uint8_t len, uint8_t poll );

void	mmc1_wr( uint16_t addr, uint8_t data, uint8_t reset );

uint8_t	nrom_prgrom_flash_wr( uint16_t addr, uint8_t data );
void 	nrom_chrrom_flash_wr( uint16_t addr, uint8_t data );
void 	mmc1_prgrom_flash_wr( uint16_t addr, uint8_t data );
void	mmc1_chrrom_flash_wr( uint16_t addr, uint8_t data );
void 	unrom_prgrom_flash_wr( uint16_t addr, uint8_t data );
void 	cnrom_chrrom_flash_wr( uint16_t addr, uint8_t data );
uint8_t	mmc3_prgrom_flash_wr( uint16_t addr, uint8_t data );
void	mmc3_chrrom_flash_wr( uint16_t addr, uint8_t data );
void 	mmc4_prgrom_sop_flash_wr( uint16_t addr, uint8_t data );
void 	mmc4_chrrom_flash_wr( uint16_t addr, uint8_t data );
void 	cdream_chrrom_flash_wr( uint16_t addr, uint8_t data );
uint8_t	map30_prgrom_flash_wr( uint16_t addr, uint8_t data );
void 	ppu_page_wr_lfsr( uint16_t addr, uint8_t data );



#define A10_BYTE 	0x04
#define A11_BYTE 	0x08
#define PPU_A13N_WORD	0x8000
#define PPU_A13N_BYTE	0x80

#endif
