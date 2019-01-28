from lib.defs import Console
from fuzzywuzzy import fuzz, process
from lib.xml_parsers import parse_nes_cart_db, parse_no_intro_db

NES_DB_PATH = 'vendor/xml/nesdb.xml'
GENESIS_DB_PATH = 'vendor/xml/Sega - Mega Drive - Genesis (20190120-191543).dat'

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
    def search(self, input):
        suggestions = process.extractBests(
            input,
            self.search_corpus,
            limit=20,
            # this scorer returns the most sensible results for strings like "mario 2"
            scorer=fuzz.token_set_ratio,
            score_cutoff=70
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
    if console == Console.nes or console == Console.famicom:
        if not Console.nes in databases:
            databases[Console.nes] = NesDatabase(parse_nes_cart_db, NES_DB_PATH)
        database = databases[Console.nes]
    elif console == Console.genesis:
        if not Console.genesis in databases:
            databases[Console.genesis] = Database(parse_no_intro_db, GENESIS_DB_PATH)
        database = databases[Console.genesis]
        
    return database
