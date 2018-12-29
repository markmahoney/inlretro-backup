from fuzzywuzzy import fuzz, process
from lib.parse_xml import parse_database

class Database:
    def __init__(self, xml_path):
        def cache_by_catalog(acc, game):
            acc[game.catalog] = game
            return acc

        def cache_by_search_string(acc, game):
            search_name = (
                "%s (%s, %s, %s)"
                % (
                    game.name,
                    game.region,
                    game.publisher,
                    game.catalog,
                ))
            
            acc[search_name] = game
            return acc

        self.games = parse_database(xml_path)
        self.catalog = reduce(cache_by_catalog, self.games, {})
        self.search_cache = reduce(cache_by_search_string, self.games, {})
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

    # Maps search strings to games
    def get_game_for_search_string(self, search_string):
        return self.search_cache[search_string]

    def get_game_for_catalog(self, catalog):
        return self.catalog[catalog]

