def input_or_exit(prompt):
    try:
        return raw_input(prompt)
    except (KeyboardInterrupt, EOFError):
        print("\nExiting...")
        exit(0)
