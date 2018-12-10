# https://wiki.nesdev.com/w/index.php/INES#iNES_file_format

HEADER_CONSTANT = [0x4E, 0x45, 0x53, 0x1A]
HEADER_PADDING = [0] * 5

# TODO: arg list
IN_FILE = 'dump.bin'
OUT_FILE = 'castlevania2.nes'

# TODO: get from inlretro output???
PRG_ROM_SIZE = 8
CHR_ROM_SIZE = 16

# TODO: decompose flag generation, use arg list
FLAGS_6 = int('00010000', 2)
FLAGS_7 = int('00000000', 2)
PRG_RAM_SIZE = 0
FLAGS_9 = 0
FLAGS_10 = 0

header = bytearray(HEADER_CONSTANT + [
    PRG_ROM_SIZE,
    CHR_ROM_SIZE,
    FLAGS_6,
    FLAGS_7,
    PRG_RAM_SIZE,
    FLAGS_9,
    FLAGS_10,
    ] + HEADER_PADDING)

print('writing to %s...', OUT_FILE)
with open(IN_FILE, 'rb') as roms:
    with open(OUT_FILE, 'wb') as output:
        rom_data = roms.read()
        print('read %d bytes from rom dump', len(rom_data))

        header_bytes = output.write(header)
        print('wrote %d header bytes', header_bytes)

        rom_bytes = output.write(rom_data)
        print('wrote %d rom bytes', header_bytes)

