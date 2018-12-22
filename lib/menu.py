class Menu:
    def __init__(self):
        self.options = []

    def list(self, options):
        self.options = options

        for idx, option in enumerate(self.options):
            listing = "[%i] %s" % (idx + 1, option)
            print(listing)
            
