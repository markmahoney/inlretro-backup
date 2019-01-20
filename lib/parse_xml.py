from lib.defs import Console, Cartridge, Game, Mirroring
import xml.etree.ElementTree as ElementTree

# Open the NES DB XML file, parse it, and normalize it for our own needs
def parse_database(xml_path):
    xml = ElementTree.parse(xml_path)
    root = xml.getroot()
    return map(parse_game, root.findall('./game'))

def parse_game(game_xml):
    attrib = game_xml.attrib

    cartridges = map(parse_cartridge, game_xml.findall('./cartridge'))
    catalog = attrib['catalog']
    name = attrib['name']
    publisher = attrib['publisher'] if 'publisher' in attrib else 'No Publisher'
    region = attrib['region'] if 'region' in attrib else 'No Region'
    
    return Game(
        cartridges = cartridges,
        catalog = catalog,
        name = name,
        publisher = publisher,
        region = region,
    )


def parse_size(size):
    return int(size.replace('k', ''))

# TODO: make this less dumb
def parse_console(console):
    if 'NES' in console:
        return Console.nes
    else:
        return Console.famicom

def parse_mirroring(pad_h, pad_v):
    # This is the opposite of how I'd think about it but _shrug guy_
    if pad_h == "1":
        return Mirroring.vertical
    elif pad_v == "1":
        return Mirroring.horizontal
    else:
        return Mirroring.mapper

# Note: I'm really winging it here, shooting for something that will decently support
# what the INLRetro needs and INES header generation for games that are likely to
# be dumped. I'm taking a lot of liberties here. Also I have no idea what I'm doing.
def parse_cartridge(cartridge_xml):
    # TODO: ensure there aren't any cartridges with multiple boards.
    board_xml = cartridge_xml.find('./board')
    chr_xml = board_xml.find('./chr')
    pad_xml = board_xml.find('./pad')
    prg_xml = board_xml.find('./prg')
    # TODO: do we actually need to document vram for INES headers?
    vram_xml = board_xml.find('./vram')
    # TODO: support wram?
        
    chrKb = parse_size(chr_xml.attrib['size']) if chr_xml is not None else 0
    console = parse_console(cartridge_xml.attrib['system'])
    mapper = int(board_xml.attrib['mapper'])
    mirroring = parse_mirroring(pad_xml.attrib['h'], pad_xml.attrib['v']) if pad_xml is not None else Mirroring.mapper
    prgKb = parse_size(prg_xml.attrib['size']) if prg_xml is not None else 0
    sha = cartridge_xml.attrib['sha1']
    vramKb = parse_size(vram_xml.attrib['size']) if vram_xml is not None else 0

    return Cartridge(
        chrKb = chrKb,
        console = console,
        mapper = mapper,
        mirroring = mirroring,
        prgKb = prgKb,
        sha = sha,
        vramKb = vramKb,
    )
