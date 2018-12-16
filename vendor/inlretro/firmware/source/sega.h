#ifndef _sega_h
#define _sega_h

#include "pinport.h"
#include "shared_dictionaries.h"
#include "shared_errors.h"


uint8_t sega_call( uint8_t opcode, uint8_t miscdata, uint16_t operand, uint8_t *rdata );

uint8_t	sega_rd( uint16_t addr );
void	sega_wr( uint16_t addr, uint8_t data );

uint8_t genesis_page_rd( uint8_t *data, uint16_t addrH, uint8_t first, uint8_t len );

#endif
