
-- create the module's table
local genesis_v1 = {}

-- import required modules
local dict = require "scripts.app.dict"
local dump = require "scripts.app.dump"

-- file constants

-- local functions

local function unsupported(operation)
	print("\nUNSUPPORTED OPERATION: \"" .. operation .. "\" not implemented yet for Sega Genesis.\n")
end

--/ROMSEL is always low for this dump
local function dump_rom( file, rom_size_KB, debug )

	local KB_per_bank = 128	  -- A1-16 = 64K address space, 2Bytes per address
	local addr_base = 0x0000  -- control signals are manually controlled


	local num_reads = rom_size_KB / KB_per_bank
	local read_count = 0

	while (read_count < num_reads) do

		if debug then print( "Dumping ROM part ", read_count + 1, " of ", num_reads) end

		-- A "large" Genesis ROM is 24 banks, many are 8 and 16 - status every 4 is reasonable.
		-- The largest published Genesis game is Super Street Fighter 2, which is 40 banks!
		if (read_count % 4 == 0) then
			print("dumping ROM bank: ", read_count, " of ", num_reads - 1)
		end

		-- Select desired bank.
		dict.sega("SET_BANK", read_count)

		dump.dumptofile(file, KB_per_bank/2, addr_base, "GENESIS_ROM_PAGE0", debug)
		dump.dumptofile(file, KB_per_bank/2, addr_base, "GENESIS_ROM_PAGE1", debug)

		read_count = read_count + 1
	end

end

--Cart should be in reset state upon calling this function 
--this function processes all user requests for this specific board/mapper
local function process(process_opts, console_opts)
	local file 

    -- Initialize device i/o for SEGA
	dict.io("IO_RESET")
	dict.io("SEGA_INIT")


	-- TODO: test cart by reading manf/prod ID
	if process_opts["test"] then
		unsupported("test")
	end

	-- TODO: dump the ram to file 
	if dumpram then
		unsupported("dumpram")
	end

	-- Dump the cart to dumpfile.
	if process_opts["read"] then
		print("\nDumping SEGA ROM...")

		file = assert(io.open(process_opts["dump_filename"], "wb"))

		--dump cart into file
		dump_rom(file, console_opts["rom_size_kbyte"], false)

		--close file
		assert(file:close())
		print("DONE Dumping SEGA ROM")
	end

	-- TODO: erase the cart
	if process_opts["erase"] then
		unsupported("erase")
	end

	-- TODO: write to wram on the cart
	if writeram then
		unsupported("writeram")
	end

	-- TODO: program flashfile to the cart
	if process_opts["program"] then
		unsupported("program")
	end

	-- TODO: verify flashfile is on the cart
	if process_opts["verify"] then
		unsupported("verify")
	end

	dict.io("IO_RESET")
end


-- global variables so other modules can use them
--    NONE

-- call functions desired to run when script is called/imported
--    NONE

-- functions other modules are able to call
genesis_v1.process = process

-- return the module's table
return genesis_v1