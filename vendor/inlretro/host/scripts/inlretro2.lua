-- Main application flow for interacting with cartridges via USB device.
-- Refactored version that doesn't require commenting/uncommenting to change functionality.


-- Helper function that checks if a string is empty or nil.
local function isempty(s)
    return s == nil or s == ''
end

-- Wrapper for managing GBA operations.
function gba_exec(process_opts, console_opts)
    local gba = require "scripts.gba.basic"

    -- Defensively filter out any console options that are irrelevant to GBA support.
    local gba_opts = {
        rom_size_mbit = console_opts["rom_size_mbit"],
        wram_size_kb = console_opts["wram_size_kb"],
    }
    gba.process(process_opts, gba_opts)
end

-- Wrapper for managing Sega Genesis operations.
function genesis_exec(process_opts, console_opts)
    local genesis = require "scripts.sega.genesis_v1"

    -- Defensively filter out any console options that are irrelevant to Genesis support.
    local genesis_opts = {
        rom_size_mbit = console_opts["rom_size_mbit"],
        wram_size_kb = console_opts["wram_size_kb"],
    }
    genesis.process(process_opts, genesis_opts)
end

-- Wrapper for managing N64 operations.
function n64_exec(process_opts, console_opts)
    local n64 = require "scripts.n64.basic"

    -- Defensively filter out any console options that are irrelevant to N64 support.
    local n64_opts = {
        rom_size_mbit = console_opts["rom_size_mbit"],
        wram_size_kb = console_opts["wram_size_kb"],
    }
    n64.process(process_opts, n64_opts)
end

-- Wrapper for managing NES/Famicom operations.
function nes_exec(process_opts, console_opts)
    local dict = require "scripts.app.dict"
    local nes = require "scripts.app.nes"
    
    -- Defensively filter out any console options that are irrelevant to NES support.
    -- This will matter more when software support exists for other consoles.
    local nes_opts = {
        wram_size_kb = console_opts["wram_size_kb"],
        prg_rom_size_kb = console_opts["prg_rom_size_kb"],
        chr_rom_size_kb = console_opts["chr_rom_size_kb"],
        mirror = nil -- Used by NROM mapper only, "H" or "V".
    }

    local mappers = {
        action53_tsop = require "scripts.nes.action53_tsop",
        action53 = require "scripts.nes.action53",
        bnrom = require "scripts.nes.bnrom",
        cdream = require "scripts.nes.cdream",
        cninja = require "scripts.nes.cninja",
        cnrom = require "scripts.nes.cnrom",
        dualport = require "scripts.nes.dualport",
        easynsf = require "scripts.nes.easyNSF",
        fme7 = require "scripts.nes.fme7",
        mapper30 = require "scripts.nes.mapper30",
        mmc1 = require "scripts.nes.mmc1",
        mmc3 = require "scripts.nes.mmc3",
        mmc4 = require "scripts.nes.mmc4",
        nrom = require "scripts.nes.nrom",
        unrom = require "scripts.nes.unrom"
    }

    dict.io("IO_RESET")	
	dict.io("NES_INIT")	
    nes.detect_mapper_mirroring(true)
    
    m = mappers[console_opts[mapper]]
    if m == nil then
        print("UNSUPPORTED MAPPER")
    else
        -- Attempt requested operations with hardware!

        -- TODO: Do plumbing for interacting with RAM.
        m.process(process_opts, console_opts)
    end
end

-- Point of entry from C language half of program.
function main()

    -- Globals passed in from C: 
    --  console_name:    string, name of console.
    --  mapper_name:     string, name of mapper.
    --  dump_filename:   string, filename used for writing dumped data.
    --  flash_filename:  string, filename containing data to write cartridge.
    --  verify_filename: string, filename used for writing back data written to cartridge for verification.
    --  nes_prg_rom_size_kb:    int, size of cartridge PRG-ROM in kb.
    --  nes_chr_rom_size_kb:    int, size of cartridge CHR-ROM in kb.
    --  nes_wram_size_kb:       int, size of cartridge WRAM in kb.

    -- TODO: This should probably be one level up.
    -- TODO: Ram probably needs a verify file as well?

    -- Always test!
    local do_test = true
    
    -- If a dump filename was provided, dump data from cartridge to a file.
    local do_read = not isempty(dump_filename)
    
    -- If a flash filename was provided, write its contents to the cartridge.
    -- TODO: Check for erase + dump at same time, not permitted.
    local do_erase = not isempty(flash_filename)
    -- If writing, always erase.
    -- TODO: Check for program and dump at same time, not permitted.
    local do_program = do_erase
    
    -- If a verify_filename was provided, dump data from cartridge after flash to a file.
    local do_verify = not isempty(verify_filename)

    local do_dumpram = not isempty(ramdump_filename)
    local do_writeram = not isempty(ramwrite_filename)

    -- Pack main process state into table.
    local process_opts = {
        test = do_test,
        read = do_read,
        dump_filename = dump_filename,
        erase = do_erase,
        program = do_program,
        program_filename = flash_filename,
        verify = do_verify,
        verify_filename = verify_filename,
        dumpram = do_dumpram,
        dumpram_filename = ramdump_filename,
        writeram = do_writeram,
        writeram_filename = ramwrite_filename,
    }

    -- TODO: Add SNES support, as it appears to be currently usable?
    local consoles = {
        GBA = gba_exec,
        GENESIS = genesis_exec,
        NES = nes_exec,
        N64 = n64_exec,
    }
    f = consoles[console_name]
    if f == nil then 
        print("UNSUPPORTED CONSOLE")
    else
        -- TODO: Make anything console specific a dict to make function signatures less terrible.
        local console_opts = {
            wram_size_kb = nes_wram_size_kb,
            prg_rom_size_kb = nes_prg_rom_size_kb,
            chr_rom_size_kb = nes_chr_rom_size_kb,
            rom_size_mbit = rom_size_mbit,
            mapper = mapper,
        }

        f(process_opts, console_opts)
    end
end

-- Don't do this. Next iteration will call a function, not the whole script.
main()
