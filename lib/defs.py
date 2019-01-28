from collections import namedtuple
from enum import Enum

CartRevision = namedtuple(
    'CartRevision',
    (
        'romKb',
        'sha',
    ))

class Console(Enum):
    nes = 1
    famicom = 2
    genesis = 3

Game = namedtuple(
    'Game',
    (
        'catalog',    # basically the ID of the game; might remove this soon as it is NES db-specific
        'console',
        'name',
        'publisher',
        'region',
        'revisions',
    ))

class Mirroring(Enum):
    mapper = 0
    horizontal = 1
    vertical = 2

NesCartRevision = namedtuple(
    'NesCartRevision',
    CartRevision._fields +
    (
        'chrKb',
        'mapper',     # uses ines mapper numbers, will have to do work in INLRetro code to compensate
        'mirroring',  # one of the Mirroring values
        'prgKb',
        'vramKb',
    ))
