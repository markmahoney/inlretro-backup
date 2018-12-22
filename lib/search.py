from fuzzywuzzy import fuzz, process

class Search:
    def __init__(self, database):
        self.cache = self.__build_cache(database)
        self.corpus = self.cache.keys()
        
    def __build_cache(self, database):
        cache = {};
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
            cache[search_name] = attrib['catalog']
            
        return cache

    def suggest(self, input):
        suggestions = process.extractBests(
            input,
            self.corpus,
            limit=20,
            # this scorer returns the most sensible results for strings like "mario 2"
            scorer=fuzz.token_set_ratio,
            score_cutoff=70
        )
        
        # Sort by score then name (multiply score by -1 so we sort largest to smallest)
        ordered = sorted(suggestions, key=lambda pair: (pair[1] * -1, pair[0]))

        return map(lambda pair: pair[0], ordered)

    def get_catalog(self, suggestion):
        return self.cache[suggestion]

