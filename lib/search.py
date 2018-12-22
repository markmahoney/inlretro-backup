from fuzzywuzzy import process

class Search:
    def __init__(self, database):
        self.build_cache(database)
        
    def build_cache(self, database):
        self.cache = {};
        for game in database.games():
            attrib = game.attrib
            search_name = (
                "%s (%s, %s, %s)"
                % (
                    attrib['name'],
                    attrib['publisher'] if 'publisher' in attrib else 'No Publisher',
                    attrib['region'] if 'region' in attrib else 'No Region',
                    attrib['catalog'] if 'catalog' in attrib else 'No Catalog',
                ))
            self.cache[search_name] = attrib['catalog']

    def suggest(self, input):
        return process.extractBests(input, self.cache.keys(), limit=20, score_cutoff=70)
