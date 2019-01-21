from lib.defs import Mirroring

# https://wiki.nesdev.com/w/index.php/INES#iNES_file_format
HEADER_CONSTANT = [0x4E, 0x45, 0x53, 0x1A]
HEADER_PADDING = [0] * 5
LOWER_NIB_MASK = int('00001111', 2)
UPPER_NIB_MASK = int('11110000', 2)
NES2 = int('00001000', 2)

# Potential TODOs, possibly not worth it as they seem to have little support pre-INES 2
PRG_RAM_SIZE = 0
FLAGS_9 = 0
FLAGS_10 = 0

# PRG INES header value is total kb / 16kb, minimal value of 1 (I think very rarely
# prg can be 8kb or less)
def prg_val(cartridge):
    return cartridge.prgKb >> 4 or 1

# CHR INES header value is total kb / 8kb, minimal value of 1 (I think very rarely
# chr can be 4kb)
def chr_val(cartridge):
    if cartridge.chrKb > 0:
        chr = cartridge.chrKb >> 3 or 1
    else:
        chr = 0
        
    return chr

# Return byte representation of flags 6, as described in link above
def flags_6(cartridge):
    if cartridge.mirroring == Mirroring.mapper or cartridge.mirroring == Mirroring.horizontal:
        mirroring = 0
    else:
        mirroring = 1

    # TODO: PRG RAM flag, trainer flag (?), four screen VRAM flag.

    mapper_lower_nibble = (cartridge.mapper & LOWER_NIB_MASK) << 4

    return mapper_lower_nibble | mirroring

# Return byte representation of flags 7, as described in link above
def flags_7(cartridge):
    # The upper nibble already occupies the upper 4 bits, so no need to shift left here
    mapper_upper_nibble = (cartridge.mapper & UPPER_NIB_MASK)

    # TODO: support other flags? Probably not though.
    
    return mapper_upper_nibble | NES2

def make_header(game):
    # For now, just grab the first cartidge revision in the list.
    cartridge = game.cartridges[0]
    
    return bytearray(HEADER_CONSTANT + [
        prg_val(cartridge),
        chr_val(cartridge),
        flags_6(cartridge),
        flags_7(cartridge),
        PRG_RAM_SIZE,
        FLAGS_9,
        FLAGS_10,
    ] + HEADER_PADDING)
    
def write_with_header(game, bin_path, out_path):
    print("Generating header and writing final file...")
    
    header = make_header(game)
    with open(bin_path, 'rb') as bin:
        with open(out_path, 'wb') as output:
            rom_data = bin.read()
            print("Read %d bytes from rom file" % len(rom_data))

            output.write(header)
            output.write(rom_data)

    print("Done! Wrote %d total bytes to\n\t%s\n" % (
        len(header) + len(rom_data),
        out_path
    ))

