from lib.util import input_or_exit

DIVIDER = "=" * 60
_OPTION_FORMAT = "{0:>2}. {1}"
    
# Render the list of options and wait for user input
def _render_options(options, alt_action):
    print(DIVIDER)
    for idx, option in enumerate(options):
        print(_OPTION_FORMAT.format(idx + 1, option))
    print(DIVIDER)

    if alt_action != None:
        print(_OPTION_FORMAT.format('0', alt_action))
        
# Parse and validate user input
def _get_input(options):
    input = input_or_exit("\nSelection: ")
    try:
        selection = int(input)
        if selection < 0 or selection > len(options):
            raise ValueError
        return selection
    except (ValueError, IndexError):
        print("Invalid selection")
        return None

def retry_prompt(prompt, default_yes = True):
    choice = None

    while choice == None:
        options = '[Y|n]' if default_yes else '[y|N]'
        input = input_or_exit(prompt + ' ' + options + ': ')
        if not input:
            choice = True if default_yes else False
        elif input.lower() == 'y':
            choice = True
        elif input.lower() == 'n':
            choice = False

    return choice
    
# Returns the index of the selected item from a list, or None if nothing is selected
def select_item_index(options, alt_action = None):
    selection = None

    if len(options) > 0:
        _render_options(options, alt_action)
        while selection == None:
            selection = _get_input(options)
    else:
        print(DIVIDER)
        print("No matches found")
            
    # Handle the special cases of no suggestions found or redo being selected
    if selection == None or selection == 0:
        return None
    else:
        return selection - 1

