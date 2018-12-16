
-- create the module's table
local files = {}

-- import required modules
local help = require "scripts.app.help"

-- file constants

-- local functions


--compare the two files return true if identical
--files should be closed prior to calling, files are closed after compared
local function compare(filename1, filename2, size_must_equal, debug)


	file1 = assert(io.open(filename1, "rb"))
	file2 = assert(io.open(filename2, "rb"))

	local byte_str1
	local byte_str2

	local buffsize = 1
	local byte_num = 0

	local rv = true

	while true do	--exit when end of file 1 reached

		--read next byte from the file and convert to binary
		--gotta be a better way to read a half word (16bits) at a time but don't care right now...
		byte_str1 = file1:read(buffsize)
		byte_str2 = file2:read(buffsize)

		if byte_str1 and byte_str2 then
			--compare byte string from each file

			if byte_str1 == byte_str2 then
				--bytes matched count the bytes
				byte_num = byte_num + 1
				--print(filename1, "was:", help.hex(data1), filename2, "was:", help.hex(data2))
			else
				local data1 = string.unpack("B", byte_str1, 1)
				local data2 = string.unpack("B", byte_str2, 1)
				print("failed to verify byte number:", string.format("0x%X", byte_num))
				print(filename1, "was:", help.hex(data1), filename2, "was:", help.hex(data2))
				rv = false
				break
			end
		
		elseif byte_str1 and not byte_str2 then
			print("end of file:", filename2, "reached, it's smaller than", filename1 )
			if size_must_equal then
				print("files were not the same size")
				rv = false
			else
				rv = "FILE2 larger than FILE1"
			end
			break
		elseif byte_str2 and not byte_str1 then
			print("end of file:", filename1, "reached, it's smaller than", filename2 )
			if size_must_equal then
				print("files were not the same size")
				rv = false
			else
				rv = "FILE1 larger than FILE2"
			end
			break
		else
			--end of both files reached, they must have matched
			break
			rv = true
		end

	end


	--close the files
	assert(file1:close())
	assert(file2:close())

	return rv

end


-- global variables so other modules can use them


-- call functions desired to run when script is called/imported


-- functions other modules are able to call
files.compare = compare

-- return the module's table
return files
