from fuzzywuzzy import fuzz, process
from lib.parse_xml import parse_database

class Database:
    def __init__(self, xml_path):
        def cache_by_catalog(acc, game):
            acc[game.catalog] = game
            return acc

        def cache_by_name(acc, game):
            if game.name not in acc:
                acc[game.name] = []

            acc[game.name].append(game)
            return acc
            
        self.games = parse_database(xml_path)
        self.catalog = reduce(cache_by_catalog, self.games, {})
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
        return self.search_cache[name] if self.search_cache[name] else []

    def get_game_for_catalog(self, catalog):
        return self.catalog[catalog]

