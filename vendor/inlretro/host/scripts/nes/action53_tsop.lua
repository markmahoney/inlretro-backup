
-- create the module's table
local action53_tsop = {}

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
	
	--//Setup as CNROM, then scroll through outer banks.
	--cpu_wr(0x5000, 0x80);   //reg select mode
	dict.nes("NES_CPU_WR", 0x5000, 0x80)

	--//   xxSSPPMM   SS-size: 0-32KB, PP-prg mode: 0,1 32KB, MM-mirror
	--cpu_wr(0x8000, 0b00000000);     //reg value 256KB inner, 32KB banks
	dict.nes("NES_CPU_WR", 0x8000, 0x00)
	--cpu_wr(0x5000, 0x81);   //outer reg select mode
	dict.nes("NES_CPU_WR", 0x5000, 0x81)
	--cpu_wr(0x8000, 0x00);   //first 32KB bank
	dict.nes("NES_CPU_WR", 0x8000, 0x00)
	--
	--cpu_wr(0x5000, 0x01);   //inner prg reg select
	dict.nes("NES_CPU_WR", 0x5000, 0x01)
	--cpu_wr(0x8000, 0x00);   //controls nothing in this size
	dict.nes("NES_CPU_WR", 0x8000, 0x00)
	--cpu_wr(0x5000, 0x00);   //chr reg select
	dict.nes("NES_CPU_WR", 0x5000, 0x00)
	--cpu_wr(0x8000, 0x00);   //first chr bank
	dict.nes("NES_CPU_WR", 0x8000, 0x00)
	--selecting CNROM means that mapper writes to $8000-FFFF will only change the CHR-RAM bank which
	--doesn't affect anything we're concerned about
	
	--enable flash writes $5000 set to 0b0 101 010 0
	dict.nes("NES_CPU_WR", 0x5000, 0x54)
	--dict.nes("NES_CPU_WR", 0x5555, 0x54)

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


local function read_gift( base, len )

	local rv
	init_mapper()

	--select last bank in read only mode
	dict.nes("NES_CPU_WR", 0x5000, 0x81)
	dict.nes("NES_CPU_WR", 0x8000, 0xFF)

	local i = 0

	while i < len do 
		rv = dict.nes("NES_CPU_RD", base+i)
		io.write(string.char(rv))
		i = i+1
	end

	i = 0

	print("")

	while i < len do 
		rv = dict.nes("NES_CPU_RD", base+i)
		io.write(string.format("%X.", rv))
		i = i+1
	end

	print("")
end

local function write_gift(base, off)

	local i
	local rv
	init_mapper()

	--select last bank in flash mode
	dict.nes("NES_CPU_WR", 0x5000, 0x81)
	dict.nes("NES_CPU_WR", 0x8000, 0xFF)
	dict.nes("NES_CPU_WR", 0x5000, 0x54)

	--enter unlock bypass mode
	dict.nes("NES_CPU_WR", 0x8AAA, 0xAA)
	dict.nes("NES_CPU_WR", 0x8555, 0x55)
	dict.nes("NES_CPU_WR", 0x8AAA, 0x20)

	--write 0xA0 to address of byte to write, then write data
	dict.nes("NES_CPU_WR", base+off, 0xA0)
	dict.nes("NES_CPU_WR", base+off, 0x00)		--end previous line
	off=off+1
	dict.nes("NES_CPU_WR", base+off, 0xA0)
	dict.nes("NES_CPU_WR", base+off, 0x15)		--line number..?
	off=off+1
	dict.nes("NES_CPU_WR", base+off, 0xA0)
	dict.nes("NES_CPU_WR", base+off, string.byte("(",1))		--start with open parenth


	--off = off + 1	--increase to start of message but index starting at 1
	i = 1

	--local msg1 = "Regular Edition"
	--local msg1 = "Contributor Edition"
	local msg1 = "Limited Edition"
	local msg2 = "82 of 100"	--  all flashed

	--local msg1 = " Contributor Edition "
	--local msg2 = " PinoBatch "	--issue if capital P or R is first char for some reason..

	local len = string.len(msg1)

	while (i <= len) do
		dict.nes("NES_CPU_WR", base+off+i, 0xA0)
		dict.nes("NES_CPU_WR", base+off+i, string.byte(msg1,i))	--line 1 of message
		print("write:", string.byte(msg1,i))
		i=i+1
	end

	off = off + i

	dict.nes("NES_CPU_WR", base+off, 0xA0)
	dict.nes("NES_CPU_WR", base+off, 0x00)		--end current line
	off=off+1
	dict.nes("NES_CPU_WR", base+off, 0xA0)
	dict.nes("NES_CPU_WR", base+off, 0x16)		--line number..?
	off=off+1
	dict.nes("NES_CPU_WR", base+off, 0xA0)
	dict.nes("NES_CPU_WR", base+off, string.byte("(",1))		--start with open parenth

	i = 1


	len = string.len(msg2)

	while (i <= len) do
		dict.nes("NES_CPU_WR", base+off+i, 0xA0)
		dict.nes("NES_CPU_WR", base+off+i, string.byte(msg2,i))	--line 2 of message
		print("write:", string.byte(msg2,i))
		i=i+1
	end

	off = off + i

	dict.nes("NES_CPU_WR", base+off, 0xA0)
	dict.nes("NES_CPU_WR", base+off, 0x00)		--end current line

	--]]


	--poll until stops toggling, or data is as wrote
--	rv = dict.nes("NES_CPU_RD", 0x8BDC)
--	print (rv)


	--exit unlock bypass
	dict.nes("NES_CPU_WR", 0x8000, 0x90)
	dict.nes("NES_CPU_WR", 0x8000, 0x00)
	--reset the flash chip
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
	-- TODO: Handle variable ROM sizes.

--initialize device i/o for NES
	dict.io("IO_RESET")
	dict.io("NES_INIT")

--test cart by reading manf/prod ID
	if test then
		prgrom_manf_id(true)

		--manipulate gift message
		local base = 0x8BD0
		local start_offset = 0xC
		local len = 80
		read_gift(base, len)

		write_gift(base, start_offset)

		read_gift(base, len)
	end

--dump the cart to dumpfile
	if read then
		--initialize the mapper for dumping
		init_mapper(debug)

		file = assert(io.open(dumpfile, "wb"))

		--TODO find bank table to avoid bus conflicts!
		--dump cart into file
		dump.dumptofile( file, 1024, "A53", "PRGROM", true )

		--close file
		assert(file:close())
	end

--erase the cart
	if erase then

		--initialize the mapper for erasing
		init_mapper(debug)

		print("\nerasing action53 tsop takes ~30sec");

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
		flash.write_file( file, 1024, "A53", "PRGROM", true )
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
		dump.dumptofile( file, 1024, "A53", "PRGROM", true )

		--close file
		assert(file:close())
	end

	dict.io("IO_RESET")
end


-- global variables so other modules can use them


-- call functions desired to run when script is called/imported


-- functions other modules are able to call
action53_tsop.process = process

-- return the module's table
return action53_tsop
