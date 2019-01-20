from collections import namedtuple
from enum import Enum

class Mirroring(Enum):
    mapper = 0
    horizontal = 1
    vertical = 2

class Console(Enum):
    nes = 1
    famicom = 2

Cartridge = namedtuple(
    'Cartridge',
    [
        'chrKb',
        'console',    # currently only supports NES and Famicom
        'mapper',     # uses ines mapper numbers, will have to do work in INLRetro code to compensate
        'mirroring',  # "H", "V", or None
        'prgKb',
        'sha',
        'vramKb',
    ])

Game = namedtuple(
    'Game',
    [
        'catalog',    # basically the ID of the game
        'cartridges',
        'name',
        'publisher',
        'region',
    ])
