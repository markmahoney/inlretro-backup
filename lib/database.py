import xml.etree.ElementTree as ElementTree

class Database:
    def __init__(self, xml_path):
        self.xml = ElementTree.parse(xml_path)
        self.root = self.xml.getroot()

    def games(self):
        return self.root.findall('./game')

    def game(self, catalog):
        search = "./game[@catalog=%s]" % catalog
        return self.root.find(search)
