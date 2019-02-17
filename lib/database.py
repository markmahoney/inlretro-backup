from lib.defs import Console
from fuzzywuzzy import fuzz, process
from lib.xml_parsers import parse_nes_cart_db, parse_no_intro_db

GENESIS_DB_PATH = 'vendor/xml/Sega - Mega Drive - Genesis (20190120-191543).dat'
NES_DB_PATH = 'vendor/xml/nesdb.xml'
N64_DB_PATH = 'vendor/xml/Nintendo - Nintendo 64 (BigEndian) (20190124-212209).dat'
SNES_DB_PATH = 'vendor/xml/Nintendo - Super Nintendo Entertainment System (Combined) (20190127-074433).dat'

databases = {}

class Database:
    def __init__(self, xml_parser, xml_path):
        def cache_by_name(acc, game):
            if game.name not in acc:
                acc[game.name] = []
            acc[game.name].append(game)
            return acc

        self.games = xml_parser(xml_path)
        self.search_cache = reduce(cache_by_name, self.games, {})
        self.search_corpus = self.search_cache.keys()

    # Returns the set of search strings most closely matching the user input
    def search(self, input, scorer = fuzz.token_set_ratio, cutoff = 70):
        suggestions = process.extractBests(
            input,
            self.search_corpus,
            limit=20,
            scorer=scorer,
            score_cutoff=cutoff
        )
        
        # Sort by score then name (multiply score by -1 so we sort largest to smallest)
        ordered = sorted(suggestions, key=lambda pair: (pair[1] * -1, pair[0]))

        return map(lambda pair: pair[0], ordered)

    # Maps game names to a set of games (usually this will cluster names across regions)
    def get_games_by_name(self, name):
        return self.search_cache[name] if name in self.search_cache else []
    
class NesDatabase(Database):
    def __init__(self, xml_parser, xml_path):
        def cache_by_catalog(acc, game):
            acc[game.catalog] = game
            return acc

        Database.__init__(self, xml_parser, xml_path)
        self.catalog = reduce(cache_by_catalog, self.games, {})

    def get_game_for_catalog(self, catalog):
        return self.catalog[catalog]

def get_database(console):
    if console == Console.NES or console == Console.FAMICOM:
        if not Console.NES in databases:
            databases[Console.NES] = NesDatabase(parse_nes_cart_db, NES_DB_PATH)
        database = databases[Console.NES]
    elif console == Console.GENESIS:
        if not Console.GENESIS in databases:
            databases[Console.GENESIS] = Database(parse_no_intro_db, GENESIS_DB_PATH)
        database = databases[Console.GENESIS]
    elif console == Console.N64:
        if not Console.N64 in databases:
            databases[Console.N64] = Database(parse_no_intro_db, N64_DB_PATH)
        database = databases[Console.N64]
    elif console == Console.SNES:
        if not Console.SNES in databases:
            databases[Console.SNES] = Database(parse_no_intro_db, SNES_DB_PATH)
        database = databases[Console.SNES]
        
    return database
