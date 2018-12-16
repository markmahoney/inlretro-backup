#include "memory.h"

void init_memory_elements( memory *mem )
{
	mem->manf = UNKNOWN;
	mem->part = UNKNOWN;
	mem->volatility = UNKNOWN;
	mem->size = UNKNOWN;
	mem->bank_size = UNKNOWN;
	mem->wr_dict = UNKNOWN;
	mem->wr_opcode = UNKNOWN;
	mem->width = UNKNOWN;
	mem->protocol = UNKNOWN;
	mem->sector_size = UNKNOWN;
}

