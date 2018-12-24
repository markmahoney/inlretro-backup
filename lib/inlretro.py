import os
import subprocess
import sys

EXEC = 'vendor/inlretro/host/inlretro'
PATH = 'vendor/inlretro/host/'
SCRIPT = 'scripts/inlretro2.lua'

# This is possibly incomplete or even totally wrong; I need to learn more about how
# board types map to INES mappers.
INES_TO_INLRETRO_MAPPERS = {
    0: 'nrom',
    1: 'mcc1',
    2: 'unrom',
    3: 'cnrom',
    4: 'mmc3',
    5: 'mmc5',
    10: 'mmc4',
    11: 'cdream',
    34: 'bnrom',
}

def dump(game, dump_path):
    # For now, just grab the first cartidge in the list.
    # TODO: figure out how to be less dumb about this.
    cartridge = game.cartridges[0]
    mapper = INES_TO_INLRETRO_MAPPERS[cartridge.mapper]

    if (mapper == None):
        # TODO: what's the exit strategy here? Exception?
        print("Sorry, the INLRetro currently cannot read this game!")
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
        print("------------------------------------------------------")

        child = subprocess.Popen(args, cwd=os.path.abspath(PATH), stderr=sys.stdout)
        child.wait()

        print("------------------------------------------------------")
        print("... and we're back! We got an exit code of %d" % child.returncode)
