class Menu:
    def __init__(self):
        self.options = []

    # Returns a user-selected search string from a list, or None if nothing is selected
    def select_item(self, options):
        selection = None

        self.render(options)

        while(selection == None):
            selection = self.get_input()

        # Handle the special case of redo being selected
        # TODO: find a way to signal this without leaning on None so much?
        if selection == 0:
            return None
        else:
            return self.options[selection - 1]

    # Render the list of options and wait for user input
    def render(self, options):
        self.options = options
        
        for idx, option in enumerate(self.options):
            # TODO: pad spaces between index and string to line things up better
            listing = "%i. %s" % (idx + 1, option)
            print(listing)

        print("---------------")
        print("0. Redo search")

    # Parse and validate user input
    def get_input(self):
        input = raw_input("> ")
        try:
            selection = int(input)
            if selection < 0 or selection > len(self.options):
                raise ValueError
            return selection
        except (ValueError, IndexError):
            print("Invalid selection")
            return None
            
