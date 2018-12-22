from collections import namedtuple

Cartridge = namedtuple(
    'Cartridge',
    [
        'console',
        'mapper',
        'mirroring',
        'prgKb',
        'chrKb',
        'vramKb',
    ])

class INLRetro:
    def __init__(self, database):
        self.database = database

    def dump(self, catalog, file):
        pass
    
