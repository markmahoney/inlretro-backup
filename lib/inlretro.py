import os
import subprocess
import sys

DIVIDER = "=" * 60

EXEC = 'vendor/inlretro/host/inlretro'
PATH = 'vendor/inlretro/host/'
SCRIPT = 'scripts/inlretro2.lua'

# This is possibly incomplete or even totally wrong; I need to learn more about how
# board types map to INES mappers.
INES_TO_INLRETRO_MAPPERS = {
    0: 'nrom',
    1: 'mmc1',
    2: 'unrom',
    3: 'cnrom',
    4: 'mmc3',
    5: 'mmc5',
    10: 'mmc4',
    11: 'cdream',
    34: 'bnrom',
}

# Returns True if we get an exit code of 0 after shelling out to the INL Retro, else False
def dump_game(game, dump_path):
    # For now, just grab the first cartidge in the list.
    # TODO: figure out how to be less dumb about this.
    cartridge = game.cartridges[0]
    mapper = INES_TO_INLRETRO_MAPPERS[cartridge.mapper]

    if (mapper == None):
        print("Sorry, the INLRetro cannot read this game.")
        return False
    
    else:
        args = [
            os.path.abspath(EXEC),
            '-c', 'nes',
            '-d', dump_path,
            '-m', mapper,
            '-s', SCRIPT,
            '-x', str(cartridge.prgKb),
            '-y', str(cartridge.chrKb),
        ]

        print("Shelling out to INLRetro, see you on the other side...")
        print(DIVIDER)

        child = subprocess.Popen(args, cwd=os.path.abspath(PATH), stderr=sys.stdout)
        child.wait()

        print(DIVIDER)
        print("... and we're back!")

        return True if child.returncode == 0 else False
