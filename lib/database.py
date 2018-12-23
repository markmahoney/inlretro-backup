# import xml.etree.ElementTree as ElementTree
from lib.parse_xml import parse_database

class Database:
    def __init__(self, xml_path):
        def cache_by_catalog(acc, game):
            acc[game.catalog] = game
            return acc

        self.games = parse_database(xml_path)
        self.catalog = reduce(cache_by_catalog, self.games, {})
        
    def get_game(self, catalog):
        return self.catalog[catalog]

