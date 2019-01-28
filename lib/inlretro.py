from lib.defs import Console
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

CONSOLE_TO_INLRETRO = {
    Console.FAMICOM: 'nes',
    Console.GENESIS: 'genesis',
    Console.NES: 'nes',
    Console.N64: 'n64',
}

def nes_args(game, dump_path):
    # For now, just grab the first revision in the list.
    # TODO: figure out how to be less dumb about this.
    cartridge = game.revisions[0]
    mapper = INES_TO_INLRETRO_MAPPERS.get(cartridge.mapper, None)

    if (mapper == None):
        print("Sorry, the INLRetro cannot read this game.")
        return False
    
    return [
        '-c', 'nes',
        '-d', dump_path,
        '-m', mapper,
        '-s', SCRIPT,
        '-x', str(cartridge.prgKb),
        '-y', str(cartridge.chrKb),
    ]

def console_args(game, dump_path):
    # For now, just grab the first revision in the list.
    # TODO: figure out how to be less dumb about this.
    revision = game.revisions[0]

    return [
        '--console=%s' % CONSOLE_TO_INLRETRO[game.console],
        '--rom_size_kbyte=%d' % revision.romKb,
        '--dump_filename=%s' % dump_path,
        '--lua_filename=%s' % SCRIPT,
    ]

# Returns True if we get an exit code of 0 after shelling out to the INLretro, else False
def dump_game(game, dump_path):
    if (game.console == Console.NES or game.console == Console.FAMICOM):
        cart_args = nes_args(game, dump_path)
    else:
        cart_args = console_args(game, dump_path)

    if cart_args:
        args = [os.path.abspath(EXEC)] + cart_args

        print("Shelling out to INLretro, see you on the other side...")
        print(DIVIDER)

        print(' '.join(args))
        child = subprocess.Popen(args, cwd=os.path.abspath(PATH), stderr=sys.stdout)
        child.wait()

        print(DIVIDER)
        print("... and we're back!")

        return True if child.returncode == 0 else False
    else:
        return False
