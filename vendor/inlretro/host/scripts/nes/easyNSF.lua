
-- create the module's table
local easyNSF = {}

-- import required modules
local dict = require "scripts.app.dict"
local dump = require "scripts.app.dump"
local flash = require "scripts.app.flash"

-- file constants

-- local functions

--local function wr_flash_byte(addr, value, debug)

--base is the actual NES CPU address, not the rom offset (ie $FFF0, not $7FF0)
--local function wr_bank_table(base, entries)
--Action53 not susceptible to bus conflicts, no banktable needed



--initialize mapper for dump/flash routines
local function init_mapper( debug )

	--rom A11-0 are directly connected to CPU
	--A12 pin is part of sector address
	--in BYTE mode, pin A12 is actually CPU A13
	--so ROM A11 must be valid for flash commands
	--ROM A11 pin is actually CPU A12
	--A12 is actually controlled my mapper register...
	--So it should need to be initialized to work, but flash ID is responding properly without it..
	--Therefore I don't think rom A11 pin (CPU A12) needs to be valid, just A11-0?
	

	dict.nes("NES_CPU_WR", 0x5000, 0x00)
	dict.nes("NES_CPU_WR", 0x5001, 0x00)
	dict.nes("NES_CPU_WR", 0x5002, 0x00)
	dict.nes("NES_CPU_WR", 0x5003, 0x00)
	dict.nes("NES_CPU_WR", 0x5004, 0x00)
	dict.nes("NES_CPU_WR", 0x5005, 0x00)
	dict.nes("NES_CPU_WR", 0x5006, 0x00)
	dict.nes("NES_CPU_WR", 0x5007, 0x00)

end


--read PRG-ROM flash ID
local function prgrom_manf_id( debug )

	local rv
	init_mapper()

	if debug then print("reading PRG-ROM manf ID") end
	--A0-A14 are all directly addressable in CNROM mode
	--and mapper writes don't affect PRG banking
	dict.nes("NES_CPU_WR", 0x8AAA, 0xAA)
	dict.nes("NES_CPU_WR", 0x8555, 0x55)
	dict.nes("NES_CPU_WR", 0x8AAA, 0x90)
	rv = dict.nes("NES_CPU_RD", 0x8000)
	if debug then print("attempted read PRG-ROM manf ID:", string.format("%X", rv)) end	--0x01
	rv = dict.nes("NES_CPU_RD", 0x8002)
	if debug then print("attempted read PRG-ROM prod ID:", string.format("%X", rv)) end	--0xDA(top), 0x5B(bot)

	--exit software
	dict.nes("NES_CPU_WR", 0x8000, 0xF0)

end




--Cart should be in reset state upon calling this function 
--this function processes all user requests for this specific board/mapper
local function process(process_opts, console_opts)
	local test = process_opts["test"]
	local read = process_opts["read"]
	local erase = process_opts["erase"]
	local program = process_opts["program"]
	local verify = process_opts["verify"]
	local dumpfile = process_opts["dump_filename"]
	local flashfile = process_opts["flash_filename"]
	local verifyfile = process_opts["verify_filename"]

	local rv = nil
	local file 
	-- TODO: Handle variable rom sizes.

--initialize device i/o for NES
	dict.io("IO_RESET")
	dict.io("NES_INIT")

--test cart by reading manf/prod ID
	if test then
		prgrom_manf_id(true)

	end

--dump the cart to dumpfile
	if read then
		--initialize the mapper for dumping
		init_mapper(debug)

		file = assert(io.open(dumpfile, "wb"))

		--TODO find bank table to avoid bus conflicts!
		--dump cart into file
		dump.dumptofile( file, 1024, "EZNSF", "PRGROM", true )

		--close file
		assert(file:close())
	end

--erase the cart
	if erase then

		--initialize the mapper for erasing
		init_mapper(debug)

		print("\nerasing tsop takes ~30sec");

		print("erasing PRG-ROM");
		--A0-A14 are all directly addressable in CNROM mode
		--only A0-A11 are required to be valid for tsop-48
		--and mapper writes don't affect PRG banking
		dict.nes("NES_CPU_WR", 0x8AAA, 0xAA)
		dict.nes("NES_CPU_WR", 0x8555, 0x55)
		dict.nes("NES_CPU_WR", 0x8AAA, 0x80)
		dict.nes("NES_CPU_WR", 0x8AAA, 0xAA)
		dict.nes("NES_CPU_WR", 0x8555, 0x55)
		dict.nes("NES_CPU_WR", 0x8AAA, 0x10)
		rv = dict.nes("NES_CPU_RD", 0x8000)

		local i = 0

		--TODO create some function to pass the read value 
		--that's smart enough to figure out if the board is actually erasing or not
		while ( rv ~= 0xFF ) do
			rv = dict.nes("NES_CPU_RD", 0x8000)
			i = i + 1
		end
		print(i, "naks, done erasing prg.");

	end


--program flashfile to the cart
	if program then

		--initialize the mapper for dumping
		init_mapper(debug)

		--open file
		file = assert(io.open(flashfile, "rb"))
		--determine if auto-doubling, deinterleaving, etc, 
		--needs done to make board compatible with rom

		--not susceptible to bus conflicts

		--flash cart
		flash.write_file( file, 1024, "EZNSF", "PRGROM", true )
		--close file
		assert(file:close())

	end

--verify flashfile is on the cart
	if verify then
		--for now let's just dump the file and verify manually

		--initialize the mapper for dumping
		init_mapper(debug)

		file = assert(io.open(verifyfile, "wb"))

		--dump cart into file
		dump.dumptofile( file, 1024, "EZNSF", "PRGROM", true )

		--close file
		assert(file:close())
	end

	dict.io("IO_RESET")
end


-- global variables so other modules can use them


-- call functions desired to run when script is called/imported


-- functions other modules are able to call
easyNSF.process = process

-- return the module's table
return easyNSF
