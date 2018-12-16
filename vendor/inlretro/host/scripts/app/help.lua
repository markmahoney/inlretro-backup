
-- create the module's table
local help = {}

-- import required modules
--local dict = require "scripts.app.dict"

-- file constants

-- local functions
local function hex(data)
	return string.format("%X", data)
end


-- global variables so other modules can use them


-- call functions desired to run when script is called/imported


-- functions other modules are able to call
help.hex = hex

-- return the module's table
return help
