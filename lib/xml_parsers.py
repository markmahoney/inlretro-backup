from lib.defs import CartRevision, Console, Game, Mirroring, NesCartRevision
import re
import xml.etree.ElementTree as ElementTree

def parse_nes_cart_db(xml_path):
    def parse_game(game_xml):
        attrib = game_xml.attrib

        catalog = attrib['catalog']
        # Console is weird because the system attribute is attached at the cartridge
        # level, though from what I can see no game entry spans systems. So: just
        # grab the first cartridge to determine Famicom or NES, I guess.
        console = parse_console(game_xml.find('./cartridge').attrib['system'])
        name = attrib['name']
        publisher = attrib['publisher'] if 'publisher' in attrib else 'No Publisher'
        region = attrib['region'] if 'region' in attrib else 'No Region'
        revisions = map(parse_revision, game_xml.findall('./cartridge'))
    
        return Game(
            catalog = catalog,
            console = console,
            name = name,
            publisher = publisher,
            region = region,
            revisions = revisions,
        )

    def parse_size(size):
        return int(size.replace('k', ''))

    def parse_console(console):
        # This will catch both NTCS and PAL
        if 'NES' in console:
            return Console.NES
        else:
            return Console.FAMICOM

    def parse_mirroring(pad_h, pad_v):
        if pad_h == "1":
            return Mirroring.vertical
        elif pad_v == "1":
            return Mirroring.horizontal
        else:
            return Mirroring.mapper

    # Note: I'm really winging it here, shooting for something that will decently support
    # what the INLRetro needs and INES header generation for games that are likely to
    # be dumped. I'm taking a lot of liberties here. Also I have no idea what I'm doing.
    def parse_revision(cartridge_xml):
        # TODO: ensure there aren't any cartridges with multiple boards.
        board_xml = cartridge_xml.find('./board')
        chr_xml = board_xml.find('./chr')
        pad_xml = board_xml.find('./pad')
        prg_xml = board_xml.find('./prg')
        # TODO: do we actually need to document vram for INES headers?
        vram_xml = board_xml.find('./vram')
        # TODO: support wram?
        
        chrKb = parse_size(chr_xml.attrib['size']) if chr_xml is not None else 0
        mapper = int(board_xml.attrib['mapper'])
        mirroring = parse_mirroring(pad_xml.attrib['h'], pad_xml.attrib['v']) if pad_xml is not None else Mirroring.mapper
        prgKb = parse_size(prg_xml.attrib['size']) if prg_xml is not None else 0
        sha = cartridge_xml.attrib['sha1']
        vramKb = parse_size(vram_xml.attrib['size']) if vram_xml is not None else 0

        return NesCartRevision(
            chrKb = chrKb,
            mapper = mapper,
            mirroring = mirroring,
            prgKb = prgKb,
            romKb = chrKb + prgKb,
            sha = sha,
            vramKb = vramKb,
        )

    # The actual parsing starts here
    xml = ElementTree.parse(xml_path).getroot()
    return map(parse_game, xml.findall('./game'))

def parse_no_intro_db(xml_path):
    # Titles are of the form "name (regions) (languages)? (revision)? (unlicensed)?"
    # Regions are sometimes comma-separated, as are abbreviated language codes: "(USA,Europe) (En,Es,Fr)"
    # Revisions can take the form "Rev #" or sometimes "v#.#", though that list isn't exhaustive
    # This should capture name(1), regions(2), languages(3, optional), and revision(4, optional):
    TITLE_PARSER = r'^(.*?) \(([,\sa-zA-Z]+)\)(?: \(([,\sa-zA-Z]+)\))?(?: \(((?:Rev |v)[\.0-9]+)\))?'
    # Map the name of the console in the no-intro db to the console enum
    CONSOLE_NAMES = {
        'Nintendo - Nintendo 64 (BigEndian)': Console.N64,
        'Nintendo - Super Nintendo Entertainment System (Combined)': Console.SNES,
        'Sega - Mega Drive - Genesis': Console.GENESIS,
    }
    
    def parse_console(xml):
        name = xml.find('./header/name').text
        return CONSOLE_NAMES.get(name, None)

    def parse_game(console):
        def do_parsing(name_region_cache, game_xml):
            attrib = game_xml.attrib

            revision = parse_rom(game_xml.find('./rom'))
            (name, region, _) = parse_name_region_revision(attrib['name'])

            if (name, region) in name_region_cache:
                # We already have a game by this name/region, so assume this is
                # another revision
                name_region_cache[(name, region)].revisions.append(revision)
            else:
                name_region_cache[(name, region)] = Game(
                    catalog = None,
                    console = console,
                    name = name,
                    publisher = None,
                    region = region,
                    revisions = [revision],
                )

            return name_region_cache

        return do_parsing

    # See comment above TITLE_PARSER definition
    def parse_name_region_revision(title):
        return re.match(TITLE_PARSER, title).group(1, 2, 4)

    def parse_size(size):
        return int(size) / 1024

    def parse_rom(rom_xml):
        attrib = rom_xml.attrib
        
        romKb = parse_size(attrib['size'])
        sha = attrib['sha1']

        return CartRevision(
            romKb = romKb,
            sha = sha,
        )

    # The actual parsing starts here
    xml = ElementTree.parse(xml_path).getroot()
    console = parse_console(xml)
    games_by_name_region = reduce(parse_game(console), xml.findall('./game'), {})
    return games_by_name_region.values()

