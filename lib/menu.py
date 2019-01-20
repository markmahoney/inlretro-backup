from lib.util import input_or_exit

class Menu:
    DIVIDER = "=" * 60
    SUGGEST_FORMAT = "{0:>2}. {1}"
    
    def __init__(self):
        self.options = []

    # Render the list of options and wait for user input
    def render_options(self, options):
        self.options = options

        print(self.DIVIDER)
        for idx, option in enumerate(self.options):
            print(self.SUGGEST_FORMAT.format(idx + 1, option))
        print(self.DIVIDER)
        print(self.SUGGEST_FORMAT.format('0', 'Redo search'))
        
    # Parse and validate user input
    def get_input(self):
        input = input_or_exit("\nSelection: ")
        try:
            selection = int(input)
            if selection < 0 or selection > len(self.options):
                raise ValueError
            return selection
        except (ValueError, IndexError):
            print("Invalid selection")
            return None

    # Returns the index of the selected item from a list, or None if nothing is selected
    def select_item_index(self, options):
        selection = None

        if len(options) > 0:
            self.render_options(options)
            while selection == None:
                selection = self.get_input()
        else:
            print(self.DIVIDER)
            print("No matches found")
            
        # Handle the special cases of no suggestions found or redo being selected
        if selection == None or selection == 0:
            return None
        else:
            return selection - 1

